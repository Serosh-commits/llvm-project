// RUN: %clang_cc1 -verify -fexperimental-new-constant-interpreter %s
// RUN: %clang_cc1 -verify %s

// This test ensures that the constexpr math builtins do not crash the compiler
// when given invalid or malicious inputs such as null pointers or const objects.

constexpr double d_const = 1.0;

// modf tests
constexpr double test_modf_null() {
  double *p = nullptr;
  __builtin_modf(1.0, p); // expected-note {{read of null pointer}}
  return 0.0;
}
static_assert(test_modf_null()); // expected-error {{static assertion expression is not an integral constant expression}} \
                                  // expected-note {{in call to 'test_modf_null()'}}

constexpr double test_modf_const() {
  __builtin_modf(1.0, const_cast<double*>(&d_const)); // expected-note {{modification of object of const-qualified type 'const double' is not allowed}}
  return 0.0;
}
static_assert(test_modf_const()); // expected-error {{static assertion expression is not an integral constant expression}} \
                                   // expected-note {{in call to 'test_modf_const()'}}

// remquo tests
constexpr double test_remquo_null() {
  int *p = nullptr;
  __builtin_remquo(1.0, 1.0, p); // expected-note {{read of null pointer}}
  return 0.0;
}
static_assert(test_remquo_null()); // expected-error {{static assertion expression is not an integral constant expression}} \
                                    // expected-note {{in call to 'test_remquo_null()'}}

// Complex number tests
constexpr double test_creal_invalid() {
  double d = 1.0;
  return __builtin_creal(&d); // expected-note {{read of uninitialized object}}
}
static_assert(test_creal_invalid()); // expected-error {{static assertion expression is not an integral constant expression}} \
                                     // expected-note {{in call to 'test_creal_invalid()'}}

// Check for large quotients in remquo
constexpr double test_remquo_overflow() {
  int quo = 0;
  // Very large quotient that should trigger a diagnostic
  __builtin_remquo(1e60, 1.0, &quo); // expected-note {{floating point arithmetic produced a NaN}}
  return 0.0;
}
static_assert(test_remquo_overflow()); // expected-error {{static assertion expression is not an integral constant expression}} \
                                       // expected-note {{in call to 'test_remquo_overflow()'}}
