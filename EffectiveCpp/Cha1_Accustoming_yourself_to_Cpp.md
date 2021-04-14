# Chapter 1. Accustoming yourself to C++
- [Chapter 1. Accustoming yourself to C++](#chapter-1-accustoming-yourself-to-c)
  - [Item 1: View C++ as a federation of languages](#item-1-view-c-as-a-federation-of-languages)
  - [Item 2: Prefer consts, enumS, and inlinesS to #defineS](#item-2-prefer-consts-enums-and-inliness-to-defines)
    - [case 1:](#case-1)
    - [case 2 (misuse): using `#define` to implement macros](#case-2-misuse-using-define-to-implement-macros)
    - [the enum hack](#the-enum-hack)
  - [Item 3: Use `const` whenever possible](#item-3-use-const-whenever-possible)
    - [const Member Functions](#const-member-functions)

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
This template generates a whole family of functions,
each of which takes two objects of the same type and calls `f` with the greater of the two objects.
There is no need to parenthesize parameters inside the function body,
no need to worry about evaluating parameters multiple times, etc.
FUrthermore,
because `callWithMax` is a real function,
it obeys scope and access rules.
For example,
it makes perfect sense to talk about an inline function that is private to a class.
In general, there is just no way to do that with a macro.

Given the abailability of `const`s, `enum`s, and `inline`s, your need for the preprocessor is reduced, but it is not eliminated.

**Things to remember**
- For simple constants, prefer `const` objects or enums to `#define`s.
- For function-like macros, prefer inline functions to `#define`s.

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
  
## Item 3: Use `const` whenever possible
It allows you to communicate to both compilers and other programmers that a value should remain invariant.

The `const` keyword is remarkable versatile.
Outside of classes, you can use it for constants at global or namespace scope,
as well as for objects declared `static` at file, function, or block scope.
Inside classes, you can use it for both static and non-static data members.
For pointers, you can specify whether the pointer itself is `const`,
the data it points to is `const`, both, or neither
```cpp
char greeting[] = "Hello";
char *p = greeting; // non-const pointer, non-const data

const char *p = greeting; // non-const pointer, const data

char * const p = greeting; // const pointer, non-const data

const char * const p = greeting; // const pointer, const data
```
(memorize)This syntax isn't as capricious as it may seem.
If the word `const` appears to the left of the asterisk,
what's pointed to is constant;
if the word `const` appears to the right of the asterisk,
the pointer itself is constant;
it isn't allowed to point to something different,
but the thing it points to may be modified.
if `const` appears on both sides, both are constant.

When what's pointed to is constant, some programmers list `const` before the type.
Other list it after the type but before the asterisk.
```cpp
void f1(const Widget * pw); // f1 takes a pointer to a constant Widget object
void f2(Widget const *pw); // so does f2
```

STL iterators are modeled on pointers,
so an `iterator` acts much like a `T*` pointer.
```cpp
std::vector<int> vec;

const std::vector<int>::iterator iter = vec.begin(); // iter acts like a T* const
*iter = 10; // OK, changes what iter points to 
++iter; // error! iter is const

std::vector<int>::const_iterator cIter = vec.begin(); // cIter acts like a const T*
*cIter = 10; // error! *cIter is const
++cIter; // fine, changes cIter
```

Some of the most powerful uses of `const` stem from its application to function declarations.
Having a function return a constant value often makes it possible re reduce the incidence of client errors without giving up safety or efficiency.
```cpp
class Rational {...};

const Rational operator*(const Rational& lhs, const Rational& rhs);
```
Many programmers squint when they first see this.
Why should the result of `operator*` be a `const` object?
Because if it weren't,
clients would be able to commit atrocities like this:
```cpp
Rational a, b, c;

(a * b) = c; // invoke operator= on the result of a*b
if (a * b = c) ... // oops, meant to do a comparison!
```
Declaring `operator*`'s return value `const` prevents *(allowing assignments to the product of two numbers),
and that's why it's The Right Thing TO Do.

you should use both whenever you can.
it can save you from annoying errors such as the "I meant to type '==' but I accidently typed '='" mistake we just saw.

### const Member Functions
Such member functions are important for two reasons.
First, they make the interface of a class easier to understand.
It is important to know which functions may modify an object and which may not.
Second, they make it possible to work with `const` objects.

```cpp
class TextBlock {
public:
  const char& operator[](std::size_t position) const // operator[] for const objects
  {return text[position];}

  char& operator[](std::size_t position) // operator[] for non-const objects
  { return text[position];} // non-const objects

private:
  std::string text;
};

// TextBlock's operator[]s can be used like this:
TextBlock tb("Hello");
std::cout << tb[0]; // calls non-const TextBlock::operator[]

const TextBlock ctb("World");
std::cout << ctb[0]; // calls const TextBlock::operator[]
```

