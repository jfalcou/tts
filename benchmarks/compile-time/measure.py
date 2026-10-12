#!/usr/bin/env python3
##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
"""Measure the test files without runner and build the test executables, keeping the median CPU time and the peak RSS.

    measure.py <dir> <out.json> --compiler NAME=PATH... --library NAME=FLAGS... [--link NAME=FLAGS...]
               [--main NAME=SOURCES...] [--ar PATH] [--options FLAGS] [--runs N] [--jobs N]

<dir> is what generate.py wrote. --main names the sources of a library's main library, built once per compiler,
as a list separated by semicolons or @file with one source per line. Every executable has to run and pass.
"""
import argparse
import concurrent.futures
import datetime
import json
import os
import pathlib
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
            raise RuntimeError("%s failed:\n%s" % (" ".join(command), output.read().decode(errors="replace")))
    return usage.ru_utime + usage.ru_stime, usage.ru_maxrss


def version_of(compiler):
    """`g++ 13.3.0`, from the first line of --version."""
    first = subprocess.run([compiler, "--version"], capture_output=True, text=True).stdout.splitlines()[0]
    numbers = [w for w in first.split() if "." in w and w.replace(".", "").isdigit()]
    number = numbers[-1] if numbers else first
    return "%s %s" % (pathlib.Path(compiler).name, number)


def compile_unit(compiler, flags, unit, output):
    return run([compiler, "-std=c++20", *flags, "-c", str(unit), "-o", str(output)])


def unit_cost(compiler, flags, unit, runs):
    samples = [compile_unit(compiler, flags, unit, os.devnull) for _ in range(runs)]
    return {"cpu": statistics.median(cpu for cpu, _ in samples), "peak_kib": max(rss for _, rss in samples)}


def build_main(compiler, flags, sources, work, ar):
    objects, cpu = [], 0.0
    for k, source in enumerate(sources):
        objects.append(work / ("%d.o" % k))
        cpu += compile_unit(compiler, flags, source, objects[-1])[0]
    archive = work / "libmain.a"
    run([ar, "rcs", str(archive), *map(str, objects)])
    return cpu, archive


def build_executable(compiler, flags, link, sources, runs):
    compile_cpu, link_cpu, peak = [], [], 0
    with tempfile.TemporaryDirectory() as work:
        work = pathlib.Path(work)
        for _ in range(runs):
            objects, cpu = [], 0.0
            for source in sources:
                objects.append(work / (source.stem + ".o"))
                time, rss = compile_unit(compiler, flags, source, objects[-1])
                cpu, peak = cpu + time, max(peak, rss)
            compile_cpu.append(cpu)
            link_cpu.append(run([compiler, *map(str, objects), *link, "-o", str(work / "test")])[0])
        run([str(work / "test")])
    return {"compile": statistics.median(compile_cpu), "link": statistics.median(link_cpu), "peak_kib": peak}


def sources_of(text):
    if text.startswith("@"):
        return [pathlib.Path(line) for line in pathlib.Path(text[1:]).read_text().split("\n") if line]
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


def submit_executables(pool, compiler, path, root, flags, link, runs):
    futures = {}
    for folder in sorted((root / "executables").iterdir()):
        library, shape, count = folder.name.rsplit("-", 2)
        if library in flags:
            sources = sorted(folder.glob("*.cpp"))
            future = pool.submit(build_executable, path, flags[library], link(compiler, library), sources, runs)
            futures[future] = point(compiler, library, shape=shape, count=int(count), units=len(sources))
    return futures


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("units")
    ap.add_argument("output")
    ap.add_argument("--compiler", action="append", default=[])
    ap.add_argument("--library", action="append", default=[])
    ap.add_argument("--link", action="append", default=[])
    ap.add_argument("--main", action="append", default=[])
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

        unit_futures, executables = {}, {}
        for compiler, path in compilers.items():
            for unit in sorted((root / "units").glob("*.cpp")):
                library, form, count = unit.stem.rsplit("-", 2)
                if library in flags:
                    future = pool.submit(unit_cost, path, flags[library], unit, args.runs)
                    unit_futures[future] = point(compiler, library, form=form, count=int(count))
            executables.update(submit_executables(pool, compiler, path, root, flags, link, args.runs))
        units = [{**key, **future.result()} for future, key in unit_futures.items()]
        built = [{**key, **future.result()} for future, key in executables.items()]

    versions = {name: version_of(path) for name, path in compilers.items()}
    report = {"compilers": versions, "options": args.options, "runs": args.runs, "jobs": args.jobs,
              "date": datetime.date.today().isoformat(), "units": units, "mains": mains_built,
              "executables": built}
    text = json.dumps(report, indent=1, sort_keys=True)
    pathlib.Path(args.output).write_text(text)  # NOSONAR - a path the build gives


if __name__ == "__main__":
    main()
