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
COMPILERS = ("gcc", "clang")
TITLES = {"simple": "three checks per case", "template": "three checks per case, over four types"}

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
    """Least squares fit of cpu = a + b * cases, in seconds."""
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


def chart(points, form, version):
    xmax = max(c for c, _ in (p for line in points.values() for p in line))
    step = step_for(max(t for line in points.values() for _, t in line))
    ymax = step * math.ceil(max(t for line in points.values() for _, t in line) / step)

    def x(c):
        return LEFT + (WIDTH - LEFT - RIGHT) * c / xmax

    def y(t):
        return HEIGHT - BOTTOM - (HEIGHT - TOP - BOTTOM) * t / ymax

    out = [SVG % (WIDTH, HEIGHT)]
    out.append('<text x="%d" y="22" fill="%s" font-size="14">%s, %s</text>' % (LEFT, INK, version, TITLES[form]))
    tick = 0.0
    while tick <= ymax + 1e-9:
        out.append('<line x1="%d" x2="%d" y1="%.1f" y2="%.1f" stroke="%s" stroke-opacity="0.3"/>'
                   % (LEFT, WIDTH - RIGHT, y(tick), y(tick), INK))
        out.append('<text x="%d" y="%.1f" fill="%s" text-anchor="end">%g s</text>' % (LEFT - 6, y(tick) + 4, INK, tick))
        tick += step
    out.extend('<text x="%.1f" y="%d" fill="%s" text-anchor="middle">%d</text>' % (x(c), HEIGHT - BOTTOM + 16, INK, c)
               for c in sorted({c for line in points.values() for c, _ in line}))
    out.append('<text x="%.1f" y="%d" fill="%s" text-anchor="middle">test cases in the unit</text>'
               % ((LEFT + WIDTH - RIGHT) / 2, HEIGHT - 8, INK))

    # End labels closer than 14 px are pushed apart.
    ends = sorted(((y(line[-1][1]), lib) for lib, line in points.items()), reverse=True)
    placed = []
    for ypos, lib in ends:
        if placed and placed[-1] - ypos < 14:
            ypos = placed[-1] - 14
        placed.append(ypos)
        out.append('<text x="%d" y="%.1f" fill="%s" font-weight="bold">%s</text>'
                   % (WIDTH - RIGHT + 8, ypos + 4, COLORS[lib],
                      "%s +%.1f ms" % (NAMES[lib], 1000 * linear_fit(points[lib])[1])))
    for lib, line in points.items():
        path = " ".join("%.1f,%.1f" % (x(c), y(t)) for c, t in line)
        out.append('<polyline points="%s" fill="none" stroke="%s" stroke-width="2.5"/>' % (path, COLORS[lib]))
        for c, t in line:
            out.append('<circle cx="%.1f" cy="%.1f" r="3" fill="%s"/>' % (x(c), y(t), COLORS[lib]))
    return close(out)


def include_chart(rows, version):
    """One bar per library for the unit with the runner and no test case."""
    rows = sorted(rows, key=lambda r: r[1])
    top = max(cpu for _, cpu, _ in rows)
    step = step_for(top)
    xmax = step * math.ceil(top / step)
    height = TOP + 28 * len(rows) + BOTTOM
    left, right = 96, 150

    def x(t):
        return left + (WIDTH - left - right) * t / xmax

    out = [SVG % (WIDTH, height),
           '<text x="%d" y="22" fill="%s" font-size="14">%s, minimal include cost</text>' % (left, INK, version)]
    tick = 0.0
    while tick <= xmax + 1e-9:
        out.append('<line x1="%.1f" x2="%.1f" y1="%d" y2="%d" stroke="%s" stroke-opacity="0.3"/>'
                   % (x(tick), x(tick), TOP, height - BOTTOM, INK))
        out.append('<text x="%.1f" y="%d" fill="%s" text-anchor="middle">%g s</text>'
                   % (x(tick), height - BOTTOM + 16, INK, tick))
        tick += step
    for row, (lib, cpu, peak) in enumerate(rows):
        y = TOP + 28 * row + 6
        out.extend(['<text x="%d" y="%d" fill="%s" font-weight="bold" text-anchor="end">%s</text>'
                    % (left - 8, y + 12, COLORS[lib], NAMES[lib]),
                    '<rect x="%d" y="%d" width="%.1f" height="16" fill="%s"/>' % (left, y, x(cpu) - left, COLORS[lib]),
                    '<text x="%.1f" y="%d" fill="%s">%.2f s, %d MiB</text>'
                    % (x(cpu) + 6, y + 12, INK, cpu, peak // 1024)])
    return close(out)


def types_chart(pairs, version, n):
    """Per library, n cases on distinct types against one template case over them."""
    rows = sorted(pairs.items(), key=lambda kv: kv[1][0])
    top = max(max(d, t) for d, t in pairs.values())
    step = step_for(top)
    xmax = step * math.ceil(top / step)
    height = TOP + 44 * len(rows) + BOTTOM + 20
    left, right = 96, 150

    def x(t):
        return left + (WIDTH - left - right) * t / xmax

    out = [SVG % (WIDTH, height),
           '<text x="%d" y="22" fill="%s" font-size="14">%s, %d cases on %d types, or one template case</text>'
           % (left, INK, version, n, n)]
    tick = 0.0
    while tick <= xmax + 1e-9:
        out.append('<line x1="%.1f" x2="%.1f" y1="%d" y2="%d" stroke="%s" stroke-opacity="0.3"/>'
                   % (x(tick), x(tick), TOP, height - BOTTOM - 20, INK))
        out.append('<text x="%.1f" y="%d" fill="%s" text-anchor="middle">%g s</text>'
                   % (x(tick), height - BOTTOM - 4, INK, tick))
        tick += step
    for row, (lib, (separate, together)) in enumerate(rows):
        y = TOP + 44 * row + 4
        out.extend(['<text x="%d" y="%d" fill="%s" font-weight="bold" text-anchor="end">%s</text>'
                    % (left - 8, y + 20, COLORS[lib], NAMES[lib]),
                    '<rect x="%d" y="%d" width="%.1f" height="14" fill="%s"/>'
                    % (left, y, x(separate) - left, COLORS[lib]),
                    '<rect x="%d" y="%d" width="%.1f" height="14" fill="%s" fill-opacity="0.45"/>'
                    % (left, y + 17, x(together) - left, COLORS[lib]),
                    '<text x="%.1f" y="%d" fill="%s">%.2f s</text>' % (x(separate) + 6, y + 11, INK, separate),
                    '<text x="%.1f" y="%d" fill="%s">%.2f s, %+.0f %%</text>'
                    % (x(together) + 6, y + 28, INK, together, 100 * (together - separate) / separate)])
    out.append('<text x="%d" y="%d" fill="%s">solid: %d separate cases, light: one template case</text>'
               % (left, height - 10, INK, n))
    return close(out)


def summary(data, fits, types=None):
    """HTML blocks included by the documentation, by file name."""
    forms = [(c, f) for c in data["compilers"] for f in TITLES if (c, f) in fits]
    libraries = sorted({lib for per in fits.values() for lib in per}, key=lambda lib: (lib != "tts", NAMES[lib]))
    blocks = {}

    blocks["bench-meta.html"] = (
        '<p>Measured on %s with %s and <code>%s</code>, the median of %d compilations per point, %d at a time.</p>'
        % (data.get("date", "an unknown date"), " and ".join(data["compilers"].values()), data.get("options", "-O0"),
           data["runs"], data.get("jobs", 1)))
    # The page includes every block, so an unmeasured compiler still gets its own.
    for c in COMPILERS:
        missing = "<p>%s was not measured in this build.</p>" % c
        here = c in data["compilers"]
        blocks["bench-include-%s.html" % c] = (
            '<img src="bench/%s-include.svg" alt="Minimal include cost with %s"/>' % (c, c) if here else missing)
        blocks["bench-cases-%s.html" % c] = "\n".join(
            '<img src="bench/%s-%s.svg" alt="Compile time of %s test cases with %s"/>' % (c, f, f, c)
            for cc, f in forms if cc == c) or missing
        blocks["bench-types-%s.html" % c] = (
            '<img src="bench/%s-types.svg" alt="Cases on distinct types against one template case, with %s"/>' % (c, c)
            if c in (types or {}) else missing)
        blocks["bench-fits-%s.html" % c] = fit_tables([cf for cf in forms if cf[0] == c], fits, libraries) \
                                          if any(cf[0] == c for cf in forms) else missing
    return {name: text + "\n" for name, text in blocks.items()}


def crossing(tts, other):
    """Cases up to which TTS compiles faster: a library cheaper per case crosses it at (a' - a) / (b - b')."""
    (a, b), (fixed, per_case) = tts, other
    if b <= per_case:
        return "always" if a <= fixed else "never"
    cases = (fixed - a) / (b - per_case)
    return "%.0f" % cases if cases > 0 else "never"


def fit_tables(forms, fits, libraries):
    """Fitted costs, then crossing points with TTS."""
    head = "".join("<th>%s cases</th>" % f for _, f in forms)
    out = ['<p>Cost of a unit holding \\(n\\) test cases, fitted as a fixed part and a part per case:</p>',
           '<table class="markdownTable"><tr class="markdownTableHead"><th>Library</th>%s</tr>' % head]
    for lib in libraries:
        # MathJax 2 renders \( \) in raw HTML; $ is not one of its delimiters by default.
        cells = "".join("<td>\\(%.2f\\,\\mathrm{s} + %.1f\\,\\mathrm{ms} \\cdot n\\)</td>"
                        % (fits[cf][lib][0], 1000 * fits[cf][lib][1]) if lib in fits[cf]
                        else "<td></td>" for cf in forms)
        out.append("<tr><td>%s</td>%s</tr>" % (NAMES[lib], cells))
    out.extend(["</table>",
                '<p>Number of test cases in a unit up to which <b>TTS</b> compiles faster:</p>',
                '<table class="markdownTable"><tr class="markdownTableHead"><th>Against</th>%s</tr>' % head])
    for lib in (lib for lib in libraries if lib != "tts"):
        cells = "".join("<td>%s</td>" % (crossing(fits[cf]["tts"], fits[cf][lib])
                                          if lib in fits[cf] and "tts" in fits[cf] else "") for cf in forms)
        out.append("<tr><td>%s</td>%s</tr>" % (NAMES[lib], cells))
    out.append("</table>")
    return "\n".join(out)


def save(destination, name, text):
    destination.mkdir(parents=True, exist_ok=True)  # NOSONAR - a path the build gives
    (destination / name).write_text(text)  # NOSONAR - a path the build gives


def case_charts(data, compiler, version, destination):
    """The line charts of one compiler, and its fitted costs by form."""
    fits_by_form = {}
    for form in TITLES:
        points = {}
        for r in data["results"]:
            if r["compiler"] == compiler and r["form"] == form:
                points.setdefault(r["library"], []).append((r["cases"], r["cpu"]))
        if not points:
            continue
        points = {lib: sorted(line) for lib, line in sorted(points.items())}
        fits = {lib: linear_fit(line) for lib, line in points.items()}
        fits_by_form[(compiler, form)] = fits
        for lib, (fixed, per_case) in fits.items():
            print("%-6s %-9s %-11s %7.3f s + %6.2f ms per case" % (compiler, form, NAMES[lib], fixed, 1000 * per_case))
            if lib != "tts" and "tts" in fits:
                print("%-6s %-9s TTS ahead of %s: %s" % (compiler, form, NAMES[lib], crossing(fits["tts"], fits[lib])))
        save(destination, "%s-%s.svg" % (compiler, form), chart(points, form, version))
    return fits_by_form


def types_pairs(data, compiler):
    """At the largest N, the time of N separate cases and of one template case, per library."""
    rows = [r for r in data["results"] if r["compiler"] == compiler and r["form"] in ("distinct", "onetemplate")]
    if not rows:
        return 0, {}
    n = max(r["cases"] for r in rows)
    pairs = {}
    for r in (r for r in rows if r["cases"] == n):
        pairs.setdefault(r["library"], [0.0, 0.0])[r["form"] == "onetemplate"] = r["cpu"]
    return n, pairs


def main(results, destination):
    data = json.loads(pathlib.Path(results).read_text())  # NOSONAR - a path the build gives
    destination = pathlib.Path(destination)
    all_fits, types = {}, {}
    for compiler, version in data["compilers"].items():
        version = "%s %s" % (version, data.get("options", ""))
        rows = [(r["library"], r["cpu"], r["peak_kib"]) for r in data["results"]
                if r["compiler"] == compiler and r["form"] == "include"]
        if rows:
            save(destination, "%s-include.svg" % compiler, include_chart(rows, version))
        all_fits.update(case_charts(data, compiler, version, destination))
        n, pairs = types_pairs(data, compiler)
        if pairs:
            types[compiler] = n
            save(destination, "%s-types.svg" % compiler, types_chart(pairs, version, n))
    for name, text in summary(data, all_fits, types).items():
        save(destination, name, text)


if __name__ == "__main__":
    main(*sys.argv[1:])
