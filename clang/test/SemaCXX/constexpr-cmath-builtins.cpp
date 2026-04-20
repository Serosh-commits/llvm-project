// RUN: %clang_cc1 -std=c++26 -verify %s
// RUN: %clang_cc1 -std=c++26 -verify %s -fexperimental-new-constant-interpreter

namespace std {
  constexpr double remainder(double x, double y) {
    return __builtin_remainder(x, y);
  }
  constexpr double fmod(double x, double y) {
    return __builtin_fmod(x, y);
  }
  constexpr double ceil(double x) {
    return __builtin_ceil(x);
  }
  constexpr double floor(double x) {
    return __builtin_floor(x);
  }
  constexpr double round(double x) {
    return __builtin_round(x);
  }
  constexpr double roundeven(double x) {
    return __builtin_roundeven(x);
  }
  constexpr double trunc(double x) {
    return __builtin_trunc(x);
  }
  constexpr int ilogb(double x) {
    return __builtin_ilogb(x);
  }
  constexpr double logb(double x) {
    return __builtin_logb(x);
  }
  constexpr double scalbn(double x, int n) {
    return __builtin_scalbn(x, n);
  }
  constexpr double ldexp(double x, int n) {
    return __builtin_ldexp(x, n);
  }
  constexpr double nextafter(double x, double y) {
    return __builtin_nextafter(x, y);
  }
}

// Remainder
static_assert(std::remainder(10.0, 3.0) == 1.0);
static_assert(std::remainder(10.0, -3.0) == 1.0);
static_assert(std::remainder(-10.0, 3.0) == -1.0);
static_assert(std::remainder(7.0, 2.0) == -1.0); // 7 - 2*4 = -1

// Fmod
static_assert(std::fmod(10.0, 3.0) == 1.0);
static_assert(std::fmod(10.0, -3.0) == 1.0);
static_assert(std::fmod(-10.0, 3.0) == -1.0);

// Rounding
static_assert(std::ceil(2.1) == 3.0);
static_assert(std::ceil(-2.1) == -2.0);
static_assert(std::floor(2.9) == 2.0);
static_assert(std::floor(-2.9) == -3.0);
static_assert(std::round(2.5) == 3.0);
static_assert(std::round(2.4) == 2.0);
static_assert(std::round(-2.5) == -3.0);
static_assert(std::roundeven(2.5) == 2.0);
static_assert(std::roundeven(3.5) == 4.0);
static_assert(std::trunc(2.9) == 2.0);
static_assert(std::trunc(-2.9) == -2.0);

// ILogb / Logb
static_assert(std::ilogb(1024.0) == 10);
static_assert(std::logb(1024.0) == 10.0);
static_assert(std::logb(0.5) == -1.0);

// Scalbn / Ldexp
static_assert(std::scalbn(1.0, 10) == 1024.0);
static_assert(std::ldexp(1.0, 10) == 1024.0);
static_assert(std::scalbn(1024.0, -10) == 1.0);

// Nextafter
static_assert(std::nextafter(1.0, 2.0) > 1.0);
static_assert(std::nextafter(1.0, 0.0) < 1.0);
static_assert(std::nextafter(1.0, 1.0) == 1.0);
static_assert(__builtin_copysign(1.0, -0.0) == -1.0);
static_assert(__builtin_copysign(1.0, 0.0) == 1.0);

// Rounding Batch 2
static_assert(__builtin_rint(2.5) == 2.0);
static_assert(__builtin_nearbyint(2.5) == 2.0);
static_assert(__builtin_lround(2.5) == 3);
static_assert(__builtin_llround(2.5) == 3);
static_assert(__builtin_lrint(2.5) == 2);

// Fdim / Fma
static_assert(__builtin_fdim(10.0, 5.0) == 5.0);
static_assert(__builtin_fdim(5.0, 10.0) == 0.0);
static_assert(__builtin_fma(2.0, 3.0, 4.0) == 10.0);

// Nan
static_assert(__builtin_isnan(__builtin_nan("")));

// Multi-output: modf
constexpr double test_modf() {
  double iptr = 0.0;
  double fpart = __builtin_modf(3.14159, &iptr);
  if (iptr != 3.0) return 0.0;
  if (fpart < 0.1415 || fpart > 0.1416) return 0.0;
  return 1.0;
}
static_assert(test_modf() == 1.0);

// Multi-output: frexp
constexpr double test_frexp() {
  int exp = 0;
  double mant = __builtin_frexp(6.0, &exp); // 6 = 0.75 * 2^3
  if (exp != 3) return 0.0;
  if (mant != 0.75) return 0.0;
  return 1.0;
}
static_assert(test_frexp() == 1.0);

// Multi-output: remquo
constexpr double test_remquo() {
  int quo = 0;
  double rem = __builtin_remquo(10.0, 3.0, &quo); // 10 = 3*3 + 1
  if (rem != 1.0) return 0.0;
  if ((quo & 0x7) != 3) return 0.0; // Standard guarantees at least 3 bits
  return 1.0;
}
static_assert(test_remquo() == 1.0);

// Complex builtins
constexpr _Complex double cd = 1.0 + 2.0i;
static_assert(__builtin_creal(cd) == 1.0);
static_assert(__builtin_cimag(cd) == 2.0);

constexpr _Complex double conj_cd = __builtin_conj(cd);
static_assert(__builtin_creal(conj_cd) == 1.0);
static_assert(__builtin_cimag(conj_cd) == -2.0);

static_assert(__builtin_creal(__builtin_cproj(1.0 + 2.0i)) == 1.0);
static_assert(__builtin_cimag(__builtin_cproj(1.0 + 2.0i)) == 2.0);
static_assert(__builtin_creal(__builtin_cproj(__builtin_inf() + 1.0i)) == __builtin_inf());
static_assert(__builtin_cimag(__builtin_cproj(__builtin_inf() + 1.0i)) == 0.0);
static_assert(__builtin_cimag(__builtin_cproj(1.0 - __builtin_inf() * 1.0i)) == 0.0);
static_assert(1.0 / __builtin_cimag(__builtin_cproj(1.0 - __builtin_inf() * 1.0i)) == __builtin_inf());

constexpr _Complex double inf_cd = __builtin_cproj(__builtin_inf() + 2.0i);
static_assert(__builtin_creal(inf_cd) == __builtin_inf());
static_assert(__builtin_cimag(inf_cd) == 0.0);

constexpr _Complex double inf_imag_cd = __builtin_cproj(1.0 + __builtin_inf() * 1.0i);
static_assert(__builtin_creal(inf_imag_cd) == __builtin_inf());
static_assert(__builtin_cimag(inf_imag_cd) == 0.0);

// Error cases
constexpr double nan_mod = std::fmod(1.0, 0.0); // expected-error {{constexpr variable 'nan_mod' must be initialized by a constant expression}} \
                                               // expected-note {{floating point arithmetic produced a NaN}}
constexpr double nan_rem = std::remainder(1.0, 0.0); // expected-error {{constexpr variable 'nan_rem' must be initialized by a constant expression}} \
                                                    // expected-note {{floating point arithmetic produced a NaN}}

// check f16 and f128
#ifdef __FLOAT128__
constexpr __float128 f128_val = __builtin_remainderf128(10.0q, 3.0q);
static_assert(f128_val == 1.0q);
#endif

// expected-no-diagnostics
