//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <array>
#include <sstream>

TTS_CASE("Check that types can be tested for equality")
{
  TTS_TYPE_IS(std::add_pointer<float const>::type, float const*);
};

TTS_CASE("Check that expression types can be tested for equality")
{
  [[maybe_unused]] double d;

  TTS_EXPR_IS(&d + 5, double*);
  TTS_EXPR_IS(std::move(d), double&&);
  TTS_EXPR_IS(std::swap(d, d), void);
};

TTS_CASE("Check that expression types can be tested for compilation status")
{
  int x {}, y {};
  TTS_EXPECT_COMPILES(x, y, { x += y; });
  TTS_EXPECT_NOT_COMPILES(x, { x.foo(); });

  // Issue #62
  std::stringstream s;
  TTS_EXPECT_COMPILES(s, x, { s << x; });
};

TTS_CASE_TPL("Check interaction with templates", int, float, char, void*)
<typename Type>(::tts::type<Type>)
{
  TTS_TYPE_IS(std::add_const_t<Type>, Type const);
};

TTS_CASE_TPL("Check interaction with pre-made type lists", ::tts::arithmetic_types)
<typename Type>(::tts::type<Type>)
{
  TTS_TYPE_IS(std::add_const_t<Type>, Type const);
};

template<int N, typename Indexes = std::make_index_sequence<N>> struct sizes;

template<int N, std::size_t... I> struct sizes<N, std::index_sequence<I...>>
{
  using types_list = tts::types<std::array<std::byte, I + 1>...>;
};

TTS_CASE_TPL("Check interaction with type list generator", sizes<5>)
<typename Type>(tts::type<Type>)
{
  TTS_TYPE_IS(std::add_const_t<Type>, Type const);
};

struct A
{
};
struct B
{
};
struct X
{
};
struct Y
{
};
struct Z
{
};

TTS_CASE("Check cartesian product of type lists")
{
  using list1 = tts::types<A, B>;
  using list2 = tts::types<X, Y>;
  using list3 = tts::types<X, Y, Z>;

  TTS_TYPE_IS((tts::cartesian_product<list1, list2>::types_list),
              (tts::types<tts::types<A, X>, tts::types<A, Y>, tts::types<B, X>, tts::types<B, Y>>));

  TTS_TYPE_IS((tts::cartesian_product<list1, list3>::types_list),
              (tts::types<tts::types<A, X>,
                          tts::types<A, Y>,
                          tts::types<A, Z>,
                          tts::types<B, X>,
                          tts::types<B, Y>,
                          tts::types<B, Z>>));

  TTS_TYPE_IS((tts::cartesian_product<list3, list2>::types_list),
              (tts::types<tts::types<X, X>,
                          tts::types<X, Y>,
                          tts::types<Y, X>,
                          tts::types<Y, Y>,
                          tts::types<Z, X>,
                          tts::types<Z, Y>>));
};
