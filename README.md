# Artic

A replacement for [Impala](https://github.com/AnyDSL/impala).

## Building

A compiler that supports C++17 and CMake are required to build the project.
Additionally, this project depends on [Thorin](https://github.com/AnyDSL/thorin).
Use the following commands to build the program:

    mkdir build
    cd build
    cmake .. -DThorin_DIR=<path/to/thorin>
    make -j

Note that `path/to/thorin` is usually `thorin/build/share/anydsl/cmake`.

## Status

Artic is in pre-alpha stage, which means that the basic functionality is implemented and that
it can already be used to write small programs. However, some crucial functionality is missing,
such as device intrinsics and built-ins, and there might be a couple of bugs that need fixing.

## Testing

Once built, Artic can be run with the following command:

    bin/artic [files]

## Syntax

The syntax follows [that of Impala](https://anydsl.github.io/Impala.html), whenever possible.
On top of this, polymorphism is supported.
Some notable changes compared to the syntax of Impala are:

 - The type inference algorithm is now bidirectional type checking, which means
   that type information is propagated _locally_, not globally. This gives improved
   error messages and better support for advanced type system features, at the cost
   of slightly more type annotations:
```rust
let x = |i| i; // artic needs a type annotation on `i` or on `x`
x(1) // impala would see this as a constraint that `x` is a function on integers
```
 - Declarations can be annotated with attributes:
 ```rust
 // This function will be exported in the generated LLVM module.
 // Note that only functions of order 1 (functions that do not take
 // other functions as arguments) can be exported.
 #[export]
 fn foo() = 1
 ```
 - Non-refutable (always matching) patterns are allowed as function parameters:
```rust
fn foo(x: f32, (y: f32, z: f32)) -> ... { ... }
```
 - Functions can use the `=` sign instead of braces if their body is just an expression:
```rust
fn foo() -> i32 = 1
```
 - Functions have their return type deducted automatically if they do not use `return` and
   are not recursive:
```rust
fn foo() = 1
```
 - Structure patterns and expressions use the `=` sign instead of `:` to give values to their members
   (this is for consistency with the use of `:` for type annotations, structure _types_ are not affected):
```rust
let p = Pair { x = 1, y = 2 };
match p {
    Pair { x = x_, y = y_ } => x_
}
```
 - Structure patterns can now have the `...` symbol to indicate they do not capture everything:
```rust
fn foo(Pair { x = f, ... }) = f
```
 - Type annotations can be added on every expression:
```rust
let x : i32 = (1 : i32) : i32;
```
 - Literals types are inferred depending on their context:
```rust
let x : u8 = 1;  // `x` types as u8
let y = 1;       // `y` types as i32 (default when no annotation is present)
let z : f32 = 1; // `z` types as f32
```
 - Patterns are now compiled using decision trees. This means that the generated code will be
   more efficient, and also that error messages for the completeness of a pattern are now more
   accurate (less conservative). The following case expression is now legal:
```rust
// impala would complain that this match is missing a default case
match x {
    (true, (_, true)) => 1,
    (true, (_, false)) => 2,
    (_, (1, _)) => 3,
    (_, (_, false)) => 4,
    (_, (_, true)) => 5
}
```
 - The `@@` sign for call-site annotations has been replaced by `@`:
```rust
@@foo(1, x); // valid in impala, invalid with artic
@foo(1, x); // valid in artic, invalid in impala
```
