#!/usr/bin/env python3
##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
"""Write the benchmark sources: a bare test file and the sources of whole test executables, per library.

    generate.py <dir> [--single 1,2,5,10,20,50] [--files 1,2,5,10,20,50] [--tests 200]

<dir>/units/<library>-include-0.cpp is a test file with neither runner nor test, <library>-tests-<n>.cpp the same
with n tests.
<dir>/executables/<library>-single-<n>/ holds one file: the runner and n tests.
<dir>/executables/<library>-files-<n>/ holds a runner file and n files of one test each.
"""
import argparse
import pathlib
import shutil

## runner is the unit defining main; a lone file takes prefix and suffix instead. Catch2 and GoogleTest link one.
LIBRARIES = {
  "tts": {
    "head": "#include <tts/tts.hpp>\n",
    "runner": "#define TTS_MAIN\n#include <tts/tts.hpp>\n", "prefix": "#define TTS_MAIN\n",
    "test": 'TTS_CASE("case {i}")\n{{\n  int a = {i}, b = {i} + 1;\n  TTS_EQUAL(a, {i});\n  TTS_NOT_EQUAL(a, b);\n'
            '  TTS_EXPECT(a < b);\n}};\n'},
  "catch2": {
    "head": "#include <catch2/catch_test_macros.hpp>\n",
    "test": 'TEST_CASE("case {i}")\n{{\n  int a = {i}, b = {i} + 1;\n  CHECK(a == {i});\n  CHECK(a != b);\n'
            '  CHECK(a < b);\n}}\n'},
  "doctest": {
    "head": "#include <doctest/doctest.h>\n",
    "runner": "#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN\n#include <doctest/doctest.h>\n",
    "prefix": "#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN\n",
    "test": 'TEST_CASE("case {i}")\n{{\n  int a = {i}, b = {i} + 1;\n  CHECK(a == {i});\n  CHECK(a != b);\n'
            '  CHECK(a < b);\n}}\n'},
  "gtest": {
    "head": "#include <gtest/gtest.h>\n",
    "test": 'TEST(Suite, Case{i})\n{{\n  int a = {i}, b = {i} + 1;\n  EXPECT_EQ(a, {i});\n  EXPECT_NE(a, b);\n'
            '  EXPECT_LT(a, b);\n}}\n'},
  ## included/unit_test.hpp defines the framework: one unit includes it, the others unit_test.hpp.
  "boost": {
    "head": "#include <boost/test/unit_test.hpp>\n",
    "runner": "#define BOOST_TEST_MODULE bench\n#include <boost/test/included/unit_test.hpp>\n",
    "lone": "#define BOOST_TEST_MODULE bench\n#include <boost/test/included/unit_test.hpp>\n",
    "test": 'BOOST_AUTO_TEST_CASE(case_{i})\n{{\n  int a = {i}, b = {i} + 1;\n  BOOST_CHECK_EQUAL(a, {i});\n'
            '  BOOST_CHECK_NE(a, b);\n  BOOST_CHECK_LT(a, b);\n}}\n'},
  ## ut runs its tests from a static destructor, so main is empty. 1_i is a type per value and suite<"name"> an
  ## instance per name: plain ints, one suite per file.
  "ut": {
    "head": "#include <boost/ut.hpp>\nusing namespace boost::ut;\n",
    "runner": "int main() {}\n", "suffix": "int main() {}\n",
    "open": 'static suite<"{name}"> {name} = [] {{\n', "close": "};\n",
    "test": '  "case {i}"_test = [] {{\n    int a = {i}, b = {i} + 1;\n    expect(a == {i});\n    expect(a != b);\n'
            '    expect(a < b);\n  }};\n'},
}


def tests(name, first, n, suite):
    lib = LIBRARIES[name]
    body = "".join(lib["test"].format(i=i) for i in range(first, first + n))
    return lib["open"].format(name=suite) + body + lib["close"] if "open" in lib else body


def write(path, text):
    path.write_text(text)  # NOSONAR - a path the build gives


def fresh(path):
    shutil.rmtree(path, ignore_errors=True)  # NOSONAR - a path the build gives
    path.mkdir(parents=True)  # NOSONAR - a path the build gives
    return path


def single(out, name, n):
    lib = LIBRARIES[name]
    head = lib.get("lone", lib.get("prefix", "") + lib["head"])
    write(out / "t0.cpp", head + tests(name, 0, n, "s0") + lib.get("suffix", ""))


def files(out, name, n):
    lib = LIBRARIES[name]
    if "runner" in lib:
        write(out / "main.cpp", lib["runner"])
    for f in range(n):
        write(out / ("t%d.cpp" % f), lib["head"] + tests(name, f, 1, "s%d" % f))


def counts(text):
    return [int(n) for n in text.replace(";", ",").split(",") if n]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("output")
    ap.add_argument("--single", type=counts, default="1,2,5,10,20,50")
    ap.add_argument("--files", type=counts, default="1,2,5,10,20,50")
    ap.add_argument("--tests", type=int, default=200)
    args = ap.parse_args()

    root = pathlib.Path(args.output)
    units, executables = fresh(root / "units"), fresh(root / "executables")
    for name, lib in LIBRARIES.items():
        write(units / ("%s-include-0.cpp" % name), lib["head"])
        write(units / ("%s-tests-%d.cpp" % (name, args.tests)), lib["head"] + tests(name, 0, args.tests, "s0"))
        for n in args.single:
            single(fresh(executables / ("%s-single-%d" % (name, n))), name, n)
        for n in args.files:
            files(fresh(executables / ("%s-files-%d" % (name, n))), name, n)


if __name__ == "__main__":
    main()
