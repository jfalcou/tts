#error DO NOT INCLUDE - DOCUMENTATION PURPOSE ONLY

//==================================================================================================
/**
  @page  rationale  Rationale

  @tableofcontents

  @section ulp Precision testing

  > Are these two floating computations results similar enough?

  This is maybe the most difficult question to answer when implementing and validating
  numerical algorithms. Various methods are often found in existing testing frameworks
  or are used by developers. But if floating-point arithmetic can be tricky, floating-point
  comparisons are even trickier.

  In the rest of this section, we take for granted that the basic notions of floating-point
  computations and their related problems are known by the reader. If not, we strongly recommend
  having a deep look at [Goldberg's paper on the subject](http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html)
  or a [simplified version](http://floating-point-gui.de/).

  @subsection ulp-metric Precision metric

  The first thing people learn (and often they learn it the hard way) is that strict
  equality for floating points numbers is often meaningless or very hard to achieve.
  Once this state of fact is integrated, people often go to use a simple absolute difference
  with an arbitrary threshold. If this method looks sound, it's easy to fold and may lead to
  false positives. The proper way to compare non-zero or non-invalid floating-point numbers
  is to use the **Unit in the Last Place** metric.

  Let us define ε -- the machine epsilon -- as being the smallest positive floating-point number such that 1+ε
  is different from 1. Usually, ε is equal to \f$2^-52\f$ for double precision numbers and \f$2^-23\f$ for single
  precision numbers. In fact, 1+ε and 1 only differ by one bit in the least significant digit: the unit in the last
  place (ULP). For IEEE754 numbers, the ULP between a floating-point number \f$x\f$ and its immediate successor is
  \f$2^E \times \epsilon\f$ where E is the exponent of x.

  @subsection ulp-computing Computing ULP
  The ULP distance (or `ulpdist`) is a way to compare floating-point numbers by estimating the number
  of significant bits between their respective representations. The IEEE 754 specification -- followed by most
  modern floating-point hardware -- requires that the result of an elementary arithmetic operation (addition,
  subtraction, multiplication, division, and square root) must be within 0.5 ULP of the mathematically exact result.
  Achieving 0.5-1 ULP for computationally complex functions like transcendental functions is what a proper numerical
  library should aim for.

  The full algorithm can be expressed in standard C++ in the following way:

  @code
  template<class T> double ulpdist(T a0, T a1)
  {
    // Exit if a0 and a1 are equal or both NaN
    if( (a0 == a1) || ((a0!=a0) && (a1!=a1)) )
      return 0.;

    int e1,e2;
    T   m1,m2;
    m1 = std::frexp(a0, &e1);
    m2 = std::frexp(a1, &e2);

    // Compute the largest exponent between arguments
    int expo = std::max(e1, e2);

    // Properly normalize the two numbers by the same factor in a way
    // that the largest of the two numbers exponents will be brought to zero
    T n1 = std::ldexp(a0, -expo);
    T n2 = std::ldexp(a1, -expo);

    // Compute the absolute difference of the normalized numbers
    T e = (e1 == e2) ? std::abs(m1-m2) : std::abs(n1-n2);

    // Return the distance in ULP by diving this difference by the machine epsilon
    return double(e/std::numeric_limits<T>::epsilon());
  }
  @endcode

  Put in another way, one can estimate the `ulpdist` between two floating point numbers as the number of
  representable floating points values between them. This estimation leads to the following properties:

  \f$
  \begin{align}
  ulpdist(N,N \times ε) & = 0.5 \\
  ulpdist(N,N+N \times \frac{ε}{2}) & = 0 \\
  \end{align}
  \f$

  Note that if a `double` is compared to the double representation of its single precision
  conversion (they are exceptions as for fully representable reals), their `ulpdist` will be around
  \f$2^{26.5}\f$ (or \f$10^8\f$). For example: `ulpdist( double(float(M_PI)), double(M_PI) ) == 9.84293e+07`

  @section ulp-testing Testing ULPs

  What to do then when writing an unit test that handles floating points number ?
  You basically have three cases :

    * The value you compare must be equal by design. In this case, use [**`TTS_EQUAL`**](reference.html#tts_equal)
      to clearly state your intent. One such case can be for example functions that construct a floating point value bitwise.
    * The value you compare are the results of an undetermined number of other
      floating point operations. In this case, use [**`TTS_ULP_EQUAL`**](reference.html#tts_ulp_equal)
      and try to estimate the maximum amount of ULP your implementation should give. This can be either
      done by a strict analysis of the function behavior or by some guess work.
    * The value you compare are the results of an undetermined number of other floating point operations
      but stands in a predictable absolute range of error independent of the architecture and magnitude
      of the input. In this case, use [**`TTS_RELATIVE_EQUAL`**](reference.html#tts_relative_equal).

  Take extreme care to not overestimate the value of ULP measures. Some classical algorithms
  may ends up with hundreds of ULPs but still be meaningful.

**/
//==================================================================================================
