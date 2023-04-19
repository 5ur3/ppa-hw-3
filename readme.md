# Static analyzer to find inputs that fail assertions

### Target language
The target language is a modification of a whilelang with asserts and without actual while loops. We have also changed the syntax a bit. 

Example:

```
a = .;

a = a * 1 + 2;

if (a < 8) {
  assert (false);
} else {};
```

```.``` is expression that returns integer input from the console

### Solution overview
The solutions consists of two parts
1) Parser written in C++ with flex/bison
2) Analyzer written in python with PySMT and z3

Parser outputs every possible execution path, and analyzer examines them and determines whether any of the asserts can be failed with some input.

#### Details
For the example program above parser generates two execution paths in special format:
```
ASS a I_0 
ASS a PLUS MUL a 1 2 
COND NOT LESS a 8 
```
and
```
ASS a I_0 
ASS a PLUS MUL a 1 2 
COND LESS a 8 
ASSERT FALSE
```

The first execution path is for the case where if condition fails. THe second is for the case when it succeeds

The analyzer analyzes all execution paths and prints the result:
```
Assertion fails on:
I_0 := 5
```

For a more complex program with miltiple conditions and inputs analyzer may generate multiple sets of inputs that fail an assertion.