//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>

template<typename T> struct array_of
{
  template<typename L> struct make;
  template<typename... Ls> struct make<tts::types<Ls...>>
  {
    using type = tts::types<std::array<Ls,10>...>;
  };

  using types_list = typename make<T>::type;
};

TTS_CASE_WITH ( "Check behavior of add on wide"
              , array_of<::tts::arithmetic_types>
              , ::tts::generate ( tts::value{37}
                                , tts::ramp{65}, tts::ramp{1,2}
                                , tts::reverse_ramp{10}, tts::reverse_ramp{100,2}
                                , tts::between{0,100}
                                , tts::randoms{-100.,+100.}
                                , tts::sample{ std::uniform_real_distribution{0.,10.} }
                                )
              )
<typename T>( T const& value
            , T const& rmp  , T const& rmps
            , T const& rrmp , T const& rrmps
            , T const& btw
            , T const& rng, T const& urng
            )
{
  T value_ref { 37, 37, 37, 37, 37, 37, 37, 37, 37, 37}
  , rmp_ref   { 65, 66, 67, 68, 69, 70, 71, 72, 73, 74}
  , rmps_ref  {  1,  3,  5,  7,  9, 11, 13, 15, 17, 19}
  , rrmp_ref  { 19, 18, 17, 16, 15, 14, 13, 12, 11, 10}
  , rrmps_ref {118,116,114,112,110,108,106,104,102,100};

  using v_t = typename T::value_type;
  auto w1   = static_cast<v_t>(0);
  auto w2   = static_cast<v_t>(100);
  auto step = (w2-w1)/9;

  T btw_ref;
  for(std::size_t i=0;i<btw.size();++i)
  {
    btw_ref[i] = std::min( static_cast<v_t>(w1 + i*step), w2);
  }

  TTS_ALL_EQUAL(value , value_ref );
  TTS_ALL_EQUAL(rmp   , rmp_ref   );
  TTS_ALL_EQUAL(rmps  , rmps_ref  );
  TTS_ALL_EQUAL(rrmp  , rrmp_ref  );
  TTS_ALL_EQUAL(rrmps , rrmps_ref );
  TTS_ALL_EQUAL(btw   , btw_ref   );
};
