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
import sys

NAMES = {"tts": "TTS", "catch2": "Catch2", "doctest": "doctest", "gtest": "GoogleTest", "boost": "Boost.Test",
         "ut": "ut"}
COLORS = {"tts": "#d1495b", "catch2": "#2e86ab", "doctest": "#3fa34d", "gtest": "#e09f3e", "boost": "#8e6bbf",
          "ut": "#5c8a8a"}
URLS = {"catch2": "https://github.com/catchorg/Catch2", "doctest": "https://github.com/doctest/doctest",
        "gtest": "https://github.com/google/googletest", "boost": "https://github.com/boostorg/test",
        "ut": "https://github.com/boost-ext/ut"}
COMPILERS = ("gcc", "clang")

WIDTH, HEIGHT = 720, 400
LEFT, RIGHT, TOP, BOTTOM = 56, 170, 40, 44
INK = "#808080"
SVG = '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 %d %d" font-family="sans-serif" font-size="12">'


def step_for(top):
    """1, 2, 2.5 or 5 times a power of ten."""
    raw = top / 5
    power = 10 ** math.floor(math.log10(raw))
    return next(m * power for m in (1, 2, 2.5, 5, 10) if m * power >= raw)


def linear_fit(line):
    """Least squares fit of cpu = a + b * tests, in seconds."""
    n = len(line)
    if n < 2:
        return line[0][1], 0.0
    mx = sum(c for c, _ in line) / n
    my = sum(t for _, t in line) / n
    sxx = sum((c - mx) ** 2 for c, _ in line)
    slope = sum((c - mx) * (t - my) for c, t in line) / sxx if sxx else 0.0
    return my - slope * mx, slope


def close(out):
    return "\n".join([*out, "</svg>"])


def line_chart(points, title, axis):
    xmax = max(c for line in points.values() for c, _ in line)
    step = step_for(max(t for line in points.values() for _, t in line))
    ymax = step * math.ceil(max(t for line in points.values() for _, t in line) / step)

    def x(c):
        return LEFT + (WIDTH - LEFT - RIGHT) * c / xmax

    def y(t):
        return HEIGHT - BOTTOM - (HEIGHT - TOP - BOTTOM) * t / ymax

    out = [SVG % (WIDTH, HEIGHT)]
    out.append('<text x="%d" y="22" fill="%s" font-size="14">%s</text>' % (LEFT, INK, title))
    tick = 0.0
    while tick <= ymax + 1e-9:
        out.append('<line x1="%d" x2="%d" y1="%.1f" y2="%.1f" stroke="%s" stroke-opacity="0.3"/>'
                   % (LEFT, WIDTH - RIGHT, y(tick), y(tick), INK))
        out.append('<text x="%d" y="%.1f" fill="%s" text-anchor="end">%g s</text>' % (LEFT - 6, y(tick) + 4, INK, tick))
        tick += step
    out.extend('<text x="%.1f" y="%d" fill="%s" text-anchor="middle">%d</text>' % (x(c), HEIGHT - BOTTOM + 16, INK, c)
               for c in sorted({c for line in points.values() for c, _ in line}))
    out.append('<text x="%.1f" y="%d" fill="%s" text-anchor="middle">%s</text>'
               % ((LEFT + WIDTH - RIGHT) / 2, HEIGHT - 8, INK, axis))

    # End labels closer than 14 px are pushed apart.
    ends = sorted(((y(line[-1][1]), lib) for lib, line in points.items()), reverse=True)
    placed = []
    for ypos, lib in ends:
        if placed and placed[-1] - ypos < 14:
            ypos = placed[-1] - 14
        placed.append(ypos)
        out.append('<text x="%d" y="%.1f" fill="%s" font-weight="bold">%s</text>'
                   % (WIDTH - RIGHT + 8, ypos + 4, COLORS[lib],
                      "%s %+.1f ms" % (NAMES[lib], 1000 * linear_fit(points[lib])[1])))
    for lib, line in points.items():
        path = " ".join("%.1f,%.1f" % (x(c), y(t)) for c, t in line)
        out.append('<polyline points="%s" fill="none" stroke="%s" stroke-width="2.5"/>' % (path, COLORS[lib]))
        for c, t in line:
            out.append('<circle cx="%.1f" cy="%.1f" r="3" fill="%s"/>' % (x(c), y(t), COLORS[lib]))
    return close(out)


def bar_chart(rows, title, unit="s"):
    rows = sorted(rows, key=lambda r: r[1])
    top = max(cpu for _, cpu, _ in rows)
    step = step_for(top)
    xmax = step * math.ceil(top / step)
    height = TOP + 28 * len(rows) + BOTTOM
    left, right = 96, 170

    def x(t):
        return left + (WIDTH - left - right) * t / xmax

    out = [SVG % (WIDTH, height), '<text x="%d" y="22" fill="%s" font-size="14">%s</text>' % (left, INK, title)]
    tick = 0.0
    while tick <= xmax + 1e-9:
        out.append('<line x1="%.1f" x2="%.1f" y1="%d" y2="%d" stroke="%s" stroke-opacity="0.3"/>'
                   % (x(tick), x(tick), TOP, height - BOTTOM, INK))
        out.append('<text x="%.1f" y="%d" fill="%s" text-anchor="middle">%g %s</text>'
                   % (x(tick), height - BOTTOM + 16, INK, tick, unit))
        tick += step
    for row, (lib, cpu, label) in enumerate(rows):
        y = TOP + 28 * row + 6
        out.extend(['<text x="%d" y="%d" fill="%s" font-weight="bold" text-anchor="end">%s</text>'
                    % (left - 8, y + 12, COLORS[lib], NAMES[lib]),
                    '<rect x="%d" y="%d" width="%.1f" height="16" fill="%s"/>' % (left, y, x(cpu) - left, COLORS[lib]),
                    '<text x="%.1f" y="%d" fill="%s">%s</text>' % (x(cpu) + 6, y + 12, INK, label)])
    return close(out)


def fit_table(fits):
    out = ['<p>Cost of an executable from one file holding \\(n\\) tests, fitted as a fixed part and a part per test:'
           '</p>',
           '<table class="markdownTable"><tr class="markdownTableHead"><th>Library</th><th>Cost</th></tr>']
    for lib in sorted(fits, key=lambda lib: (lib != "tts", NAMES[lib])):
        # MathJax 2 renders \( \) in raw HTML; $ is not one of its delimiters by default.
        fixed, per_test = fits[lib]
        cost = "\\(%.2f\\,\\mathrm{s} %s %.1f\\,\\mathrm{ms} \\cdot n\\)" % (fixed, "+" if per_test >= 0 else "-",
                                                                         1000 * abs(per_test))
        name = '<a href="%s">%s</a>' % (URLS[lib], NAMES[lib]) if lib in URLS else NAMES[lib]
        out.append("<tr><td>%s</td><td>%s</td></tr>" % (name, cost))
    out.append("</table>")
    return "\n".join(out)


def save(destination, name, text):
    destination.mkdir(parents=True, exist_ok=True)  # NOSONAR - a path the build gives
    (destination / name).write_text(text)  # NOSONAR - a path the build gives


def image(name, alt):
    return '<img src="bench/%s" alt="%s"/>' % (name, alt)


def include_blocks(rows, compiler, version, destination):
    rows = [(r["library"], r["cpu"], "%.3f s, %d MiB" % (r["cpu"], r["peak_kib"] // 1024))
            for r in rows if r["form"] == "include"]
    save(destination, "%s-include.svg" % compiler, bar_chart(rows, "%s, a test file with no test" % version))
    return {"include": image("%s-include.svg" % compiler, "Include cost with %s" % compiler)}


def tests_blocks(rows, compiler, version, destination):
    bare = {r["library"]: r["cpu"] for r in rows if r["form"] == "include"}
    bars = []
    for r in (r for r in rows if r["form"] == "tests" and r["library"] in bare):
        per_test = 1000 * (r["cpu"] - bare[r["library"]]) / r["count"]
        bars.append((r["library"], per_test, "%.2f ms, %.2f s for %d tests" % (per_test, r["cpu"], r["count"])))
    if not bars:
        return {}
    n = max(r["count"] for r in rows if r["form"] == "tests")
    title = "%s, cost of one test in a file of %d without runner" % (version, n)
    save(destination, "%s-tests.svg" % compiler, bar_chart(bars, title, "ms"))
    return {"tests": image("%s-tests.svg" % compiler, "Cost of one test with %s" % compiler)}


def single_blocks(rows, compiler, version, destination):
    points = {}
    for r in rows:
        points.setdefault(r["library"], []).append((r["count"], r["compile"] + r["link"]))
    points = {lib: sorted(line) for lib, line in sorted(points.items())}
    low = min(line[0][0] for line in points.values())
    high = max(line[-1][0] for line in points.values())
    one = [(lib, line[0][1], "%.2f s" % line[0][1]) for lib, line in points.items() if line[0][0] == low]
    save(destination, "%s-one.svg" % compiler, bar_chart(one, "%s, one file: runner and %d test" % (version, low)))
    save(destination, "%s-cases.svg" % compiler,
         line_chart(points, "%s, one file: runner and %d to %d tests" % (version, low, high), "tests in the file"))
    fits = {lib: linear_fit(line) for lib, line in points.items()}
    for lib, (fixed, per_test) in fits.items():
        print("%-6s %-11s %7.3f s + %6.2f ms per test" % (compiler, NAMES[lib], fixed, 1000 * per_test))
    return {"one": image("%s-one.svg" % compiler, "One test executable with %s" % compiler),
            "cases": image("%s-cases.svg" % compiler, "Executables of one file with %s" % compiler),
            "fits": fit_table(fits)}


def files_blocks(rows, compiler, version, destination):
    points = {}
    for r in rows:
        points.setdefault(r["library"], []).append((r["count"], r["compile"] + r["link"]))
    points = {lib: sorted(line) for lib, line in sorted(points.items())}
    low = min(line[0][0] for line in points.values())
    high = max(line[-1][0] for line in points.values())
    title = "%s, a runner and %d to %d files of one test" % (version, low, high)
    save(destination, "%s-files.svg" % compiler, line_chart(points, title, "files of one test"))
    return {"files": image("%s-files.svg" % compiler, "Executables of many files with %s" % compiler)}


def per_compiler(data, compiler, version, destination):
    def of(rows, **match):
        return [r for r in rows if r["compiler"] == compiler and all(r[k] == v for k, v in match.items())]

    blocks = {}
    for rows, draw in ((of(data["units"]), include_blocks),
                       (of(data["units"]), tests_blocks),
                       (of(data["executables"], shape="single"), single_blocks),
                       (of(data["executables"], shape="files"), files_blocks)):
        if rows:
            blocks.update(draw(rows, compiler, version, destination))
    return blocks


def main(results, destination):
    data = json.loads(pathlib.Path(results).read_text())  # NOSONAR - a path the build gives
    destination = pathlib.Path(destination)
    blocks = {"meta": '<p>Measured on %s with %s and <code>%s</code>, the median of %d builds per point, %d at a time.'
                      '</p>' % (data.get("date", "an unknown date"), " and ".join(data["compilers"].values()),
                                data.get("options", "-O0"), data["runs"], data.get("jobs", 1))}
    # The page includes every block, so an unmeasured compiler still gets its own.
    for compiler in COMPILERS:
        here = {}
        if compiler in data["compilers"]:
            version = "%s %s" % (data["compilers"][compiler], data.get("options", ""))
            here = per_compiler(data, compiler, version, destination)
        for kind in ("include", "one", "cases", "fits", "tests", "files"):
            blocks["%s-%s" % (kind, compiler)] = here.get(kind, "<p>%s was not measured in this build.</p>" % compiler)
    for name, text in blocks.items():
        save(destination, "bench-%s.html" % name, text + "\n")


if __name__ == "__main__":
    main(*sys.argv[1:])
