//==================================================================================================
/*!
  @file

  Defines functions to retrieve options through command line or environment variables

  @copyright 2018 Joel FALCOU


  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

**/
//==================================================================================================
#ifndef TTS_COMMON_ARGS_HPP_INCLUDED
#define TTS_COMMON_ARGS_HPP_INCLUDED

#include <unordered_map>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>

namespace tts { namespace detail
{
  struct args_map
  {
    args_map()
    {
      // List of environment variable to map onto options
      std::pair<std::string,std::string> envvars[] =  { {"TTS_COMPACT" , "compact"}
                                                      , {"TTS_FAIL_ONLY", "fail-only"}
                                                      };

      // fill map with environment variable
      for(auto const& id : envvars)
      {
        auto p = get_env(id.first);
        if(!p.empty()) data_[id.second].push_back(p);
      }
    }

    void update(int argc, const char** argv) const
    {
      bool found = false;
      std::string id;

      for(int i=1;i<argc;++i)
      {
        // look for --name value pattern
        std::string cur{argv[i]};

        if(is_option(cur))
        {
          found = true;
          id    = std::string{cur.begin()+2,cur.end()}; // Remove the '--'
        }
        else
        {
          // if an option was found and we're looking at another impromptu option, update the map
          if(found && !is_option(cur))
          {
            data_[id].push_back(cur);
          }
        }
      }
    }

    template<typename R> R operator()(std::string const& id, R def = R{} ) const
    {
      auto opt = data_.find(id);
      if(opt != data_.cend())
      {
        std::istringstream s(opt->second[0]);
        s >> def;
      }

      return def;
    }

    std::string operator()(std::string const& id, std::string def = "") const
    {
      auto opt = data_.find(id);
      if(opt != data_.cend())
      {
        def = data_[id][0];
      }

      return def;
    }

    std::vector<std::string> operator()(std::string const& id, std::vector<std::string> def = {}) const
    {
      auto opt = data_.find(id);
      if(opt != data_.cend())
      {
        def = data_[id];
      }

      return def;
    }

    static bool is_option(std::string const& s)
    {
      return (s.size() > 2) && (s[0] == '-') && (s[1] == '-');
    }

    // warning-free getenv on MSVC
    static std::string get_env(std::string const& name)
    {
#if defined(BOOST_MSVC)
      char* buf = 0;
      std::size_t sz = 0;

      _dupenv_s(&buf, &sz, name.c_str());

      std::string that = buf ? buf : " ";
      free(buf);
#else
      auto p = std::getenv(name.c_str());
      std::string that = p ? p : "";
#endif

      return that;
    }

    private:
    mutable std::unordered_map<std::string,std::vector<std::string>> data_;
  };
} }

namespace tts
{
  /*!
    @ingroup group-common

    Centralized options provider

    Provides a global access to all the command line and environment options.
  **/
  inline const detail::args_map args;
}

#endif
