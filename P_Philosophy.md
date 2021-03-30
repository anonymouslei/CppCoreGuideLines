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