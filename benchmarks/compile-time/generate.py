#!/usr/bin/env python3
##======================================================================================================================
##  TTS - Tiny Test System
##  Copyright : TTS Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
"""Write the benchmark units: one per library, form of test case and number of cases.

    generate.py <dir> [--simple 1,50,200] [--template 1,25,100] [--types 1,25,100]
"""
import argparse
import pathlib

TYPES = "short, int, float, double"
SETUP = "  T a = T({i}), b = T({i} + 1);\n"

LIBRARIES = {
  "tts": {
    "head": "#define TTS_MAIN\n#include <tts/tts.hpp>\n",
    "simple": 'TTS_CASE("case {i}")\n{{\n  int a = {i}, b = {i} + 1;\n  TTS_EQUAL(a, {i});\n  TTS_NOT_EQUAL(a, b);\n'
           '  TTS_EXPECT(a < b);\n}};\n',
    "template": 'TTS_CASE_TPL("case {i}", ' + TYPES + ')\n<typename T>(tts::type<T>)\n{{\n' + SETUP +
             '  TTS_EQUAL(a, T({i}));\n  TTS_NOT_EQUAL(a, b);\n  TTS_EXPECT(a < b);\n}};\n'},
  "catch2": {
    "head": "#include <catch2/catch_template_test_macros.hpp>\n#include <catch2/catch_test_macros.hpp>\n",
    "simple": 'TEST_CASE("case {i}")\n{{\n  int a = {i}, b = {i} + 1;\n  CHECK(a == {i});\n  CHECK(a != b);\n'
           '  CHECK(a < b);\n}}\n',
    "template": 'TEMPLATE_TEST_CASE("case {i}", "", ' + TYPES + ')\n{{\n  using T = TestType;\n' + SETUP +
             '  CHECK(a == T({i}));\n  CHECK(a != b);\n  CHECK(a < b);\n}}\n'},
  "doctest": {
    "head": "#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN\n#include <doctest/doctest.h>\n",
    "simple": 'TEST_CASE("case {i}")\n{{\n  int a = {i}, b = {i} + 1;\n  CHECK(a == {i});\n  CHECK(a != b);\n'
           '  CHECK(a < b);\n}}\n',
    "template": 'TEST_CASE_TEMPLATE("case {i}", T, ' + TYPES + ')\n{{\n' + SETUP +
             '  CHECK(a == T({i}));\n  CHECK(a != b);\n  CHECK(a < b);\n}}\n'},
  "gtest": {
    "head": "#include <gtest/gtest.h>\nusing Types = ::testing::Types<" + TYPES + ">;\n",
    "simple": 'TEST(Suite, Case{i})\n{{\n  int a = {i}, b = {i} + 1;\n  EXPECT_EQ(a, {i});\n  EXPECT_NE(a, b);\n'
           '  EXPECT_LT(a, b);\n}}\n',
    "template": 'template<typename T> struct S{i} : ::testing::Test {{}};\nTYPED_TEST_SUITE(S{i}, Types);\n'
             'TYPED_TEST(S{i}, Case)\n{{\n  using T = TypeParam;\n' + SETUP +
             '  EXPECT_EQ(a, T({i}));\n  EXPECT_NE(a, b);\n  EXPECT_LT(a, b);\n}}\n'},
  "boost": {
    "head": "#define BOOST_TEST_MODULE bench\n#include <boost/test/included/unit_test.hpp>\n#include <tuple>\n"
         "using Types = std::tuple<" + TYPES + ">;\n",
    "simple": 'BOOST_AUTO_TEST_CASE(case_{i})\n{{\n  int a = {i}, b = {i} + 1;\n  BOOST_CHECK_EQUAL(a, {i});\n'
           '  BOOST_CHECK_NE(a, b);\n  BOOST_CHECK_LT(a, b);\n}}\n',
    "template": 'BOOST_AUTO_TEST_CASE_TEMPLATE(case_{i}, T, Types)\n{{\n' + SETUP +
             '  BOOST_CHECK(a == T({i}));\n  BOOST_CHECK(a != b);\n  BOOST_CHECK(a < b);\n}}\n'},
  "ut": {
    "head": "#include <boost/ut.hpp>\n#include <tuple>\nusing namespace boost::ut;\n",
    "simple": 'static suite<"s{i}"> s{i} = [] {{\n  "case {i}"_test = [] {{\n    int a = {i}, b = {i} + 1;\n'
           '    expect(a == {i}_i);\n    expect(a != b);\n    expect(a < b);\n  }};\n}};\n',
    "template": 'static suite<"s{i}"> s{i} = [] {{\n  "case {i}"_test = []<class T>(T) {{\n  ' + SETUP +
             '    expect(a == T({i}));\n    expect(a != b);\n    expect(a < b);\n  }} | std::tuple<' + TYPES +
             '>{{}};\n}};\n',
    "tail": "int main() {}\n"},
}

## distinct and onetemplate check the same types, so their gap is the template machinery alone.
DISTINCT_TYPE = ("template<int K> struct num\n{\n  int v;\n  constexpr num(int x) : v(x) {}\n"
                 "  friend constexpr bool operator==(num, num) = default;\n"
                 "  friend constexpr auto operator<=>(num, num) = default;\n};\n")
TYPED_SETUP = "  T a = T(1), b = T(2);\n"
TYPED_CHECKS = {
  "tts": "  TTS_EQUAL(a, T(1));\n  TTS_NOT_EQUAL(a, b);\n  TTS_EXPECT(a < b);\n",
  "catch2": "  CHECK(a == T(1));\n  CHECK(a != b);\n  CHECK(a < b);\n",
  "doctest": "  CHECK(a == T(1));\n  CHECK(a != b);\n  CHECK(a < b);\n",
  "gtest": "  EXPECT_TRUE(a == T(1));\n  EXPECT_TRUE(a != b);\n  EXPECT_TRUE(a < b);\n",
  "boost": "  BOOST_CHECK(a == T(1));\n  BOOST_CHECK(a != b);\n  BOOST_CHECK(a < b);\n",
  "ut": "  expect(a == T(1));\n  expect(a != b);\n  expect(a < b);\n",
}


def distinct(name, n):
    """n test cases, the k-th on num<k>."""
    out = []
    for k in range(n):
        body = "  using T = num<%d>;\n" % k + TYPED_SETUP + TYPED_CHECKS[name]
        out.append({"tts": 'TTS_CASE("case %d")\n{\n%s};\n',
                    "catch2": 'TEST_CASE("case %d")\n{\n%s}\n',
                    "doctest": 'TEST_CASE("case %d")\n{\n%s}\n',
                    "gtest": 'TEST(Suite, Case%d)\n{\n%s}\n',
                    "boost": 'BOOST_AUTO_TEST_CASE(case_%d)\n{\n%s}\n',
                    "ut": 'static suite<"s%d"> s%d = [] {\n  "case"_test = [] {\n%s  };\n};\n'}[name]
                   % ((k, k, body) if name == "ut" else (k, body)))
    return "".join(out)


def one_template(name, n):
    """One template test case over num<0> .. num<n-1>."""
    types = ", ".join("num<%d>" % k for k in range(n))
    body = TYPED_SETUP + TYPED_CHECKS[name]
    if name == "tts":
        return 'TTS_CASE_TPL("case", %s)\n<typename T>(tts::type<T>)\n{\n%s};\n' % (types, body)
    if name == "catch2":
        return 'TEMPLATE_TEST_CASE("case", "", %s)\n{\n  using T = TestType;\n%s}\n' % (types, body)
    if name == "doctest":
        return 'TEST_CASE_TEMPLATE("case", T, %s)\n{\n%s}\n' % (types, body)
    if name == "gtest":
        return ('using Distinct = ::testing::Types<%s>;\ntemplate<typename T> struct S : ::testing::Test {};\n'
                'TYPED_TEST_SUITE(S, Distinct);\nTYPED_TEST(S, Case)\n{\n  using T = TypeParam;\n%s}\n' % (types, body))
    if name == "boost":
        return ('using Distinct = std::tuple<%s>;\nBOOST_AUTO_TEST_CASE_TEMPLATE(case_0, T, Distinct)\n{\n%s}\n'
                % (types, body))
    values = ", ".join("num<%d>(0)" % k for k in range(n))
    return ('static suite<"s"> s = [] {\n  "case"_test = []<class T>(T) {\n%s  } | std::tuple<%s>{%s};\n};\n'
            % (body, types, values))


def write(path, text):
    path.write_text(text)  # NOSONAR - a path the build gives


def counts(text):
    return [int(n) for n in text.replace(";", ",").split(",") if n]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("output")
    ap.add_argument("--simple", type=counts, default="1,50,200")
    ap.add_argument("--template", type=counts, default="1,25,100")
    ap.add_argument("--types", type=counts, default="1,25,100")
    args = ap.parse_args()

    out = pathlib.Path(args.output)
    out.mkdir(parents=True, exist_ok=True)  # NOSONAR - a path the build gives
    for stale in out.glob("*.cpp"):
        stale.unlink()  # NOSONAR - a path the build gives
    for name, lib in LIBRARIES.items():
        write(out / ("%s-include-0.cpp" % name), lib["head"] + lib.get("tail", ""))
        for form, sizes in (("simple", args.simple), ("template", args.template)):
            for n in sizes:
                body = "".join(lib[form].format(i=i) for i in range(n))
                write(out / ("%s-%s-%d.cpp" % (name, form, n)), lib["head"] + body + lib.get("tail", ""))
        for n in args.types:
            for form, body in (("distinct", distinct(name, n)), ("onetemplate", one_template(name, n))):
                write(out / ("%s-%s-%d.cpp" % (name, form, n)),
                      lib["head"] + DISTINCT_TYPE + body + lib.get("tail", ""))


if __name__ == "__main__":
    main()
