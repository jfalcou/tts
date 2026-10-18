#!/usr/bin/env python3
##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
"""Measure the test files without runner and build the test executables, keeping the median CPU time and the peak RSS.

    measure.py <dir> <out.json> --compiler NAME=PATH... --library NAME=FLAGS... [--link NAME=FLAGS...]
               [--main NAME=SOURCES...] [--version NAME=TEXT...] [--ar PATH] [--options FLAGS] [--runs N] [--jobs N]

<dir> is what generate.py wrote. --main names the sources of a library's main library, built once per compiler,
as a list separated by semicolons or @file with one source per line. Every executable has to run and pass.
"""
import argparse
import concurrent.futures
import datetime
import json
import os
import pathlib
import platform
import shlex
import statistics
import subprocess
import tempfile


def run(command):
    """CPU seconds and peak RSS in KiB, the driver's children included through wait4."""
    # A pipe would block the process once full, before wait4 returns: the output goes to a file.
    with tempfile.TemporaryFile() as output:
        process = subprocess.Popen(command, stdout=output, stderr=subprocess.STDOUT)
        _, status, usage = os.wait4(process.pid, 0)
        if os.waitstatus_to_exitcode(status) != 0:
            output.seek(0)
            raise RuntimeError(f"{' '.join(command)} failed:\n{output.read().decode(errors='replace')}")
    return usage.ru_utime + usage.ru_stime, usage.ru_maxrss


def version_of(compiler):
    """`g++ 13.3.0`, from the first line of --version."""
    first = subprocess.run([compiler, "--version"], capture_output=True, text=True, check=True).stdout.splitlines()[0]
    numbers = [w for w in first.split() if "." in w and w.replace(".", "").isdigit()]
    number = numbers[-1] if numbers else first
    return f"{pathlib.Path(compiler).name} {number}"


def compile_unit(compiler, flags, unit, output):
    return run([compiler, "-std=c++20", *flags, "-c", str(unit), "-o", str(output)])


def build_main(compiler, flags, sources, work, ar):
    objects, cpu = [], 0.0
    for k, source in enumerate(sources):
        objects.append(work / f"{k}.o")
        cpu += compile_unit(compiler, flags, source, objects[-1])[0]
    archive = work / "libmain.a"
    run([ar, "rcs", str(archive), *map(str, objects)])
    return cpu, archive


def unit_sample(compiler, flags, unit, check):
    cpu, rss = compile_unit(compiler, flags, unit, os.devnull)
    return {"cpu": cpu, "peak_kib": rss}


def executable_sample(compiler, flags, link, sources, check):
    with tempfile.TemporaryDirectory() as scratch:
        work = pathlib.Path(scratch)
        objects, cpu, peak = [], 0.0, 0
        for source in sources:
            objects.append(work / (source.stem + ".o"))
            time, rss = compile_unit(compiler, flags, source, objects[-1])
            cpu, peak = cpu + time, max(peak, rss)
        link_cpu = run([compiler, *map(str, objects), *link, "-o", str(work / "test")])[0]
        if check:
            run([str(work / "test")])
    return {"compile": cpu, "link": link_cpu, "peak_kib": peak}


def cpu_model():
    try:
        for line in pathlib.Path("/proc/cpuinfo").read_text(encoding="utf-8").split("\n"):
            if line.startswith("model name"):
                return line.split(":", 1)[1].strip()
    except OSError:
        pass
    return platform.processor() or "an unknown CPU"


def sources_of(text):
    if text.startswith("@"):
        return [pathlib.Path(line) for line in pathlib.Path(text[1:]).read_text(encoding="utf-8").split("\n") if line]
    return [pathlib.Path(s) for s in text.split(";") if s]


def pairs(values):
    return {k: v for k, _, v in (value.partition("=") for value in values)}


def point(compiler, library, **rest):
    return {"compiler": compiler, "library": library, **rest}


def build_mains(pool, compilers, flags, mains, scratch, ar):
    futures = {}
    for compiler, path in compilers.items():
        for library, sources in mains.items():
            work = scratch / compiler / library
            work.mkdir(parents=True)
            futures[pool.submit(build_main, path, flags[library], sources, work, ar)] = (compiler, library)
    archives, built = {}, []
    for future in concurrent.futures.as_completed(futures):
        compiler, library = futures[future]
        cpu, archives[(compiler, library)] = future.result()
        built.append(point(compiler, library, cpu=cpu, units=len(mains[library])))
    return archives, built


def tasks_of(compilers, root, flags, link):
    """Sorted so that the libraries alternate within a round."""
    tasks = []
    for compiler, path in compilers.items():
        for unit in sorted((root / "units").glob("*.cpp")):
            library, form, count = unit.stem.rsplit("-", 2)
            if library in flags:
                key = point(compiler, library, form=form, count=int(count))
                tasks.append((key, unit_sample, (path, flags[library], unit)))
        for folder in sorted((root / "executables").iterdir()):
            library, shape, count = folder.name.rsplit("-", 2)
            if library in flags:
                sources = sorted(folder.glob("*.cpp"))
                key = point(compiler, library, shape=shape, count=int(count), units=len(sources))
                tasks.append((key, executable_sample, (path, flags[library], link(compiler, library), sources)))
    return sorted(tasks, key=lambda t: (t[0]["compiler"], t[0].get("form", t[0].get("shape")), t[0]["count"],
                                        t[0]["library"]))


def measure_rounds(pool, tasks, runs):
    """Each round builds every point once, starting further down the list; the first one runs the executables."""
    samples = [[] for _ in tasks]
    for r in range(runs):
        shift = r * len(tasks) // runs
        order = list(range(len(tasks)))[shift:] + list(range(len(tasks)))[:shift]
        futures = {pool.submit(tasks[k][1], *tasks[k][2], r == 0): k for k in order}
        for future in concurrent.futures.as_completed(futures):
            samples[futures[future]].append(future.result())
    return [summarize(key, s) for (key, _, _), s in zip(tasks, samples, strict=True)]


def summarize(key, samples):
    """Medians, the shortest and the longest build, and every build in the order of the rounds."""
    peak = max(s["peak_kib"] for s in samples)
    if "compile" in samples[0]:
        totals = [s["compile"] + s["link"] for s in samples]
        return {**key, "compile": statistics.median(s["compile"] for s in samples),
                "link": statistics.median(s["link"] for s in samples), "low": min(totals), "high": max(totals),
                "rounds": totals, "peak_kib": peak}
    cpus = [s["cpu"] for s in samples]
    return {**key, "cpu": statistics.median(cpus), "low": min(cpus), "high": max(cpus), "rounds": cpus,
            "peak_kib": peak}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("units")
    ap.add_argument("output")
    ap.add_argument("--compiler", action="append", default=[])
    ap.add_argument("--library", action="append", default=[])
    ap.add_argument("--link", action="append", default=[])
    ap.add_argument("--main", action="append", default=[])
    ap.add_argument("--version", action="append", default=[])
    ap.add_argument("--ar", default="ar")
    ap.add_argument("--options", default="-O0")
    ap.add_argument("--runs", type=int, default=3)
    ap.add_argument("--jobs", type=int, default=2)
    args = ap.parse_args()

    root = pathlib.Path(args.units)
    compilers = pairs(args.compiler)
    options = shlex.split(args.options)
    flags = {k: options + shlex.split(v) for k, v in pairs(args.library).items()}
    links = {k: shlex.split(v) for k, v in pairs(args.link).items()}
    mains = {k: sources_of(v) for k, v in pairs(args.main).items() if k in flags}

    with tempfile.TemporaryDirectory() as scratch, \
         concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        archives, mains_built = build_mains(pool, compilers, flags, mains, pathlib.Path(scratch), args.ar)

        def link(compiler, library):
            archive = archives.get((compiler, library))
            return ([str(archive)] if archive else []) + links.get(library, [])

        points = measure_rounds(pool, tasks_of(compilers, root, flags, link), args.runs)
        units = [p for p in points if "form" in p]
        built = [p for p in points if "shape" in p]

    versions = {name: version_of(path) for name, path in compilers.items()}
    report = {"compilers": versions, "libraries": pairs(args.version), "cpu": cpu_model(), "cores": os.cpu_count(),
              "options": args.options,
              "runs": args.runs, "jobs": args.jobs,
              "date": datetime.date.today().isoformat(), "units": units, "mains": mains_built,
              "executables": built}
    text = json.dumps(report, indent=1, sort_keys=True)
    pathlib.Path(args.output).write_text(text, encoding="utf-8")  # NOSONAR - a path the build gives


if __name__ == "__main__":
    main()
