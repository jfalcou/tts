//==================================================================================================
/*!
  @file

  Defines the approximate testing related helpers

  @copyright 2018 Joel FALCOU


  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

**/
//==================================================================================================
#ifndef TTS_TESTS_IMPL_APPROX_HPP_INCLUDED
#define TTS_TESTS_IMPL_APPROX_HPP_INCLUDED

#include <tts/detail/rt_helpers.hpp>
#include <tts/detail/comparator.hpp>
#include <vector>
#include <string>

namespace tts
{
  // Provides the basic services to perform approximate distance computation and report
  template<typename Measure, typename Reference> struct approx_
  {
    approx_(Reference const& r, double u) : ref(r), diff(u), size_mismatch(false), max_diff(u) {}

    template<typename U> inline bool compare(U const& data) const
    {
      Measure m;
      size_mismatch = detail::size(ref) != detail::size(data);
      if(size_mismatch) return false;

      auto br = detail::begin(data);
      auto bi = detail::begin(ref);

      std::ptrdiff_t sz = static_cast<std::ptrdiff_t>(detail::size(data));

      for(std::ptrdiff_t idx=0;idx < sz; ++idx)
      {
        check( m(*br,*bi), *br, *bi, (sz>1 ? idx : -1) );
        br++;
        bi++;
      }

      return errors.size() == 0;
    }

    struct error
    {
      double          value;
      std::string     ref,data;
      std::ptrdiff_t  idx;
    };

    bool                      mismatched()  const { return size_mismatch; }
    double                    max()         const { return max_diff;      }
    std::vector<error> const& report()      const { return errors;        }

    private:

    template<typename U, typename X, typename Y>
    inline void check(U const&  u, X const& x, Y const& y, std::ptrdiff_t idx) const
    {
      using tts::detail::to_string;

      if( u  > diff )
      {
        errors.push_back( {u, to_string(x),to_string(y), idx} );
        max_diff = std::max<double>(max_diff,u);
      }
    }

    Reference                   ref;
    double                      diff;
    mutable bool                size_mismatch;
    mutable double              max_diff;
    mutable std::vector<error>  errors;
  };

  // Streaming approx_ builds the report
  template<typename Measure, typename Reference>
  std::ostream& operator<<( std::ostream& os, approx_<Measure,Reference> const& u )
  {
    using tts::detail::to_string;

    if(u.mismatched()) return os << "arguments with mismatched size.";

    std::ostringstream s,ls;

    // Stream all errors
    ls.precision(20);

    for(auto const& e : u.report())
    {
      (e.idx >= 0) ? ls << "  [" << e.idx << "]: " : ls << "  ";
      ls << to_string(e.ref) << " vs " << to_string(e.data);
      Measure::to_stream(ls,e.value);
    }

    // Stream max error
    s.precision(20);
    Measure::to_stream(s,u.max());

    return os << "{\n"  + ls.str() + "}\n with a maximal error of " + s.str();
  }

  namespace ext
  {
    template<typename T, typename Measure, typename Reference>
    struct equal<T,tts::approx_<Measure, Reference>>
    {
      inline bool operator()(T const& l, tts::approx_<Measure, Reference> const& r) const
      {
        return r.compare(l);
      }
    };
  }
}

#endif
