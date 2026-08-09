//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION capture_flag_main
#include <tts/tts.hpp>
#include <array>
#include <string_view>

TTS_CASE("Dummy test so the driver has something to run")
{
  TTS_EXPECT(1 == 1);
};

int main(int argc, char const** argv)
{
  ::tts::initialize(argc, argv);

  bool ok = true;

  // A valid --capture path writes the run's output to that file instead of stdout.
  {
    std::array<char const*, 2> capture_argv {argv[ 0 ], "--capture=tts_capture_flag_test.txt"};
    ::tts::_::current_arguments = ::tts::options {2, capture_argv.data()};

    int rc                      = capture_flag_main(argc, argv);
    ok                          = ok && (rc == 0);

    FILE* f                     = fopen("tts_capture_flag_test.txt", "r");
    ok                          = ok && (f != nullptr);
    if(f)
    {
      char buffer[ 4096 ] = {};
      auto n              = fread(buffer, 1, sizeof(buffer) - 1, f);
      fclose(f);

      std::string_view captured {buffer, n};
      ok = ok && (captured.find("Dummy test so the driver has something to run") !=
                  std::string_view::npos);
    }
    remove("tts_capture_flag_test.txt");
  }

  // An unwritable --capture path fails fast, before the suite is run.
  {
    std::array<char const*, 2> bad_argv {argv[ 0 ],
                                         "--capture=/definitely/not/a/real/directory/x.txt"};
    ::tts::_::current_arguments = ::tts::options {2, bad_argv.data()};

    auto test_count_before      = ::tts::global_runtime.test_count;
    int  rc                     = capture_flag_main(argc, argv);
    ok = ok && (rc == 1) && (::tts::global_runtime.test_count == test_count_before);
  }

  return ok ? 0 : 1;
}
