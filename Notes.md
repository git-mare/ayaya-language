# Step-by-Step Conceptual Guide

I made a guide that describes, conceptually, the main steps to create your own programming language, from processing text to executing code.

(Well, i will probably abandon this project, if i do so and later decide to come back someday, this guide will be useful for me LOL).

## 1. Lexical Analyzer (Lexer)

The **lexer** is the first stage of a programming language.  
It takes the source code as input and converts it into **tokens**, which are meaningful units such as:

- Numbers (`123`, `3.14`)
- Identifiers (variable or function names)
- Operators (`+`, `-`, `*`, `/`)
- Keywords (`show`, `wondering`)
- Symbols (`(`, `)`, `{`, `}`)

The goal is to simplify raw text into pieces that are easier to work with in later stages.


## 2. AST (Abstract Syntax Tree)

The **AST**, or **Abstract Syntax Tree**, is a structured representation of the code.  
Instead of working directly with text, the language transforms tokens into a **tree**, where each node represents an operation or command.

Examples of nodes in an AST (With Aya Language keywords examples):

- **NumberNode** → represents a literal value.
- **VarNode** → represents a variable.
- **BinOpNode** → represents a binary operation, like `+` or `>`.
- **ShowNode** → represents an output command (`show`).
- **WonderingNode** → represents a conditional (`wondering`) with a block of code.

The AST makes code analysis and execution easier.


## 3. Parser

The **parser** takes tokens from the lexer and builds the AST.  
It is responsible for checking if the code is **syntactically correct**, meaning the structure of the code makes sense.

Conceptual responsibilities:

- Ensures parentheses and braces are closed.
- Ensures operators and operands appear in the correct order.
- Groups tokens into hierarchical structures (the tree) representing operations, conditions, and commands.


## 4. Interpreter

The **interpreter** walks through the AST and executes the code.  
It gives meaning to the instructions, evaluating expressions and performing operations.

Typical tasks of an interpreter:

- Evaluate mathematical expressions (`x + y`, `a * b`)
- Manage variables and their values
- Execute conditions (`wondering`) and loops
- Execute output commands (`show`)

The interpreter connects all previous steps, transforming the abstract structure of the AST into concrete actions.


## Summary

The general flow of a programming language is:
```
Source Code → Lexer → Tokens → Parser → AST → Interpreter → Execution
```
- Each stage processes and transforms the code, making it possible to create your own language.