//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <string>
#include <vector>
#include <list>

using namespace std::literals;

TTS_CASE( "Check display of integral types" )
{
  TTS_EQUAL(tts::as_text( true  ), "true");
  TTS_EQUAL(tts::as_text( false ), "false");

  TTS_EQUAL(tts::as_text( std::int8_t {-42} ), "-42");
  TTS_EQUAL(tts::as_text( std::int16_t{-42} ), "-42");
  TTS_EQUAL(tts::as_text( std::int32_t{-42} ), "-42");
  TTS_EQUAL(tts::as_text( std::int64_t{-42} ), "-42");

  TTS_EQUAL(tts::as_text( std::uint8_t {69} ), "69");
  TTS_EQUAL(tts::as_text( std::uint16_t{69} ), "69");
  TTS_EQUAL(tts::as_text( std::uint32_t{69} ), "69");
  TTS_EQUAL(tts::as_text( std::uint64_t{69} ), "69");
};

TTS_CASE( "Check display of string types" )
{
  TTS_EQUAL(tts::as_text( "some string"s        ) , "some string"s          );
  TTS_EQUAL(tts::as_text( "some string_view"sv  ) , "some string_view"s     );
};

TTS_CASE( "Check display of pointer types" )
{
  TTS_EQUAL(tts::as_text( nullptr ), "nullptr"s  );

#if defined(__GNUC__) && !defined(__clang__)
  TTS_EQUAL(tts::as_text( (float*)(0) ).rfind("float*(", 0) , 0UL);
  TTS_EQUAL(tts::as_text( (void*)(0)  ).rfind("void*(" , 0) , 0UL);
  TTS_EQUAL(tts::as_text( (char*)(0)  ).rfind("char*(" , 0) , 0UL);
#else
  TTS_EQUAL(tts::as_text( (float*)(0) ).rfind("float *(", 0), 0UL);
  TTS_EQUAL(tts::as_text( (void*)(0)  ).rfind("void *(" , 0), 0UL);
  TTS_EQUAL(tts::as_text( (char*)(0)  ).rfind("char *(" , 0), 0UL);
#endif
};

namespace space
{
  struct some_type { int i; };

  tts::text as_text( some_type const& s )
  {
    return "some_type[" + tts::as_text(s.i) + "]";
  }

  struct some_other_type
  {
    int j;

    friend tts::text as_text( some_other_type const& s )
    {
      return "[[" + tts::as_text(s.j) + "]]";
    }
  };
}

TTS_CASE( "Check display of type with specific to_string" )
{
  TTS_EQUAL(tts::as_text( space::some_type{42} )      , "some_type[42]"s);
  TTS_EQUAL(tts::as_text( space::some_other_type{63} ), "[[63]]"s       );
};

TTS_CASE( "Check display of sequence type" )
{
  TTS_EQUAL ( tts::as_text( std::list<int>{1,2,3,4} ), "{ 1 2 3 4 }");
  TTS_EQUAL ( tts::as_text( std::vector<space::some_type> { space::some_type{3}
                                                            , space::some_type{34}
                                                            }
                            )
            , "{ some_type[13] some_type[34] }"s
            );
};

TTS_CASE( "Check display of std::optional" )
{
  TTS_EQUAL(tts::as_text( std::optional{42} )   , "optional<int>{42}"s);
  TTS_EQUAL(tts::as_text( std::optional<int>{} ), "optional<int>{}"s  );
};
