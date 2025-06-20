//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <array>

template<typename T> struct array_of
{
  template<typename L> struct make;
  template<typename... Ls> struct make<tts::types<Ls...>>
  {
    using type = tts::types<std::array<Ls,9>...>;
  };

  using types_list = typename make<T>::type;
};


TTS_CASE_WITH ( "Check behavior for scalar types"
              , ::tts::arithmetic_types
              , ::tts::generate ( tts::value{37}
                                , tts::ramp{65}, tts::ramp{1,2}
                                , tts::reverse_ramp{10}, tts::reverse_ramp{100,2}
                                , tts::between{0,100}
                                , tts::randoms{0.,100.}
                                , tts::sample{ std::uniform_real_distribution{0.,10.} }
                                )
              )
<typename T>( T const& value
            , T const& rmp  , T const& rmps
            , T const& rrmp , T const& rrmps
            , T const& btw
            , auto const& rng, auto const& urng
            )
{
  T value_ref = 37, rmp_ref = 65, rmps_ref = 1, rrmp_ref = 10, rrmps_ref = 100;
  T btw_ref = 0;

  TTS_EXPR_IS(value , T const&      );
  TTS_EXPR_IS(rmp   , T const&      );
  TTS_EXPR_IS(rmps  , T const&      );
  TTS_EXPR_IS(rrmp  , T const&      );
  TTS_EXPR_IS(rrmps , T const&      );
  TTS_EXPR_IS(btw   , T const&      );
  TTS_EXPR_IS(rng   , T const&      );
  TTS_EXPR_IS(urng  , double const& );

  TTS_EQUAL(value , value_ref );
  TTS_EQUAL(rmp   , rmp_ref   );
  TTS_EQUAL(rmps  , rmps_ref  );
  TTS_EQUAL(rrmp  , rrmp_ref  );
  TTS_EQUAL(rrmps , rrmps_ref );
  TTS_EQUAL(btw   , btw_ref   );

  TTS_GREATER_EQUAL(rng, T(0));
  TTS_LESS_EQUAL(rng, T(100));

  TTS_GREATER_EQUAL(urng, 0.);
  TTS_LESS_EQUAL(urng, 10.);
};

TTS_CASE_WITH ( "Check behavior for non-scalar types"
              , array_of<::tts::arithmetic_types>
              , ::tts::generate ( tts::value{37}
                                , tts::ramp{65}, tts::ramp{1,2}
                                , tts::reverse_ramp{10}, tts::reverse_ramp{100,2}
                                , tts::between{0,128}
                                , tts::randoms{0.,100.}
                                , tts::sample{ std::uniform_real_distribution{0.,10.} }
                                )
              )
<typename T>( T const& value
            , T const& rmp  , T const& rmps
            , T const& rrmp , T const& rrmps
            , T const& btw
            , auto const& rng
            , auto const& urng
            )
{
  T value_ref { 37, 37, 37, 37, 37, 37, 37, 37, 37}
  , rmp_ref   { 65, 66, 67, 68, 69, 70, 71, 72, 73}
  , rmps_ref  {  1,  3,  5,  7,  9, 11, 13, 15, 17}
  , rrmp_ref  { 18, 17, 16, 15, 14, 13, 12, 11, 10}
  , rrmps_ref {116,114,112,110,108,106,104,102,100};

  using v_t = typename T::value_type;
  auto w1   = static_cast<v_t>(0);
  auto w2   = static_cast<v_t>(128);
  auto step = (w2-w1)/8;
  T btw_ref;
  for(std::size_t i=0;i<btw.size();++i)
  {
    btw_ref[i] = std::min( static_cast<v_t>(w1 + i*step), w2);
  }

  TTS_EXPR_IS(value , (std::array<v_t, 9> const&)    );
  TTS_EXPR_IS(rmp   , (std::array<v_t, 9> const&)    );
  TTS_EXPR_IS(rmps  , (std::array<v_t, 9> const&)    );
  TTS_EXPR_IS(rrmp  , (std::array<v_t, 9> const&)    );
  TTS_EXPR_IS(rrmps , (std::array<v_t, 9> const&)    );
  TTS_EXPR_IS(btw   , (std::array<v_t, 9> const&)    );
  TTS_EXPR_IS(rng   , (std::array<v_t, 9> const&)    );
  TTS_EXPR_IS(urng  , (std::array<double, 9> const&) );

  TTS_ALL_EQUAL(value , value_ref );
  TTS_ALL_EQUAL(rmp   , rmp_ref   );
  TTS_ALL_EQUAL(rmps  , rmps_ref  );
  TTS_ALL_EQUAL(rrmp  , rrmp_ref  );
  TTS_ALL_EQUAL(rrmps , rrmps_ref );
  TTS_ALL_EQUAL(btw   , btw_ref   );

  for(auto e : rng)
  {
    std::cout << "e: " << +e << " ";
    TTS_GREATER_EQUAL(e, v_t(0));
    TTS_LESS_EQUAL(e, v_t(100));
  }
  std::cout << std::endl;

  for(auto e : urng)
  {
    TTS_GREATER_EQUAL(e, 0.);
    TTS_LESS_EQUAL(e, 10.);
  }
};
