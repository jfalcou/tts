//==================================================================================================
/*
  TTS - Tiny Test System
  Copyright : TTS Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#define TTS_MAIN
#include <tts/tts.hpp>
#include <cstdlib>
#include <new>

// A test counting its allocations replaces the throwing operator new and the plain deletes only
static int allocations = 0; // NOSONAR - the replaced operator new counts into it

void*      operator new(std::size_t n)
{
  ++allocations;
  if(void* p = std::malloc(n)) return p;
  throw std::bad_alloc();
}

void operator delete(void* p) noexcept
{
  std::free(p);
}

void operator delete(void* p, std::size_t) noexcept
{
  std::free(p);
}

TTS_CASE("Check that the replaced operator new is in force")
{
  // Called as functions, so that no optimiser elides the pair
  int   before = allocations;
  void* p      = ::operator new(sizeof(int));
  int   after  = allocations;
  ::operator delete(p);

  TTS_EQUAL(after, before + 1);
};

TTS_CASE("Check that tts::buffer stays out of the replaced operator new")
{
  int before = allocations;
  {
    tts::buffer<int> b(64);
    b.push_back(1);
  }
  int after = allocations;

  TTS_EQUAL(after, before);
};
