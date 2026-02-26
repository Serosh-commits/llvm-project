// RUN: %clang_cc1 -fsyntax-only -verify %s

void f(int);

template <typename T>
void test_instantiation_dependence(T t) {
  (void)_Generic(t, int: 1, T: 2, default: 3);
}

void test_crash_case_propagation() {
  static int a = _Generic("", char (*)[f(x)]: 1, default: 2); // expected-error {{use of undeclared identifier 'x'}}
}

void test_error_in_expr() {
  (void)_Generic(1, int: f(x), default: 0); // expected-error {{use of undeclared identifier 'x'}}
}
