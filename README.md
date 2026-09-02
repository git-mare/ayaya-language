Aqui está o seu `README.md` com a seção do dicionário atualizada para incluir a nova sintaxe de declaração de variáveis (`omae wa`) e os valores booleanos que implementamos recentemente:

```markdown
# Ayaya Lang
A small programming language that I'm developing (for fun)
---

## Running
- Run the code below
```bash
g++ src/main.cpp src/lexer/Lexer.cpp src/parser/Parser.cpp src/interpreter/Interpreter.cpp -o aya

```

then

```bash
./aya ../tests/test_wondering.aya

```

OR

* Via CMake

```bash
mkdir -p build
cd build
cmake ..
make
./aya

```

* After compiling the files, you can run `./aya your_file.aya` and it should work.

## Dictionary

* Comparison between popular programming languages and Ayaya Language

| Operation | Others | Ayaya-lang |
| --- | --- | --- |
| Print / Output | `print(x)` | `show(x)` |
| Conditional / If | `if (x > y) { ... }` | `wondering (x > y) { ... }` |
| Variable Declaration | `var x = 10` / `let x = 10` | `omae wa x = 10` |
| Variable Assignment | `x = 20` | `x = 20` |
| Boolean Values | `True` / `False` | `true` / `false` |

## Plus Content

* I made a Notes file explaining how the process of creating a programming language works. You can check it [Here](https://www.google.com/search?q=/Notes.md)
