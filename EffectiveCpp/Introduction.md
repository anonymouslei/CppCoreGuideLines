# Introduction
- [Introduction](#introduction)
  - [Terminology](#terminology)
    - [Declaration (P17)](#declaration-p17)
    - [Definition](#definition)
    - [Initialization](#initialization)
    - [Copy Constructor and copy assignment operator](#copy-constructor-and-copy-assignment-operator)
    - [pass-by-value](#pass-by-value)
    - [STL](#stl)
    - [undefined behavior](#undefined-behavior)
  - [Naming Conventions](#naming-conventions)
  - [TR1 and Boost](#tr1-and-boost)

## Terminology
### Declaration (P17)
A declaration tells compilers about the name and type of something, but it omits certiain details.
There are declarations:
```cpp
extern int x; // object declaration

std::size_t numDigits(int number); // function declaration

class Widget; // class declaration
```
`size_t`, by the way, is just a typedef for some unsigned type that C++ uses when counting thinsg (e.g., the number of characters in a `char*`-based string, the number of elements in an STL container, etc.).
It is also the type taken by the `operator[]` functions in `vector, deque`, and `string`, a convention we'll follow when defining our own `operatorp[]` functions.

Each function's declaration reveals its signature, i.e., its parameter and return types. A function's signature is the same as its type. The official C++ definition of "signature" excludes the function's return type, but in this book, it's more useful to have the return type be considered part of the signature.

### Definition
A definition provides compilers with the details a declaration omits. For an object, The definition is where compilers set aside memory for the object. 
For a function or a function template, the difinition provides the code body.
For a class or a class template, the definition lists the members of the class or template:
```cpp
int x; // object definition

std::size_t numDigits(int number) // function definition.
{// this function returns the number of digits in its parameter.
    std::size_t digitsSoFar = 1;
    while((number /= 10) != 0) ++digitsSoFar;
    return digitsSoFar; 
}

class Widget { // class definition
public:
    Widget();
    ~Widget();
};

template<typename T> // template definition
class GraphNode {
public:
    GraphNode();
    ~GraphNode();
};
```
### Initialization
Initialization is the process of giving an object its first value. 
For objects of user-defined types, initialization is performed by constructors.
A default constructor is one that can be called without any arguments.
Such a constructor either has no parameters or has a default value for every parameter:
```cpp
class A {
public:
    A(); // default constructor
};

class B {
public:
    explicit B(int x=0, bool b=true); // default constructor; see below for info on "explicit"
};

class C {
public:
    explicit C(int x); // not a default constructor
}
```
The constructors for classes B and C are declared `explicit` here. 
Thar prevents them from being used to perform implicit type conversions, though they may still be used for explicit type conversions:
```cpp
void doSomething(B bObject); // a function taking an object of type B

B bOjb1; // an object of type B

doSomething(bOjb1); // fine, passes a B to doSomething

B bOjb2(28); // fine, creates a B from the int 28

doSomething(28); // error! doSomething takes a B, not an int, and there is no implicit conversion from int to B

doSomething(B(28)); // fine, uses the B constructor to explicitly convert (i.e., cast) the int to a B for this call.
```
**Rules:** Constructors declared `explicit` are usually preferable to non-`explicit` ones, because they prevent compilers from performing unexpected (often unitended) type conversions. 

### Copy Constructor and copy assignment operator
The copy constructor is used to initialize an object with a different object of the same type, 
and the copy assignment operator is used to copy the value from one object to another of the same type:
```cpp
class Widget {
public:
    Widget(); // default constructor
    Widget(const Widget& rhs); // copy constructor
    Widget& operator=(const Widget& rhs); // copy assignment operator
};

Widget w1; // invoke default constructor

Widget w2(w1); // invoke copy constructor

w1 = w2; // inovke copy assignment operator

// read carefully when you see what appears to be an assignment, because the "=" syntax can also be sued to call the copy constructor:
Widget w3 = w2; // invoke copy constructor!
```

The copy constructor is a particularly important function, because it defines how an object is passed by value.
For example, consider this:
```cpp
bool hasAcceptableQuality(Widget w);

Widget aWidget;

if(hasAcceptableQuality(aWidget)) ...
```
The parameter w is passed to `hasAcceptableQuality` by value, so in the call above, `aWidget` is copied into `w`.
The copying is done by `Widget`'s copy constructor.
### pass-by-value
Pass-by-value means "call the copy constructor."

**Rule:** it's generally a bad idea to pass user-defined types by value.
Pass-by-reference-to-const is typically a better choice.

### STL
The STL is the Standard Template Library, the part of C++'s standard library devoted to containers (e.g., `vector`, `list`, `set`, `map`, etc.),
iterators (e.g., `vector<int>::iterator, set<string>::iterator`, etc.),
algorithms (e.g., `for_each, find, sort`, etc.),
and related functionality.

### undefined behavior
```cpp
// example 1
int *p = 0; // p is a null pointer

std::cout << *p; // dereferencing a null pointer yields undefined behavior

// example 2
char name[] = "Darla"; // name is an array of size 6(don't forget the trailing null!)

char c = name[10]; // referring to an invalid array index yields undefined behavior
```

When referring to constructors and destructors in code comments, I sometimes use the abbreviations `ctor` and `dtor`.

## Naming Conventions
- `lhs` stands for "left-hand side"
- `rhs` stands for "right-hand side"

for member functions, the left-hand argument is represented by the `this` pointer, so sometiems I use the parameter name `rhs` by itself.

## TR1 and Boost
- TR1("Technical Report 1") is a specification for new functionality being added to C++'s standard library.
This functionality takes the form of new class and function templates for things like hash tables, reference-counting smart pointers, regular expressions, and more.
All TR1 components are in teh namespace `tr1` that's nested inside the namespace `std`
- Boost is an organization and a web site offering portable, peer-reviewed, open source C++ libraries.
Most TR1 functionality is based on work done at Boost, and until compiler vendors include TR1 in their C++ library distributions. 