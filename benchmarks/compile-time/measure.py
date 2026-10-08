#!/usr/bin/env python3
##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
"""Compile each unit several times, keep the median CPU time and the peak RSS.

    measure.py <units> <out.json> --compiler NAME=PATH... --library NAME=FLAGS... [--options FLAGS] [--runs N]
               [--jobs N]
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


def compile_once(compiler, flags, unit):
    """CPU seconds and peak RSS in KiB, the driver's children included through wait4."""
    # A pipe would block the compiler once full, before wait4 returns: the diagnostics go to a file.
    with tempfile.TemporaryFile() as errors:
        process = subprocess.Popen([compiler, "-std=c++20", *flags, "-c", str(unit), "-o", os.devnull],
                                   stdout=subprocess.DEVNULL, stderr=errors)
        _, status, usage = os.wait4(process.pid, 0)
        if os.waitstatus_to_exitcode(status) != 0:
            errors.seek(0)
            raise RuntimeError("%s failed on %s:\n%s" % (compiler, unit.name, errors.read().decode(errors="replace")))
    return usage.ru_utime + usage.ru_stime, usage.ru_maxrss


def version_of(compiler):
    """`g++ 13.3.0`, from the first line of --version."""
    first = subprocess.run([compiler, "--version"], capture_output=True, text=True).stdout.splitlines()[0]
    numbers = [w for w in first.split() if "." in w and w.replace(".", "").isdigit()]
    number = numbers[-1] if numbers else first
    return "%s %s" % (pathlib.Path(compiler).name, number)


def measure(compiler, flags, unit, runs):
    samples = [compile_once(compiler, flags, unit) for _ in range(runs)]
    return {"cpu": statistics.median(cpu for cpu, _ in samples), "peak_kib": max(rss for _, rss in samples)}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("units")
    ap.add_argument("output")
    ap.add_argument("--compiler", action="append", default=[])
    ap.add_argument("--library", action="append", default=[])
    ap.add_argument("--options", default="-O0")
    ap.add_argument("--runs", type=int, default=3)
    ap.add_argument("--jobs", type=int, default=2)
    args = ap.parse_args()

    compilers = {name: path for name, _, path in (c.partition("=") for c in args.compiler)}
    options = shlex.split(args.options)
    libraries = {k: options + shlex.split(v) for k, v in (lib.split("=", 1) for lib in args.library)}

    work = []
    for unit in sorted(pathlib.Path(args.units).glob("*.cpp")):
        library, form, cases = unit.stem.rsplit("-", 2)
        if library in libraries:
            for name, path in compilers.items():
                work.append((name, path, library, form, int(cases), unit))

    results = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = {pool.submit(measure, path, libraries[library], unit, args.runs): (name, library, form, cases)
                   for name, path, library, form, cases, unit in work}
        for future in concurrent.futures.as_completed(futures):
            name, library, form, cases = futures[future]
            results.append({"compiler": name, "library": library, "form": form, "cases": cases, **future.result()})

    versions = {name: version_of(path) for name, path in compilers.items()}
    report = {"compilers": versions, "options": args.options, "runs": args.runs, "jobs": args.jobs,
              "date": datetime.date.today().isoformat(), "results": results}
    text = json.dumps(report, indent=1, sort_keys=True)
    pathlib.Path(args.output).write_text(text)  # NOSONAR - a path the build gives


if __name__ == "__main__":
    main()
