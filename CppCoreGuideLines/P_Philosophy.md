# P: Philosophy
- [P: Philosophy](#p-philosophy)
  - [P.1: Express ideas directly in code](#p1-express-ideas-directly-in-code)
    - [Enforcement: very hard in general.](#enforcement-very-hard-in-general)
  - [P.2: Write in ISO Standard C++](#p2-write-in-iso-standard-c)
    - [Note:](#note)
    - [Note:](#note-1)
    - [Note:](#note-2)
  - [P.3: Express intent](#p3-express-intent)
    - [example](#example)
  - [p.4: Ideally, a program should be statically type safe](#p4-ideally-a-program-should-be-statically-type-safe)
    - [Reason](#reason)
    - [Enforcement](#enforcement)
  - [P.5: Prefer compile-time checking to run-time checking](#p5-prefer-compile-time-checking-to-run-time-checking)
    - [Reason](#reason-1)
    - [Example](#example-1)
    - [Enforcement](#enforcement-1)
  - [P.6: What cannot be checked at compile time should be checkable at run time](#p6-what-cannot-be-checked-at-compile-time-should-be-checkable-at-run-time)
    - [example](#example-2)
    - [example](#example-3)
    - [NOTE:](#note-3)
  - [P.7: Catch run-time errors early](#p7-catch-run-time-errors-early)
    - [Example](#example-4)
    - [Enforcement](#enforcement-2)
  - [P.8: Don't leak any resources](#p8-dont-leak-any-resources)
    - [Reason](#reason-2)
    - [Example](#example-5)
    - [Note](#note-4)
    - [Enforcement](#enforcement-3)
  - [P.9: Don't waste time or space](#p9-dont-waste-time-or-space)
    - [Note](#note-5)
    - [Example](#example-6)
  - [P.10: Prefer immutable data to mutable data](#p10-prefer-immutable-data-to-mutable-data)
  - [P.11 Encapsulate messy constructs, rather than spreading through the code](#p11-encapsulate-messy-constructs-rather-than-spreading-through-the-code)
    - [reason](#reason-3)
    - [Example](#example-7)
    - [Note](#note-6)

## P.1: Express ideas directly in code
```cpp
class Date {
public:
    Month month() const; // do
    int month(); // don't

}
```
example, bad: this loop is a restricted form of `std::find:`
```cpp
void f(vector<string>& v)
{
  string val;
  cin >> val;
  int index = -1; // bad, plus should use gsl::index
  for (int i = 0; i < v.size(); ++i) {
    if (v[i] == val) {
      index = i;
      break;
    }
  }
}
```
example, good: a much clearer expression of intent would be:
```cpp
void f(vector<string>& v)
{
  string val;
  cin >> val;
  auto p = find(begin(v), end(v), val); //better
}
```
A C++ programmer should know the basics of the standard library, and use it where appropriate. 
```cpp
change_speed(double s); // bad: what does s signify?
change_speed(Speed s); // better: the meaning of s is specified

change_speed(2.3); // error: no unit
change_speed(23m / 10s); // meters per second
```
We could have accepted a plain (unit-less) `double` as a delta, but that would have been error-prone. If we wanted both absolute speed and deltas, we would have defined as `Delta` type.

### Enforcement: very hard in general.
- use `const` consistently (check if member fuctions modify their object; check if functions modify arguments passed by pointer or reference)
- flag uses of casts (casts neuter the type system)
- detect code that mimics the standard library
  
## P.2: Write in ISO Standard C++
### Note:
there are environments where extensions are necessary. e.g., to access system resources. In such cases, localize the use of necessary extensions and control their use with non-core Coding Guidelines. If possible, build interfaces that encapsulate the extensions so they can be turned off or compiled away on systems that do not support those extensions.
Extensions often do not have rigorously defined semantics. Even extensions that are common and implemented by multiple compilers might have slightly different behaviors and edge case behavior as a direct result of not having a rigorous standard definition. with sufficient use of any such extension, expected portability will be impacted.

### Note:
Using valid ISO C++ does not guarantee portability. Avoid dependence on undefined behavior and be aware of constructs with implementation defined meaning (e.g., `sizeof(int)`)

### Note:
there are environments where restrictions on use of standard C++ language or library features are necessary, e.g., to avoid dynamic memory allocation as required by aircraft control software standards. 

## P.3: Express intent
### example 
```cpp
draw_line(int, int, int, int); // obscure
draw_line(Point, Point); // clearer
```

## p.4: Ideally, a program should be statically type safe
### Reason 
Probelm areas:
- unions
- casts
- array decay
- range errors
- narrowing conversions

### Enforcement
Always suggest an alternative. For example:
- unions - use `variant`
- casts - minimize their use; templates can help
- array decay - use `span`
- range errors - use `span`
- narrowing conversions - minimize their use and use `narrow` or `narrow_cast` where they are necessary

## P.5: Prefer compile-time checking to run-time checking 

### Reason 
code clarity and performance.
You don't need to write error handlers for errors caught at compile time.

### Example
```cpp
// don't: avoidable code
int bits = 0;
for (Int i = 1; i; i<<=1)
  ++bits;
if (bits < 32)
  ceer << "Int too small\n";
```
This example fails to achieve what it is trying to achieve (because overflow is undefined).
```cpp
// replaced with a simple static_assert
static_assert(sizeof(Int)>= 4); // do: compile-time check
```
or better still just use the type system and replace `int` with `int32_t`.

```cpp
void read(int*p, int n); // read max n integers into *p
int a[100];
read(a, 1000); // bad, off the end
```
better
```cpp
void read(span<int> r); // read into the range of integers r

int a[100];
read(a); // better: let the compiler figure out the number of elements
```

### Enforcement
- look for pointer arguments
- look for run-time checks for range violations.
  
## P.6: What cannot be checked at compile time should be checkable at run time

we should endeavor to write programs that in principle can be checked, given sufficient resources (analysis programs, run-time checks, machine resources, time).

### example 
```cpp
// bad
// separately compiled, possible dynamically loaded
extern void f(int* p);
void g(int n)
{
  f(new int[n]); // bad: the number of elements is not passed to f()
}
```
the number of elements has been so thoroughly "obscured" that static analysis is probably rendered infeasible and dynamic checking can be very difficult when `f()` is part of an ABI so that we cannot "instrument" that pointer.

```cpp
extern void f2(int* p, int n);
void g2(int n)
{
  f2(new int[n], m); // bad: a wrong number of elements can be passed to f()
}
```
Passing the number of elements as an argument is better than just passing the pointer and relying on some (unstated) convention for knowing or discovering the number of elements.

```cpp
// the standard library resource management pointers fail to pass the size when they point to an object
// separately compiled, possible dynamically loaded
// this assumes the calling code is ABI-compatible, using a compatible C++ compiler and the same stdlib implementation
extern void f3(unique_ptr<int[]>, int n)

void g3(int n)
{
  f3(make_unique<int[]>(n), m); // bad: pass ownership and size separately
}
```

```cpp
// we need to pass the pointer and the number of elements as an integral object
extern void f4(vector<int>&); // separately compiled, possibly dynamically loaded
extern void f4(span<int>); // separately compiled, possibly dynamically loaded
void g3(int n)
{
  vector<int> v(n);
  f4(v); // pass a reference, retain ownership
  f4(span<int>{v}); // pass a view, retain ownership
}
```
This design carries the number of elements along as an integral part of an object, so that errors are unlikely and dynamic (run-time) checking is always feasible, if not always affordable.

### example
How do we transfer both ownership and all information needed for validating use?
```cpp
vector<int> f5(int n) // OK: move
{
  vector<int> v(n);
  // ... initialize v ...
  return v;
}

unique_ptr<int[]> f6(int n)  // bad: loses n
{
  auto p = make_unique<int[]>(n);
  // ... initialize *p
  return p;
}

owner<int*> f7(int n) // bad: loses n and we might forget to delete
{
  owner<int*> p = new int[n];
  // ... initialize *p ...
  return p;
}
```
### NOTE:
这个rule有点没看懂

## P.7: Catch run-time errors early
```cpp
void increment1(int* p, int n) // bad: error-prone
{
  for (int i = 0; i < n; ++i) ++p[i];
}

void use1(int m)
{
  const int n = 10;
  int a[n] = {};
  // ...
  increment1(a, m); // maybe typo, maybe m <= n is supposed
  // but assume that m == 20
}
```
here in `use1` that will lead to corrupted data or a crash.
The (pointer, count)-style interface leaves `increment1()` with no realistic way of defending itself against out-of-range errors.
if we could check subscripts for out of range access, then the error would not be discovered until `p[10]` was accessed. 
```cpp
void increment2(span<int> p)
{
  for (int& x: p) ++x;
}

void use2(int m)
{
  const int n = 10;
  int a[n] = {};
  // ...
  increment2({a, m}); // maybe typo, maybe m <= n is supposed
}
```
Now, `m <= n` can be checked at the point of call(early) rather than later. 
```cpp
void use3(int m)
{
  const int n = 10;
  int a[n] = {};
  increment2(a); // the number of elements of a need not be repeated
}
```

### Example
```cpp
// don't repeatedly check the same value.
// don't pass structured data as strings
Date read_date(istream& is); // read date from istrema

Date extract_date(const string& s); // extract date from string

void user1(const string& date) // manipulate date
{
  auto d = extract_date(date);
  // ...
}

void user2()
{
  Date d = read_date(cin);
  // ...
  user1(d.to_string());
  // ...
}
// the date is validated twice (by the `Date` constructor) and passed as a character string (unstructured date).
```

```cpp
// excess checking can be costly. 
// There are cases where checking early is inefficient because you might never need the value, or might only need part of the value that is more easily checked than the whole.
// Similarly, don't add validity checks that change the asymptotic behavior of your interface (e.g., don't add a O(n) check to an interface with an average complexity of O(1)).

class Jet { // Physics says: e * e < x * x + y * y + z * z
  float x;
  float y;
  float z;
  float e;
public:
  Jet(float x, float y, float z, float e)
  : x(x), y(y), z(z), e(e)
  {
    // sould I check here that the values are physically meaningful?
  }

  float m() const
  {
    // Sould I handle the degenerate case here?
    return sqrt(x * x + y * y + z * z - e * e);
  }

  // ...
}
```

### Enforcement
- Look at pointers and arrays: Do range-checking early and not repeatedly
- Look at conversions: Eliminate or mark narrowing conversions
- Look for unchecked values coming from input
- Look for structured data (objects of classes with invariants) being converted into strings 

## P.8: Don't leak any resources
### Reason
Even a slow growth in resources will, over time, exhaust the availability of those resources.
This is particularly important for long-running programs,
but is an essential piece of respnsible programming behavior.

### Example
```cpp
// bad
void f(char* name)
{
  FILE* input = fopen(name, "r");
  // ...
  if (something) return; // bad: if something == true, a file handle is leaked
}
```
```cpp
void f(char* name)
{
  ifstream input {name};
  // ...
  if (something) return; // OK: no leak
  // ...
}
```

see also: [The resource management section](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource)

### Note
A leak is colloquially "anything that isn't cleaned up".
The more important classification is "anything that can no longer be cleaned up".
For example, relying on system guaranteed cleanup such as file closing and memory deallocation upon process shutdown can simplify code.
However, relying on abstractions that *implicitly* clean up can be as simple, and often safer.

### Enforcement
- Look at pointers: Classify them into non-owners (the default) and owners.
Where feasible, replace owners with standard-library resource handles (as in teh example above).
Alternatively, mark an owner as such using `owner` from the GSL.
- look for naked `new` and `delete`.
- Look for known resource allocating functions returning raw pointers (such as `fopen`, `malloc` and `strdup`).

## P.9: Don't waste time or space
### Note
Time and space that you spend well to achieve a goal (e.g., speed of development, resource safety, or simplification of testing) is not wasted.
"Another benefit of striving for efficiency is that the process forces you to understand the problem in more depth" - Alex Stepanov

### Example 
```cpp
// bad
struct X {
  char ch;
  int i;
  string s;
  char ch2;
};

X waste(const char* p)
{
  if (!p) throw Nullptr_error{};
  int n = strlen(p);
  auto buf = new char[n];
  if (!buf) throw Allocation_error{};
  for (int i = 0; i < n; ++i) buf[i] = p[i];
  // .. manipulate buffer ...
  X x;
  x.ch = 'a';
  x.s = string(n); // give x.s space for *p
  for (gsl::index i = 0; i < x.size(); ++i) x.s[i] = buf[i]; // copy buf into x.s
  delete[] buf;
  return x;
}

void driver()
{
  X x = waste("Typical argument");
  // ...
}
```

```cpp
void lower(zstring s)
{
  for (int i = 0; i < strlen(s); ++i) s[i] = tolower(s[i]);
  // this expression will be evaluated on every iteration of the loop, which means that strlen must walk through string every loop to discover its length.
  // While the string contents are changing, it's assumed that `toLower` will not affect the length of the string.
  // so it's better to cache the length outside the loop and not incur that cost each iteration.
}
```

## P.10: Prefer immutable data to mutable data

## P.11 Encapsulate messy constructs, rather than spreading through the code
### reason
messy code is more likely to hide bugs and harder to write.
A good interface is easier and safer to use.
Messy, low-level code breeds more such code.

### Example
```cpp
// this is low-level, verbose, and error-prone. For example, we 'forgot' to test for memory exhaustion. 
int sz = 100;
int* p = (int*) malloc(sizeof(int) * sz);
int count = 0;
// ...
for(;;) {
  // ... read an int into x, exit loop if end of file is reached ...
  if (count == sz)
    p = (int*) realloc(p, sizeof(int) * sz * 2);
  p[count++] = x;
  // ...
}
```
```cpp
vector<int> v;
v.reserve(100);
// ...
for (int x; cin >> x;) {
  // .. check that x is valid ...
  x.push_back(x);
}
```
### Note
The standards library and the GSL are examples of this philosophy.
such as `vector`, `span`, `lock_guard`, and `future`.
We can and should design and implement more specialized libraries, rather than leaving the users(often ourselves) with the challenge of repreatedly getting low-level code well. 
