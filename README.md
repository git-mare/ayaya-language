# Ayaya Lang
A small programming language that I'm developing (for fun)
---

## Tests
- Run the code below to execute the [`test_operations.aya`](/tests/test_operations.aya) file.
- Can change the `aya` test file in `main.cpp`, on main function. Can change for any inside the tests `folder` and it should work.
```
g++ src/main.cpp src/lexer/Lexer.cpp src/parser/Parser.cpp src/interpreter/Interpreter.cpp -o ayaya_test
./ayaya_test
```
OR
- Via CMake
```
mkdir -p build
cd build
cmake ..
make
./ayaya_test
```


## Dictionary
- Comparison between popular programming languages and Ayaya Language

| Operation           | Others                   | Ayaya-lang                     |
|--------------------|-------------------------|--------------------------------|
| Print / Output      | `print(x)`              | `show(x)`                      |
| Conditional / If    | `if (x > y) { ... }`    | `wondering (x > y) { ... }`   |
