# Ayaya Lang
A small programming language that I'm developing (for fun)
---

## Running
- Run the code below
```
g++ src/main.cpp src/lexer/Lexer.cpp src/parser/Parser.cpp src/interpreter/Interpreter.cpp -o aya
```
then
```
./aya ../tests/test_wondering.aya
```
OR
- Via CMake
```
mkdir -p build
cd build
cmake ..
make
./aya
```

- After compiling the files, you can run `./aya your_file.aya` and it should work.


## Dictionary
- Comparison between popular programming languages and Ayaya Language

| Operation           | Others                   | Ayaya-lang                     |
|--------------------|-------------------------|--------------------------------|
| Print / Output      | `print(x)`              | `show(x)`                      |
| Conditional / If    | `if (x > y) { ... }`    | `wondering (x > y) { ... }`   |


## Plus Content
- I made a Notes file explaining how the process of creating a programming language works. You can check it [Here](/Notes.md)
