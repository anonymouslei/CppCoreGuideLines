## How do I deal with memory leaks?
If systematic application of these techniques is not possible in your environment, be sure to use a memory leak detector as part of your standard development procedure, or plug in a garbage collector
## Can I use new just as in Java?
Sort of, but don't do it blindly, if you do want it prefer to spell is as `make_unique` or `make_shared`, and there are often superior alternatives that are simpler and more robust than any of that. Consider:
The clumsy use of `new` for `z3` is unnecessary and slow compared with the idiomatic use of a local variable. You don't need to use new to create an object if you also `delete` that object in the same scope; auch an object should be a local variable.
## Should I use NULL or 0 or nullptr?
you should use `nullptr` as the null pointer value. the others still work for backward compatibility with older code.
A problem with both `NULL`
## what is the difference between new and malloc()?
`malloc()` is a function that takes a number (of bytes) as its argument;
It returns a `void*` pointing to *unitialized* storage. 

`new` is an oeprator that takes a type and a set of *initializers* for that type as its arguments;
it returns a pointer to an initialized object of its type.
The difference is most obvious when you want to allocate an object of a user-defined type with non-trivial initialization semantics.

```cpp
class Circle: public Shape {
public:
    Circle(Point c, int t);
};
class X {
public:
    X(); // default constructor
};

void f(int n) {
    void* p1 = malloc(40); // allocate 40 bytes
    int* p2 = new int[10]; // allocate 10 uninitialized ints
    int* p3 = new int(10); // allocate 1 int initialized to 0
    int* p4 = new int(); // allocate 1 int initialized to 0
    int* p4 = new int; // allocate 1 uninitialized int

    Circle* pc1 = new Circle(Point(0,0), 10); // allocate a Circle constructed

    Circle *pc2 = new Circle; // error no default constructor

    X* px1 = new X; // allocate a default constructed X
    X* px2 = new X(); // allocate a default constructed X
    X* px2 = new X[10]; // allocate 10 default constructed Xs
}
```

Often, a `vector` is a better alternative to a free-store-allocated array (e.g., consider exception safety).

