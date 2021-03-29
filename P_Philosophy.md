# P: Philosophy
- [P: Philosophy](#p-philosophy)
  - [P.1: Express ideas directly in code](#p1-express-ideas-directly-in-code)
    - [Enforcement: very hard in general.](#enforcement-very-hard-in-general)
  - [P.2: Write in ISO Standard C++](#p2-write-in-iso-standard-c)
    - [Note:](#note)
    - [Note:](#note-1)
    - [Note:](#note-2)
  - [P.3: Express intent](#p3-express-intent)

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
