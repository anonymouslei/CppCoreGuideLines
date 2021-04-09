# Chapter 1. Accustoming yourself to C++
- [Chapter 1. Accustoming yourself to C++](#chapter-1-accustoming-yourself-to-c)
  - [Item 1: View C++ as a federation of languages](#item-1-view-c-as-a-federation-of-languages)
  - [Item 2: Prefer consts, enumS, and inlinesS to #defineS](#item-2-prefer-consts-enums-and-inliness-to-defines)
    - [case 1:](#case-1)
    - [case 2 (misuse): using `#define` to implement macros](#case-2-misuse-using-define-to-implement-macros)
    - [the enum hack](#the-enum-hack)

## Item 1: View C++ as a federation of languages
Exceptions required different approaches to structuring functions.
Templates gave rise to new ways of thinking about design, 
and the STL defined an approach to extensibility unlike any most people had ever seen.

**multiparadigm programming language** 多重范型编程语言,
C++ is one supporting a combination of procedural, object-oriented, functional, generic, and metaprogramming  features.

To make sense of C++, you have to recognize its primary sublanguages.
- C.
- object-oriented C++.
classes (including constructors and destructors), 
encapsulation, inheritance, polymorphism, virtual funcitons (dynamic binding), etc.
This is the part of C++ to which the classic rules for object-oriented design most directly apply.
- Template C++.
This is the generic programming part of C++.
- the STL.
The STL is a template library. its conventions regarding containers, iterators, algorithms, and function objects mesh beautifully.

## Item 2: Prefer consts, enumS, and inlinesS to #defineS
### case 1:
```cpp
#define ASPECT_RATIO 1.653 // the symbolic name ASPECT_RATIO may never be seen by compiler.

// solution:
const double AspectRatio = 1.653; // uppercase names are usually for macros, hence the name change
```
When replacing `#define` with constants, two special cases are worth mentioning. 
- defining constant pointers.
Because constant definitions are typically put in header files, it is important that the pointer be declared `const`,
usually in addition to what the pointer points to (write `const` twice).
```cpp
const char* const authorName = "Scott Meyers";
// string objects are generally preferable to their char*-based progenitors
const std::string authorName("Scott Meyers");
```
- class-specific constants.
To limit the scope of a constant to a class, you must make it a member, and to ensure there's at most one copy of the constant,
you must make it a static member
```cpp
class GamePlayer {
private:
    static const int NumTurns = 5; // constant declaration
    int scores[NumTurns]; // use of constant
};
```
Usually, C++ requires that you provide a definition for anything you use,
but class-specific constants that are static and of integral type are an exception.
As long as you don't take their address,
you can declare them and use them without providing a definition.
```cpp
const int GamePlayer::NumTurns;; // definition of NumTurns
```

In older compilers cases where the above syntax can't be used,
you put the initial value at the point of definition:
```cpp
class CostEstimate {
private:
    static const double FudgeFactor; // declaration of static class const; goes in header file
};

const double CostEstimate::FudgeFactor = 1.35; // definition of static class constant; goes in impl. file
```
### case 2 (misuse): using `#define` to implement macros
Macros that look like functions but that don't incur the overhead of a function call.
```cpp
// call f with the maximum of a and b
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a):(b)) 
```
Whenever you write this kind of macro, you have to remember to parenthesize all the arguments in the macro body.
Otherwise you can run into trouble when somebody calls the macro with an expression.
But even if you get that right, look at the weird things that can happen:
```cpp
int a = 5, b = 0;

CALL_WITH_MAX(++a, b); // a is incremented twice
CALL_WITH_MAX(++a, b+10); // a is incremented once
```
you can use a template for an inline function to replace using macro.
```cpp
template<typename T> 
inline void callWithMax(const T& a, const T& b) // because we don't know what T is, we pass by reference-to-const
{
  f(a > b ? a : b);
}
```
(P30)


### the enum hack
The values of an enumerated tye can be used where `int`s are expected.

The enum hack is worth knowing about for several reasons.
- the enum hack behaves in some ways more like a `#define` than a `const` does. 
For example. it's legal(合法的) to take the address of a `const`,
but it's not legal to take the address of an enum,
and it's typically nor legal to take the address of a `#define`, either.
If you don't want to let people get a pointer or reference to one of you rintegral constants,
an enum is a good way to enforce that constraint.
Though good compilers won't set aside storage for `const` objects of integral types (unless you create a pointer or reference to the object), sloppy compilers may, and you may not be willing to set aside memory for such objects.
Like `#define`, enums never result in that kind of unnecessary memory allocation
- the enum hack is a fundamental technique of template metaprogramming.
  



