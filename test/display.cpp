//==================================================================================================
/**
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <vector>
#include <list>

using namespace std::literals;

TTS_CASE( "Check display of integral types" )
{
  TTS_EQUAL(tts::as_string( true  ), "true"s);
  TTS_EQUAL(tts::as_string( false ), "false"s);

  TTS_EQUAL(tts::as_string( std::int8_t {-42} ), "-42"s);
  TTS_EQUAL(tts::as_string( std::int16_t{-42} ), "-42"s);
  TTS_EQUAL(tts::as_string( std::int32_t{-42} ), "-42"s);
  TTS_EQUAL(tts::as_string( std::int64_t{-42} ), "-42"s);

  TTS_EQUAL(tts::as_string( std::uint8_t {69} ), "69"s);
  TTS_EQUAL(tts::as_string( std::uint16_t{69} ), "69"s);
  TTS_EQUAL(tts::as_string( std::uint32_t{69} ), "69"s);
  TTS_EQUAL(tts::as_string( std::uint64_t{69} ), "69"s);
};

TTS_CASE( "Check display of string types" )
{
  TTS_EQUAL(tts::as_string( "some string"s        ) , "some string"s           );
  TTS_EQUAL(tts::as_string( "some string_view"sv  ) , "some string_view"s      );
};

TTS_CASE( "Check display of pointer types" )
{
  TTS_EQUAL(tts::as_string( nullptr )     , "nullptr"s   );

#if defined(_MSC_VER)
  TTS_EQUAL(tts::as_string( (float*)(0) ) , "float *(0000000000000000)"s );
  TTS_EQUAL(tts::as_string( (void*)(0) )  , "void *(0000000000000000)"s  );
  TTS_EQUAL(tts::as_string( (char*)(0) )  , "char*(0000000000000000)"s  );
#elif defined(__clang__)
  TTS_EQUAL(tts::as_string( (float*)(0) ) , "float *(0)"s );
  TTS_EQUAL(tts::as_string( (void*)(0) )  , "void *(0)"s  );
  TTS_EQUAL(tts::as_string( (char*)(0) )  , "char*(0)"s  );
#elif defined(__GNUC__)
  TTS_EQUAL(tts::as_string( (float*)(0) ) , "float*(0)"s );
  TTS_EQUAL(tts::as_string( (void*)(0)  ) , "void*(0)"s  );
  TTS_EQUAL(tts::as_string( (char*)(0)  ) , "char*(0)"s  );
#endif
};

namespace space
{
  struct some_type { int i; };
  struct some_other_type { int i; };

  std::ostream& operator<<(std::ostream& os, some_other_type const& s)
  {
    return os << "[[" << s.i << "]]";
  }

  std::string to_string( some_type const& s ) { return "some_type[" + tts::as_string(s.i) + "]"; }
}

TTS_CASE( "Check display of type with specific to_string" )
{
  TTS_EQUAL(tts::as_string( space::some_type{42} )      , "some_type[42]"s );
  TTS_EQUAL(tts::as_string( space::some_other_type{63} ), "[[63]]"s        );
};

TTS_CASE( "Check display of sequence type" )
{
  TTS_EQUAL ( tts::as_string( std::list<int>{1,2,3,4} ), "{ 1 2 3 4 }"s);
  TTS_EQUAL ( tts::as_string( std::vector<space::some_type> { space::some_type{13}
                                                            , space::some_type{34}
                                                            }
                            )
            , "{ some_type[13] some_type[34] }"s
            );
};
