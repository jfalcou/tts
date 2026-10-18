#!/usr/bin/env python3
##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
"""Draw the SVG charts and write the HTML blocks the documentation includes.

    plot.py <results.json> <dir>

Standard library only: the CI image has no matplotlib.
"""
import json
import math
import pathlib
import statistics
import sys

NAMES = {"tts": "TTS", "catch2": "Catch2", "catch2_fast": "Catch2 fast", "doctest": "doctest",
         "doctest_fast": "doctest fast", "gtest": "GoogleTest", "boost": "Boost.Test", "ut": "ut"}
COLORS = {"tts": "#d1495b", "catch2": "#2e86ab", "catch2_fast": "#7fb8d6", "doctest": "#3fa34d",
          "doctest_fast": "#86c98e", "gtest": "#e09f3e", "boost": "#8e6bbf", "ut": "#5c8a8a"}
COMPILERS = ("gcc", "clang")
KINDS = ("include", "one", "mains", "cases", "tests", "files")

WIDTH, HEIGHT = 720, 400
LEFT, RIGHT, TOP, BOTTOM = 56, 170, 40, 44
INK = "#808080"
LABEL_GAP = 14
SHORTEST_WHISKER = 2


def svg_open():
    return (f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {WIDTH} {HEIGHT}" font-family="sans-serif" '
            'font-size="12">')


def svg_text(x, y, content, attributes=""):
    return f'<text x="{x:.1f}" y="{y:.1f}" {attributes}>{content}</text>'


def svg_line(x1, x2, y1, y2, attributes):
    return f'<line x1="{x1:.1f}" x2="{x2:.1f}" y1="{y1:.1f}" y2="{y2:.1f}" {attributes}/>'


def step_for(top):
    """1, 2, 2.5 or 5 times a power of ten."""
    raw = top / 5
    power = 10 ** math.floor(math.log10(raw))
    return next(m * power for m in (1, 2, 2.5, 5, 10) if m * power >= raw)


def grid(out, y, ymax, step, right, unit):
    tick = 0.0
    while tick <= ymax + 1e-9:
        out.append(svg_line(LEFT, right, y(tick), y(tick), f'stroke="{INK}" stroke-opacity="0.3"'))
        out.append(svg_text(LEFT - 6, y(tick) + 4, f"{tick:g} {unit}", f'fill="{INK}" text-anchor="end"'))
        tick += step


def close(out):
    return "\n".join([*out, "</svg>"])


def line_chart(points, title, axis):
    """points maps a library to its (count, median, lowest, highest) sorted by count."""
    counts = sorted({p[0] for line in points.values() for p in line})
    top = max(p[3] for line in points.values() for p in line)
    step = step_for(top)
    ymax = step * math.ceil(top / step)

    # The axis spans the measured counts, with a margin on each side for the first and the last whisker.
    def x(c):
        return LEFT + 12 + (WIDTH - LEFT - RIGHT - 24) * (c - counts[0]) / max(counts[-1] - counts[0], 1)

    def y(t):
        return HEIGHT - BOTTOM - (HEIGHT - TOP - BOTTOM) * t / ymax

    out = [svg_open(), svg_text(LEFT, 22, title, f'fill="{INK}" font-size="14"')]
    grid(out, y, ymax, step, WIDTH - RIGHT, "s")
    out.extend(svg_text(x(c), HEIGHT - BOTTOM + 16, c, f'fill="{INK}" text-anchor="middle"') for c in counts)
    out.append(svg_text((LEFT + WIDTH - RIGHT) / 2, HEIGHT - 8, axis, f'fill="{INK}" text-anchor="middle"'))

    ends = sorted(((y(line[-1][1]), lib) for lib, line in points.items()), reverse=True)
    placed = []
    for end, lib in ends:
        label = min(end, placed[-1] - LABEL_GAP) if placed else end
        placed.append(label)
        out.append(svg_text(WIDTH - RIGHT + 8, label + 4, NAMES[lib], f'fill="{COLORS[lib]}" font-weight="bold"'))
    for lib, line in points.items():
        path = " ".join(f"{x(c):.1f},{y(t):.1f}" for c, t, _, _ in line)
        dash = ' stroke-dasharray="6 4"' if lib.endswith("_fast") else ""
        out.append(f'<polyline points="{path}" fill="none" stroke="{COLORS[lib]}" stroke-width="2.5"{dash}/>')
        for c, t, low, high in line:
            out.append(svg_line(x(c), x(c), y(low), y(high), f'stroke="{COLORS[lib]}" stroke-width="1.5"'))
            out.append(f'<circle cx="{x(c):.1f}" cy="{y(t):.1f}" r="3" fill="{COLORS[lib]}"/>')
    return close(out)


def column_chart(rows, title, unit="s"):
    """rows are (library, median, value label, detail, lowest, highest), sorted by median, extremes as a whisker."""
    rows = sorted(rows, key=lambda r: r[1])
    top = max(max(r[1], r[5]) for r in rows)
    step = step_for(top)
    ymax = step * math.ceil(top / step)
    bottom = 60
    slot = (WIDTH - LEFT - 16) / len(rows)

    def y(t):
        return HEIGHT - bottom - (HEIGHT - TOP - bottom) * max(t, 0) / ymax

    out = [svg_open(), svg_text(LEFT, 22, title, f'fill="{INK}" font-size="14"')]
    grid(out, y, ymax, step, WIDTH - 16, unit)
    whisker = f'stroke="{INK}" stroke-width="1.5"'
    for k, (lib, value, label, detail, low, high) in enumerate(rows):
        centre = LEFT + slot * (k + 0.5)
        out.append(f'<rect x="{centre - slot * 0.3:.1f}" y="{y(value):.1f}" width="{slot * 0.6:.1f}" '
                   f'height="{y(0) - y(value):.1f}" fill="{COLORS[lib]}"/>')
        if y(low) - y(high) >= SHORTEST_WHISKER:
            out.extend([svg_line(centre, centre, y(low), y(high), whisker),
                        svg_line(centre - 5, centre + 5, y(low), y(low), whisker),
                        svg_line(centre - 5, centre + 5, y(high), y(high), whisker)])
        out.append(svg_text(centre, min(y(value), y(high)) - 6, label, f'fill="{INK}" text-anchor="middle"'))
        # A name of two words takes two lines, so that eight columns fit the width.
        lines = NAMES[lib].split(" ") + ([detail] if detail else [])
        for n, text in enumerate(lines):
            is_detail = detail and n == len(lines) - 1
            style = f'fill="{INK}"' if is_detail else f'fill="{COLORS[lib]}" font-weight="bold"'
            out.append(svg_text(centre, HEIGHT - bottom + 16 + 13 * n, text, f'{style} text-anchor="middle"'))
    return close(out)


def by_name(libraries):
    return sorted(libraries, key=lambda lib: (lib != "tts", NAMES[lib]))


def save(destination, name, text):
    destination.mkdir(parents=True, exist_ok=True)  # NOSONAR - a path the build gives
    (destination / name).write_text(text, encoding="utf-8")  # NOSONAR - a path the build gives


def image(name, alt):
    return f'<img src="bench/{name}" alt="{alt}"/>'


def plural(count, noun):
    return f"{count} {noun}{'s' if count > 1 else ''}"


def include_blocks(rows, tag, version, destination):
    rows = [(r["library"], r["cpu"], f"{r['cpu']:.3f} s", f"{r['peak_kib'] // 1024} MiB", r["low"], r["high"])
            for r in rows if r["form"] == "include"]
    save(destination, f"{tag}-include.svg", column_chart(rows, f"{version}, a test file with no test"))
    return {"include": image(f"{tag}-include.svg", f"Include cost with {version}")}


def tests_blocks(rows, tag, version, destination):
    """(T(n) - T(1)) / (n - 1) in each round, both files being built in the same round: the first test, and what
    it instantiates once, stays out."""
    first = {r["library"]: r for r in rows if r["form"] == "tests" and r["count"] == 1}
    bars = []
    for r in (r for r in rows if r["form"] == "tests" and r["count"] > 1 and r["library"] in first):
        one, more = first[r["library"]], r["count"] - 1
        costs = [1000 * (n - o) / more for n, o in zip(r["rounds"], one["rounds"], strict=True)]
        per_test = statistics.median(costs)
        bars.append((r["library"], per_test, f"{per_test:.2f} ms", f"{r['cpu']:.2f} s in all", min(costs),
                     max(costs)))
    if not bars:
        return {}
    n = max(r["count"] for r in rows if r["form"] == "tests")
    title = f"{version}, cost of one test in a file of {n} without runner"
    save(destination, f"{tag}-tests.svg", column_chart(bars, title, "ms"))
    return {"tests": image(f"{tag}-tests.svg", f"Cost of one test with {version}")}


def curves(rows):
    points = {}
    for r in rows:
        points.setdefault(r["library"], []).append((r["count"], r["compile"] + r["link"], r["low"], r["high"]))
    return {lib: sorted(line) for lib, line in sorted(points.items())}


def single_blocks(rows, tag, version, destination):
    points = curves(rows)
    low = min(line[0][0] for line in points.values())
    high = max(line[-1][0] for line in points.values())
    one = [(lib, line[0][1], f"{line[0][1]:.2f} s", "", line[0][2], line[0][3])
           for lib, line in points.items() if line[0][0] == low]
    title = f"{version}, one file with {plural(low, 'test')}, runner included or linked"
    save(destination, f"{tag}-one.svg", column_chart(one, title))
    title = f"{version}, one file with {low} to {high} tests"
    save(destination, f"{tag}-cases.svg", line_chart(points, title, "tests in the file"))
    return {"one": image(f"{tag}-one.svg", f"One test executable with {version}"),
            "cases": image(f"{tag}-cases.svg", f"Executables of one file with {version}")}


def mains_blocks(rows, tag, version, destination):
    """One build of each runner library, so the whisker has nothing to span."""
    bars = [(r["library"], r["cpu"], f"{r['cpu']:.1f} s", plural(r["units"], "unit"), r["cpu"], r["cpu"])
            for r in rows]
    title = f"{version}, a runner library built from its sources"
    save(destination, f"{tag}-mains.svg", column_chart(bars, title))
    return {"mains": image(f"{tag}-mains.svg", f"Runner libraries with {version}")}


def files_blocks(rows, tag, version, destination):
    points = curves(rows)
    low = min(line[0][0] for line in points.values())
    high = max(line[-1][0] for line in points.values())
    title = f"{version}, a runner and {low} to {high} files of one test"
    save(destination, f"{tag}-files.svg", line_chart(points, title, "files of one test"))
    return {"files": image(f"{tag}-files.svg", f"Executables of many files with {version}")}


def per_compiler(data, compiler, version, destination):
    def of(rows, **match):
        match.update(compiler=compiler)
        return [r for r in rows if all(r.get(k) == v for k, v in match.items())]

    blocks = {}
    for rows, draw in ((of(data["units"]), include_blocks),
                       (of(data.get("mains", [])), mains_blocks),
                       (of(data["units"]), tests_blocks),
                       (of(data["executables"], shape="single"), single_blocks),
                       (of(data["executables"], shape="files"), files_blocks)):
        if rows:
            blocks.update(draw(rows, compiler, version, destination))
    return blocks


def meta(data):
    libraries = data.get("libraries", {})
    listed = "".join(f"<li>{NAMES[lib]} {libraries[lib]}</li>" for lib in by_name(libraries))
    items = [f"<b>Machine</b>: {data.get('cpu', 'an unknown CPU')}, {data.get('cores', '?')} cores, "
             f"on {data.get('date', 'an unknown date')}",
             f"<b>Compilers</b>: {' and '.join(data['compilers'].values())}, "
             f"with <code>-std=c++20 {data.get('options', '-O0')}</code>",
             f"<b>Builds</b>: the median of {data['runs']} builds per point, made in rounds that alternate the "
             f"libraries, {data.get('jobs', 1)} at a time",
             f"<b>Libraries</b>:<ul>{listed}</ul>"]
    return "<ul>\n{}\n</ul>".format("\n".join(f"<li>{item}</li>" for item in items))


def main(results, destination):
    data = json.loads(pathlib.Path(results).read_text(encoding="utf-8"))  # NOSONAR - a path the build gives
    destination = pathlib.Path(destination)
    blocks = {"meta": meta(data)}
    # The page includes every block, so an unmeasured compiler still gets its own.
    for compiler in COMPILERS:
        here = {}
        if compiler in data["compilers"]:
            version = f"{data['compilers'][compiler]} {data.get('options', '')}"
            here = per_compiler(data, compiler, version, destination)
        for kind in KINDS:
            blocks[f"{kind}-{compiler}"] = here.get(kind, f"<p>{compiler} was not measured in this build.</p>")
    for name, text in blocks.items():
        save(destination, f"bench-{name}.html", text + "\n")


if __name__ == "__main__":
    main(*sys.argv[1:])
