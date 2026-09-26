# COMPILER DESIGN — MASTER STUDY NOTES & MIDSEM REVISION GUIDE

> **Course**: Compiler Design (Semester 5 — IIITDM Kancheepuram)  
> **Target Audience**: 3rd Year B.Tech Computer Science & Engineering  
> **Syllabus Coverage**: Modules 1 & 2 — Everything from Introduction up to **LL(1) Parsing Algorithm**  
> **Source Material**: Directly consolidated from `compilerdesignhandwrittennotes.pdf`, classroom lecture slides, and past midsem exam papers.

---

## 📑 TABLE OF CONTENTS

- [MODULE 1: Foundations, Lexical Analysis & Grammars](#module-1-foundations-lexical-analysis--grammars)
  - [1. Introduction to Compilers & Language Processing Systems](#1-introduction-to-compilers--language-processing-systems)
  - [2. Phases of a Compiler (High-Yield 2M/5M Topic)](#2-phases-of-a-compiler-high-yield-2m5m-topic)
  - [3. Lexical Analysis: Tokens, Patterns, Lexemes & Formal Languages](#3-lexical-analysis-tokens-patterns-lexemes--formal-languages)
  - [4. Finite Automata & RE Conversion Pipeline](#4-finite-automata--re-conversion-pipeline)
    - [4.1 Thompson's Construction ($RE \to \epsilon$-NFA)](#41-thompsons-construction-re-\to-\epsilon-nfa)
    - [4.2 $\epsilon$-NFA to NFA Conversion](#42-\epsilon-nfa-to-nfa-conversion)
    - [4.3 NFA to DFA Conversion (Subset Construction)](#43-nfa-to-dfa-conversion-subset-construction)
    - [4.4 DFA Minimization: Partition Refinement & Table-Filling](#44-dfa-minimization-partition-refinement--table-filling)
  - [5. The Lex / Flex Tool & Regular Expressions in Lex](#5-the-lex--flex-tool--regular-expressions-in-lex)
  - [6. Formal Grammars & Chomsky Hierarchy](#6-formal-grammars--chomsky-hierarchy)
  - [7. Ambiguity in Grammars & Operator Semantics](#7-ambiguity-in-grammars--operator-semantics)
    - [7.1 How to Identify Operator Precedence](#71-how-to-identify-operator-precedence)
    - [7.2 How to Identify Associativity](#72-how-to-identify-associativity)
    - [7.3 Systematic Disambiguation Procedure](#73-systematic-disambiguation-procedure)
    - [7.4 Dangling Else Ambiguity](#74-dangling-else-ambiguity)
  - [8. Grammar Transformations for Top-Down Parsing](#8-grammar-transformations-for-top-down-parsing)
    - [8.1 Elimination of Left Recursion (Direct & Indirect)](#81-elimination-of-left-recursion-direct--indirect)
    - [8.2 Left Factoring (Eliminating Non-Determinism)](#82-left-factoring-eliminating-non-determinism)
- [MODULE 2: Syntax Analysis, Backtracking & LL(1) Parsing](#module-2-syntax-analysis-backtracking--ll1-parsing)
  - [9. Parser Taxonomy & Top-Down vs Bottom-Up](#9-parser-taxonomy--top-down-vs-bottom-up)
  - [10. Backtracking in Top-Down Parsers](#10-backtracking-in-top-down-parsers)
  - [11. Recursive Descent Parser (RDP) & Implementation](#11-recursive-descent-parser-rdp--implementation)
  - [12. FIRST and FOLLOW Sets (Comprehensive Rules & Examples)](#12-first-and-follow-sets-comprehensive-rules--examples)
  - [13. LL(1) Predictive Parsing Algorithm](#13-ll1-predictive-parsing-algorithm)
    - [13.1 LL(1) Disjointness Conditions](#131-ll1-disjointness-conditions)
    - [13.2 LL(1) Parsing Table Construction](#132-ll1-parsing-table-construction)
    - [13.3 Stack-Based LL(1) Simulation Engine & Trace](#133-stack-based-ll1-simulation-engine--trace)
- [MODULE 3: High-Yield Midsem Exam Questions & Solutions](#module-3-high-yield-midsem-exam-questions--solutions)

---

# MODULE 1: Foundations, Lexical Analysis & Grammars

---

## 1. Introduction to Compilers & Language Processing Systems

### 1.1 What is a Compiler?
A **compiler** is a specialized system software program that translates computer code written in one programming language (the **Source Language**) into another language (the **Target Language**, typically native machine code or assembly code) while preserving the exact program semantics and reporting diagnostic errors.

$$\text{Source Code} \xrightarrow{\quad\text{COMPILER}\quad} \text{Target Code (Machine / Assembly)}$$

### 1.2 Compiler vs. Interpreter vs. Hybrid Systems

| Feature | Compiler | Interpreter | Hybrid (e.g., Java, Python) |
|---|---|---|---|
| **Translation Mechanism** | Translates entire source code ahead-of-time (AOT) into binary object code. | Translates and executes the source code instruction-by-instruction. | Compiles source into intermediate bytecode; VM interprets/JIT compiles bytecode. |
| **Execution Speed** | Very fast runtime execution (direct hardware execution). | Slower execution (software emulation overhead on each line). | Medium to fast (Just-In-Time compilation caches native machine code). |
| **Memory Overhead** | Requires memory only for running the standalone binary executable. | Interpreter software must remain resident in memory during execution. | JVM / Python runtime engine must reside in memory. |
| **Error Diagnostics** | Reports all lexical, syntactic, and semantic errors before execution. | Halts execution immediately at the first encountered error. | Compile errors caught upfront; runtime type errors caught during execution. |
| **Portability** | Target machine binary is machine-specific (non-portable). | Source code is highly portable across any system with the interpreter. | Bytecode is fully portable ("Write Once, Run Anywhere"). |
| **Target Output** | Standalone machine binary (`.exe`, `.elf`). | No permanent intermediate object file created. | Bytecode binary (`.class`, `.pyc`). |

### 1.3 The Complete Language Processing System (LPS Pipeline)

Source code does not go directly to the compiler in isolation. It passes through a multi-stage environment:

```
[ Source Program (.c, .cpp) ]
               │
               ▼
      ┌─────────────────┐
      │   PREPROCESSOR  │  ──► Expands macros (#define), includes headers (#include),
      └─────────────────┘      removes comments, resolves conditional compilation (#ifdef)
               │
               ▼ [ Pure / Modified Source Code (.i) ]
      ┌─────────────────┐
      │    COMPILER     │  ──► Translates source code into target assembly mnemonics
      └─────────────────┘
               │
               ▼ [ Assembly Code (.s, .asm) ]
      ┌─────────────────┐
      │    ASSEMBLER    │  ──► Translates assembly mnemonics into relocatable machine code
      └─────────────────┘
               │
               ▼ [ Relocatable Object Code (.o, .obj) ]
      ┌─────────────────┐
      │  LINKER/LOADER  │  ──► Linker: Merges library routines & object files
      └─────────────────┘      Loader: Binds addresses & places code in executable RAM
               │
               ▼
[ Target Machine Code / Absolute Binary (.exe) ]
```

1. **Preprocessor**:
   - **Macro substitution**: Replaces identifier occurrences with defined text tokens (e.g., `#define MAX 100`).
   - **File inclusion**: Inlines standard library headers and developer headers (e.g., `#include <stdio.h>`).
   - **Conditional compilation**: Filters out platform-specific or debug blocks (e.g., `#ifdef DEBUG`).
   - **Stripping**: Strips whitespace, newline adjustments, and comments.
2. **Compiler**: Translates modified source code into target assembly language.
3. **Assembler**: Converts assembly instructions into binary relocatable object code (machine instructions with symbolic addresses unresolved).
4. **Linker**: Resolves cross-file external symbol references, links static archive libraries (`libc.a`), and calculates absolute/relative memory offsets.
5. **Loader**: Allocates primary memory (RAM), binds relocatable addresses to actual physical/virtual memory spaces, initializes the program counter (PC), and starts execution.

---

## 2. Phases of a Compiler (High-Yield 2M/5M Topic)

### 2.1 The Two Major Compiler Super-Structures
1. **Analysis Phase (Front End)**:
   - Language-dependent, machine-independent.
   - Breaks the source code into constituent pieces, checks syntax and semantics, builds the parse tree, and constructs an **Intermediate Representation (IR)**.
   - Includes: Lexical Analysis, Syntax Analysis, Semantic Analysis, and Intermediate Code Generation.
2. **Synthesis Phase (Back End)**:
   - Target-machine-dependent, source-language-independent.
   - Takes the IR and constructs the optimized target machine program.
   - Includes: Machine-independent Optimization, Code Generation, and Target-dependent Optimization.

### 2.2 The 6 Fundamental Phases & Auxiliary Components

```
Source Code
    │
    ▼
┌───────────────────────────┐
│ 1. Lexical Analyzer       │ ◄───► [ SYMBOL TABLE MANAGER ]
│    (Scanner)              │       Stores variable names, function
└───────────────────────────┘       signatures, types, scope, offsets
    │ Stream of Tokens
    ▼
┌───────────────────────────┐
│ 2. Syntax Analyzer        │ ◄───► [ ERROR HANDLER ]
│    (Parser)               │       Detects, logs, and recovers from
└───────────────────────────┘       errors at each phase without
    │ Parse / Syntax Tree           crashing the compilation pipeline
    ▼
┌───────────────────────────┐
│ 3. Semantic Analyzer      │
│    (Type & Scope Checker) │
└───────────────────────────┘
    │ Decorated / Annotated Tree
    ▼
┌───────────────────────────┐
│ 4. Intermediate Code Gen  │
│    (Three-Address Code)   │
└───────────────────────────┘
    │ Intermediate Representation (3AC)
    ▼
┌───────────────────────────┐
│ 5. Code Optimizer         │
│    (Efficiency Tuning)    │
└───────────────────────────┘
    │ Optimized Intermediate Representation
    ▼
┌───────────────────────────┐
│ 6. Target Code Generator  │
│    (Assembly / Machine)   │
└───────────────────────────┘
    │
    ▼
Target Machine Assembly
```

### 2.3 Comprehensive Phase-by-Phase Walkthrough

Let us trace the canonical statement across every phase:
$$\mathbf{position = initial + rate * 60}$$

#### Phase 1: Lexical Analysis (Scanning)
- Reads the raw input character stream from left to right.
- Groups characters into meaningful sequences called **lexemes** and maps them to abstract **tokens**.
- Strips all whitespace, tabs, and comments.
- Enters identifiers into the **Symbol Table**.
- **Output Token Stream**:
  $$\langle \mathbf{id}, 1 \rangle \quad \langle = \rangle \quad \langle \mathbf{id}, 2 \rangle \quad \langle + \rangle \quad \langle \mathbf{id}, 3 \rangle \quad \langle * \rangle \quad \langle \mathbf{num}, 60 \rangle$$
  *(Where `id, 1` = `position`, `id, 2` = `initial`, `id, 3` = `rate`)*

#### Phase 2: Syntax Analysis (Hierarchical Parsing)
- Takes the token stream and verifies if it adheres to the formal grammar rules of the programming language.
- Builds a hierarchical structure called a **Parse Tree** or **Abstract Syntax Tree (AST)**.
- **Output Syntax Tree**:
```
        =
      /   \
  id, 1    +
         /   \
     id, 2    *
            /   \
        id, 3   60
```

#### Phase 3: Semantic Analysis (Meaning & Type Checking)
- Checks the syntax tree for semantic consistency with language definitions.
- Performs **Type Checking**: Ensures compatible data types for operands and operators.
- Performs **Type Coercion (Implicit Type Conversion)**: If `rate` is a floating-point number, the integer constant `60` is automatically converted into a float (`inttofloat(60)` or `60.0`).
- **Output Annotated Syntax Tree**:
```
        =
      /   \
  id, 1    +
         /   \
     id, 2    *
            /   \
        id, 3   inttofloat
                    │
                    60
```

#### Phase 4: Intermediate Code Generation (ICG)
- Translates the decorated AST into an explicit, low-level, machine-independent representation.
- The most standard representation is **Three-Address Code (3AC)**, where each instruction has at most one operator on the right-hand side.
- **Output Three-Address Code**:
  $$t_1 = \text{inttofloat}(60)$$
  $$t_2 = \mathbf{id}_3 * t_1$$
  $$t_3 = \mathbf{id}_2 + t_2$$
  $$\mathbf{id}_1 = t_3$$

#### Phase 5: Code Optimization
- Analyzes the 3AC to eliminate redundancies, reduce cycle count, and minimize register usage.
- Applies transformations like:
  - **Compile-time Constant Folding**: Converts `inttofloat(60)` directly to `60.0` at compile time.
  - **Temporary Variable Elimination**: Replaces redundant assignments.
- **Output Optimized 3AC**:
  $$t_1 = \mathbf{id}_3 * 60.0$$
  $$\mathbf{id}_1 = \mathbf{id}_2 + t_1$$

#### Phase 6: Target Code Generation
- Maps intermediate instructions into target assembly mnemonics.
- Performs **Register Allocation and Assignment** (placing active variables into fast CPU registers $R_1, R_2$).
- **Output Assembly Code**:
  ```assembly
  LDF   R2, id3         ; Load float 'rate' into register R2
  MULF  R2, R2, #60.0   ; Multiply R2 by 60.0
  LDF   R1, id2         ; Load float 'initial' into register R1
  ADDF  R1, R1, R2      ; Add R2 into R1
  STF   id1, R1         ; Store result into 'position'
  ```

---

### 2.4 Lexical vs. Syntax Analysis — Why Separate Them? *(Frequent 2M Exam Question)*

1. **Simplicity of Design**: Separating the low-level lexical mechanics (stripping whitespace, comments, recognizing identifiers) simplifies the grammar rules and parsing algorithms dramatically.
2. **Compiler Efficiency**: Specialized string-scanning techniques, two-buffer schemes, and direct DFA implementations make the scanner fast. Syntax parsers, which manage pushdown stacks and trees, do not have to process characters one by one.
3. **Compiler Portability**: Non-standard lexical quirks (such as character encoding sets like ASCII vs UTF-8, operating system line-endings `\r\n` vs `\n`) are completely isolated within the front scanner. The parser remains entirely platform-independent.

### 2.5 Two-Phase Scanning Mechanism
1. **Scanning Proper**: Strips whitespace, spaces, tabs, newline counters, and comments.
2. **Lexical Analysis Proper**: Emits formal tokens, looks up keyword tables, and enters identifiers into the Symbol Table.

---

## 3. Lexical Analysis: Tokens, Patterns, Lexemes & Formal Languages

### 3.1 Terminology & Definitions

| Concept | Formal Definition | Real-World Example |
|---|---|---|
| **Token** | An abstract category / terminal symbol used by the parser. Formally represented as a pair: $\langle \text{token\_name}, \text{attribute\_value} \rangle$. | `<id, "count">`, `<num, 42>`, `<relop, LE>`, `<if, ->` |
| **Lexeme** | The concrete sequence of characters in the source code matching the pattern for a token. | `count`, `42`, `<=`, `if` |
| **Pattern** | The formal descriptive rule (written as a Regular Expression) governing which character sequences form a valid token. | `[a-zA-Z_][a-zA-Z0-9_]*` for identifiers |

### 3.2 Formal Language Fundamentals
- **Alphabet ($\Sigma$)**: Any finite set of symbols (e.g., binary $\Sigma = \{0, 1\}$, ASCII).
- **String**: A finite sequence of symbols chosen from $\Sigma$.
  - Length $|s|$: Total number of symbols in $s$.
  - Empty string $\epsilon$: String of length $0$ ($|\epsilon| = 0$).
- **String Terms**:
  - **Prefix**: Any leading symbols of $s$. A prefix of `banana` is `ban`. $\epsilon$ is a prefix of every string.
  - **Suffix**: Any trailing symbols of $s$. A suffix of `banana` is `ana`.
  - **Substring**: Any contiguous sequence of symbols within $s$.
  - **Proper Prefix / Suffix / Substring**: Any non-empty prefix, suffix, or substring not equal to $s$ itself.
  - **Subsequence**: Any sequence obtained by deleting zero or more symbols from $s$ without changing the order of the remaining symbols (e.g., `bnn` is a subsequence of `banana`).

### 3.3 Language Operations
Given two languages $L$ and $M$:
1. **Union**: $L \cup M = \{ s \mid s \in L \text{ or } s \in M \}$
2. **Concatenation**: $LM = \{ st \mid s \in L \text{ and } t \in M \}$
3. **Kleene Closure ($L^*$)**: Zero or more occurrences of $L$:
   $$L^* = \bigcup_{i=0}^{\infty} L^i = L^0 \cup L^1 \cup L^2 \cup \dots \quad (\text{where } L^0 = \{\epsilon\})$$
4. **Positive Closure ($L^+$)**: One or more occurrences of $L$:
   $$L^+ = \bigcup_{i=1}^{\infty} L^i = L L^* = L^* \setminus \{\epsilon\} \quad (\text{if } \epsilon \notin L)$$

### 3.4 Regular Expressions (RE)
- **Base Cases**:
  1. $\epsilon$ is a regular expression denoting $L(\epsilon) = \{\epsilon\}$.
  2. If $a \in \Sigma$, then $a$ is a regular expression denoting $L(a) = \{a\}$.
- **Inductive Operators & Precedence**:
  1. **Kleene Star ($*$)**: Highest precedence, left-associative.
  2. **Concatenation ($\cdot$)**: Second highest precedence, left-associative.
  3. **Alternation / Union ($\mid$)**: Lowest precedence, left-associative.
  $$\text{Example: } a \mid b \cdot c^* \equiv (a) \mid (b \cdot (c^*))$$

### 3.5 Algebraic Laws of Regular Expressions

| Law | Formulation |
|---|---|
| **Commutative** | $r \mid s = s \mid r$ |
| **Associative** | $(r \mid s) \mid t = r \mid (s \mid t)$ and $(rs)t = r(st)$ |
| **Distributive** | $r(s \mid t) = rs \mid rt$ and $(s \mid t)r = sr \mid tr$ |
| **Identity** | $\epsilon r = r \epsilon = r$ and $\emptyset \mid r = r \mid \emptyset = r$ |
| **Annihilator** | $\emptyset r = r \emptyset = \emptyset$ |
| **Idempotence** | $r \mid r = r$ |
| **Star Laws** | $r^{**} = r^*$, $\quad (r^*)^* = r^*$, $\quad (\epsilon \mid r)^* = r^*$, $\quad r^* = (r \mid \epsilon)^*$ |
| **Expansion Law** | $r^* = \epsilon \mid r r^* = \epsilon \mid r^+$, $\quad (r \mid s)^* = (r^* s^*)^* = (r^* \mid s^*)^*$ |

---

## 4. Finite Automata & RE Conversion Pipeline

### The 6-Step Implementation Pipeline

$$\text{Regular Expressions} \xrightarrow{\text{Thompson's Construction}} \epsilon\text{-NFA} \xrightarrow{\epsilon\text{-Elimination}} \text{NFA} \xrightarrow{\text{Subset Construction}} \text{DFA} \xrightarrow{\text{Minimization}} \text{Min-DFA} \xrightarrow{\text{Tabulation}} \text{DFA Table Engine}$$

---

### 4.1 Thompson's Construction ($RE \to \epsilon$-NFA)

Thompson's Algorithm builds an $\epsilon$-NFA compositionally from subexpressions.

#### 1. Fundamental Properties of Thompson's Automata
1. Exactly **one** initial (start) state.
2. Exactly **one** accepting (final) state.
3. No transitions enter the initial state; no transitions leave the final state.
4. Total states in the machine for expression $r$ satisfies: $|Q| \le 2 \times \text{length}(r)$.
5. Every state has either at most one outgoing symbol transition, or at most two outgoing $\epsilon$-transitions.

#### 2. Construction Rules

##### Base Cases
- **Symbol $\epsilon$**:
  $$\text{Start} \xrightarrow{\quad\quad} (i) \xrightarrow{\quad \epsilon \quad} ((f))$$
- **Symbol $a \in \Sigma$**:
  $$\text{Start} \xrightarrow{\quad\quad} (i) \xrightarrow{\quad a \quad} ((f))$$

##### Inductive Cases
- **Alternation ($r_1 \mid r_2$)**:
  Creates a new start state with $\epsilon$-transitions to the starts of $N(r_1)$ and $N(r_2)$, and $\epsilon$-transitions from their final states to a new final state:
```
              ┌──► [ N(r_1) ] ──┐
       ε      │                 │ ε
(start) ──────┤                 ├─────► ((final))
              │                 │
              └──► [ N(r_2) ] ──┘
```

- **Concatenation ($r_1 r_2$)**:
  The final state of $N(r_1)$ is merged with (or connected via $\epsilon$ to) the start state of $N(r_2)$:
```
(start) ──► [ N(r_1) ] ──(merged / ε)──► [ N(r_2) ] ──► ((final))
```

- **Kleene Closure ($r_1^*$)**:
  Adds a new start state, a new final state, a forward bypass $\epsilon$-transition (for $\epsilon$ matching), and a backward loop $\epsilon$-transition (for repetition):
```
          ┌────────────────────────────────────────┐
          │                  ε                     │
          ▼                                        │
(start) ─────► (i) ────► [ N(r_1) ] ────► (f) ─────┴──► ((final))
   │                                                       ▲
   └─────────────────────────── ε ─────────────────────────┘
```

- **Positive Closure ($r_1^+$)**:
  Equivalent to $r_1 r_1^*$. Preserves at least one traversal through $N(r_1)$ before allowing a loop back:
```
(start) ──► [ N(r_1) ] ──┬──► ((final))
               ▲         │
               └──── ε ──┘
```

---

### 4.2 $\epsilon$-NFA to NFA Conversion

An $\epsilon$-NFA allows transitions on empty string $\epsilon$. An NFA allows transitions only on alphabet symbols $\Sigma$, but can transition to multiple states.

#### Formal Mathematical Conversion Formula
$$\delta_{\text{NFA}}(q, a) = \epsilon\text{-closure}\Big(\delta_{\epsilon}\big(\epsilon\text{-closure}(q), a\big)\Big)$$

Where:
- $\epsilon\text{-closure}(q)$: The set of all states reachable from state $q$ by traversing zero or more $\epsilon$-transitions.
- A state $q$ in the resulting NFA is an accepting state if $\epsilon\text{-closure}(q)$ contains at least one accepting state of the $\epsilon$-NFA.

#### Worked Step-by-Step Example (From Handwritten Notes Page 20)
**Given**: $\epsilon$-NFA with states $Q = \{A, B, C\}$, alphabet $\Sigma = \{0, 1\}$, start state $A$, final state $C$:
- Transitions:
  - State $A$: loop on $0$, $\epsilon$-transition to $B$.
  - State $B$: loop on $1$, $\epsilon$-transition to $C$.
  - State $C$: loop on $0$, loop on $1$.

**Step 1: Compute $\epsilon$-closures for all states**:
- $\epsilon\text{-closure}(A) = \{A, B, C\}$ (from $A$, can reach $B$ via $\epsilon$, and $C$ via $B \xrightarrow{\epsilon} C$).
- $\epsilon\text{-closure}(B) = \{B, C\}$.
- $\epsilon\text{-closure}(C) = \{C\}$.

**Step 2: Compute NFA Transitions $\delta'(q, x)$**:
- **For State $A$**:
  - Input $0$:
    $$\epsilon\text{-closure}(A) = \{A, B, C\}$$
    $$\text{move}(\{A, B, C\}, 0) = \{A, C\} \quad (\text{since } A \xrightarrow{0} A, B \xrightarrow{0} \emptyset, C \xrightarrow{0} C)$$
    $$\delta'(A, 0) = \epsilon\text{-closure}(\{A, C\}) = \{A, B, C\} \cup \{C\} = \{A, B, C\}$$
  - Input $1$:
    $$\text{move}(\{A, B, C\}, 1) = \{B, C\} \quad (\text{since } A \xrightarrow{1} \emptyset, B \xrightarrow{1} B, C \xrightarrow{1} C)$$
    $$\delta'(A, 1) = \epsilon\text{-closure}(\{B, C\}) = \{B, C\}$$
- **For State $B$**:
  - Input $0$: $\text{move}(\{B, C\}, 0) = \{C\} \implies \epsilon\text{-closure}(\{C\}) = \{C\}$
  - Input $1$: $\text{move}(\{B, C\}, 1) = \{B, C\} \implies \epsilon\text{-closure}(\{B, C\}) = \{B, C\}$
- **For State $C$**:
  - Input $0$: $\text{move}(\{C\}, 0) = \{C\} \implies \delta'(C, 0) = \{C\}$
  - Input $1$: $\text{move}(\{C\}, 1) = \{C\} \implies \delta'(C, 1) = \{C\}$

**Step 3: Determine Final States**:
- Since $C$ is the original accepting state:
  - $\epsilon\text{-closure}(A) = \{A, B, C\}$ contains $C \implies A$ is an accepting state!
  - $\epsilon\text{-closure}(B) = \{B, C\}$ contains $C \implies B$ is an accepting state!
  - $\epsilon\text{-closure}(C) = \{C\}$ contains $C \implies C$ is an accepting state!
- Hence, all states $\{A, B, C\}$ are accepting in the converted NFA.

---

### 4.3 NFA to DFA Conversion (Subset Construction)

A Deterministic Finite Automaton (DFA) has $\delta: Q \times \Sigma \to Q$ (each state has exactly one deterministic transition for each input symbol).

#### The Subset Construction Algorithm
1. Let the start state of the DFA be $S_0 = \epsilon\text{-closure}(q_0)$.
2. For each unmarked DFA state $U \in D_{\text{states}}$ and each input symbol $a \in \Sigma$:
   $$T = \epsilon\text{-closure}(\text{move}(U, a))$$
   - If $T \neq \emptyset$ and $T \notin D_{\text{states}}$, add $T$ as an unmarked state to $D_{\text{states}}$.
   - Set $D_{\text{tran}}[U, a] = T$.
   - If $T = \emptyset$, map it to the Dead / Trap State $\Phi$.
3. Mark $U$ as processed. Repeat until all states in $D_{\text{states}}$ are marked.
4. An accepting state of the DFA is any set $U \in D_{\text{states}}$ containing at least one accepting state of the NFA.

#### Worked Step-by-Step Example (From Handwritten Notes Page 19)
**Given NFA**: States $Q = \{q_0, q_1, q_2\}$, Start $= q_0$, Final $= \{q_2\}$, Alphabet $\Sigma = \{a, b\}$.
- Transitions:
  - $q_0 \xrightarrow{a} \{q_0, q_1\}$
  - $q_0 \xrightarrow{b} \{q_0\}$
  - $q_1 \xrightarrow{b} \{q_2\}$
  - $q_2$ has no outgoing transitions.

**Execution Table**:

| DFA State | Equivalent NFA Subset | Transition on $a$ | Transition on $b$ | Is Final? |
|---|---|---|---|---|
| $Q_0$ | $\{q_0\}$ | $\{q_0, q_1\} = Q_1$ | $\{q_0\} = Q_0$ | No |
| $Q_1$ | $\{q_0, q_1\}$ | $\{q_0, q_1\} = Q_1$ | $\{q_0, q_2\} = Q_2$ | No |
| $Q_2$ | $\{q_0, q_2\}$ | $\{q_0, q_1\} = Q_1$ | $\{q_0\} = Q_0$ | **Yes** ($q_2 \in Q_2$) |

*Dead state $\Phi$ is not needed here as every transition from $\{q_0, q_2\}$ on $a$ and $b$ is handled by the $q_0$ component.*

---

### 4.4 DFA Minimization

Given a DFA $M$, construct an equivalent DFA $M'$ with the minimum number of states.

---

#### Method 1: Equivalence Partition Refinement (Hopcroft's Principle)

Two states $p$ and $q$ are $k$-equivalent if for all input strings $w$ of length $\le k$, $\delta(p, w)$ and $\delta(q, w)$ are either both accepting or both non-accepting.

##### Algorithm Steps
1. **Initial Partition $P_0$**: Divide states into two groups:
   $$P_0 = \{ G_1, G_2 \} = \{ F, Q \setminus F \}$$
   *(Accepting states and Non-accepting states)*
2. **Refinement Iteration**:
   - For each group $G \in P_k$, check if for any input symbol $a \in \Sigma$, two states $s_1, s_2 \in G$ transition into different groups of partition $P_k$:
     $$\delta(s_1, a) \in G_i \quad \text{and} \quad \delta(s_2, a) \in G_j \quad (i \neq j)$$
   - If they do, split $G$ into subgroups.
3. **Termination**: Repeat until $P_{k+1} = P_k$ (no further partition splits occur).
4. Combine equivalent states into unified representative states.

##### Worked Example 1: 5-State DFA (From Handwritten Notes Page 5)
- States: $Q = \{A, B, C, D, E\}$, $\Sigma = \{0, 1\}$, Start $= A$, Final $= \{E\}$.
- Transitions:
  - $A \xrightarrow{0} B, \quad A \xrightarrow{1} C$
  - $B \xrightarrow{0} B, \quad B \xrightarrow{1} D$
  - $C \xrightarrow{0} B, \quad C \xrightarrow{1} C$
  - $D \xrightarrow{0} B, \quad D \xrightarrow{1} E$
  - $E \xrightarrow{0} B, \quad E \xrightarrow{1} C$

**Step 1: Initial Partition $P_0$**:
$$P_0 = \{ (A, B, C, D), (E) \}$$

**Step 2: Check Partition $P_0$ on inputs 0 and 1**:
- For group $(A, B, C, D)$:
  - Transitions on $0$:
    - $A \xrightarrow{0} B \in (A,B,C,D)$
    - $B \xrightarrow{0} B \in (A,B,C,D)$
    - $C \xrightarrow{0} B \in (A,B,C,D)$
    - $D \xrightarrow{0} B \in (A,B,C,D)$
    *(All states stay in the same group on input 0)*
  - Transitions on $1$:
    - $A \xrightarrow{1} C \in (A,B,C,D)$
    - $B \xrightarrow{1} D \in (A,B,C,D)$
    - $C \xrightarrow{1} C \in (A,B,C,D)$
    - $D \xrightarrow{1} E \in (E) \implies \mathbf{D \text{ transitions to a different group!}}$
- Therefore, $D$ must be separated:
$$P_1 = \{ (A, B, C), (D), (E) \}$$

**Step 3: Check Group $(A, B, C)$ under $P_1$**:
- On input $0$: $A, B, C \to B \in (A, B, C)$ (no split).
- On input $1$:
  - $A \xrightarrow{1} C \in (A, B, C)$
  - $B \xrightarrow{1} D \in (D) \implies \mathbf{B \text{ transitions to group } (D)!}$
  - $C \xrightarrow{1} C \in (A, B, C)$
- Therefore, $B$ must be separated:
$$P_2 = \{ (A, C), (B), (D), (E) \}$$

**Step 4: Check Group $(A, C)$ under $P_2$**:
- On input $0$: $A \xrightarrow{0} B \in (B)$, $C \xrightarrow{0} B \in (B)$ (identical).
- On input $1$: $A \xrightarrow{1} C \in (A, C)$, $C \xrightarrow{1} C \in (A, C)$ (identical).
- No further splits occur! $P_3 = P_2$.

**Final Result**:
- Merged state: $[A, C]$
- Minimized DFA States: $\{[A, C], B, D, E\}$ (Reduced from 5 states to 4 states).

---

#### Method 2: Myhill-Nerode Table-Filling Method

This method builds a triangular state pair table to systematically discover indistinguishable states.

##### Step-by-Step Algorithm
1. **Construct a lower-triangular grid** of all state pairs $(p, q)$ where $p \neq q$.
2. **Base Marking**: Place an $\mathbf{X}$ in any pair $(p, q)$ where one state is accepting ($p \in F$) and the other is non-accepting ($q \notin F$).
3. **Inductive Marking**:
   - For all unmarked pairs $(p, q)$:
   - For each input symbol $a \in \Sigma$, find their successors $p' = \delta(p, a)$ and $q' = \delta(q, a)$.
   - If the pair $(p', q')$ is already marked with an $\mathbf{X}$, mark $(p, q)$ with an $\mathbf{X}$.
   - Repeat this sweep until an entire pass makes no new marks.
4. **Conclusion**: Any cells remaining **unmarked** represent equivalent, indistinguishable states that can be merged ($p \equiv q$).

##### Worked Example 2: 6-State DFA Table Filling (From Handwritten Notes Pages 6–7)
- States: $Q = \{A, B, C, D, E, F\}$, Alphabet $\Sigma = \{0, 1\}$, Final $= \{C, D, E\}$.
- Transitions:
  - $A \xrightarrow{0} B, \quad A \xrightarrow{1} C$
  - $B \xrightarrow{0} A, \quad B \xrightarrow{1} D$
  - $C \xrightarrow{0} E, \quad C \xrightarrow{1} F$
  - $D \xrightarrow{0} E, \quad D \xrightarrow{1} F$
  - $E \xrightarrow{0} E, \quad E \xrightarrow{1} F$
  - $F \xrightarrow{0} F, \quad F \xrightarrow{1} F$

**Execution**:
1. Base marking:
   - Final states are $\{C, D, E\}$; Non-final states are $\{A, B, F\}$.
   - Mark every pair combining one from $\{C, D, E\}$ and one from $\{A, B, F\}$ with $\mathbf{X}$.
2. Inductive check on remaining non-final pairs:
   - Pair $(A, B)$:
     - On $0$: $\delta(A, 0) = B$, $\delta(B, 0) = A \implies$ pair $(B, A)$ is not marked.
     - On $1$: $\delta(A, 1) = C$, $\delta(B, 1) = D \implies$ pair $(C, D)$ is not marked.
     - Pair $(A, B)$ remains unmarked!
   - Pair $(C, D)$:
     - On $0$: $\delta(C, 0) = E$, $\delta(D, 0) = E$ (same state).
     - On $1$: $\delta(C, 1) = F$, $\delta(D, 1) = F$ (same state).
     - Pair $(C, D)$ remains unmarked!
   - Pair $(D, E)$:
     - On $0$: $\delta(D, 0) = E$, $\delta(E, 0) = E$.
     - On $1$: $\delta(D, 1) = F$, $\delta(E, 1) = F$.
     - Pair $(D, E)$ remains unmarked!
   - By transitivity, pair $(C, E)$ remains unmarked.
3. **Equivalent States Discovered**:
   - $A \equiv B$
   - $C \equiv D \equiv E$
4. **Minimized DFA**:
   - Superstates: $[A, B]$, $[C, D, E]$, and $[F]$.
   - Reduced from 6 states to 3 states.

---

## 5. The Lex / Flex Tool & Regular Expressions in Lex

### 5.1 Structure of a Lex / Flex Specification (`.l`) File

A Lex source file is partitioned into three distinct sections separated by `%%`:

```lex
%{
/* SECTION 1: DEFINITIONS & C DECLARATIONS */
#include <stdio.h>
#include <stdlib.h>
int line_count = 1;
%}

/* Regular Definitions / Macros */
DIGIT    [0-9]
LETTER   [a-zA-Z_]
ID       {LETTER}({LETTER}|{DIGIT})*
NUM      {DIGIT}+(\.{DIGIT}+)?

%%
/* SECTION 2: TRANSLATION RULES (Pattern - Action Pairs) */
"if"            { printf("KEYWORD: IF\n"); }
"else"          { printf("KEYWORD: ELSE\n"); }
"while"         { printf("KEYWORD: WHILE\n"); }
{ID}            { printf("IDENTIFIER: %s\n", yytext); }
{NUM}           { printf("NUMBER: %s\n", yytext); }
"+"             { printf("OP_PLUS\n"); }
"=="            { printf("RELOP_EQ\n"); }
"\n"            { line_count++; }
[ \t]+          { /* Discard whitespace */ }
.               { printf("LEXICAL ERROR: Unknown character %s at line %d\n", yytext, line_count); }
%%

/* SECTION 3: AUXILIARY USER C CODE */
int yywrap() {
    return 1; /* Return 1 to indicate end of input file */
}

int main(int argc, char **argv) {
    yylex();
    printf("Total lines scanned: %d\n", line_count);
    return 0;
}
```

### 5.2 Core Flex Internal Variables & Functions

| Name | Type | Description |
|---|---|---|
| `yylex()` | Function (`int`) | The primary scanner routine generated by Flex. Reads input and returns tokens. |
| `yytext` | Variable (`char*`) | Pointer to the null-terminated matched string of characters (current lexeme). |
| `yyleng` | Variable (`int`) | Length of the matched lexeme string in `yytext`. |
| `yyin` | File pointer (`FILE*`) | Input file stream scanned by `yylex()` (defaults to `stdin`). |
| `yyout` | File pointer (`FILE*`) | Output file stream for unhandled characters (defaults to `stdout`). |
| `yywrap()` | Function (`int`) | Called when end-of-file is reached. Returning 1 halts scanning; returning 0 continues on next file. |

### 5.3 Lexical Conflict Resolution Rules (Crucial!)

When multiple regular expressions in Section 2 match the incoming input stream, Flex breaks ambiguities using two deterministic rules:
1. **Longest Match (Maximal Munch Rule)**:
   - The scanner chooses the rule matching the longest substring of input characters.
   - *Example*: Given input `<=`, if rules exist for `<` and `<=`, the scanner matches `<=` because it is 2 characters long.
2. **First Match Rule (Order of Rules Precedence)**:
   - If two rules match the exact same number of characters, the rule appearing **first in the `.l` file** wins.
   - *Example*: Given input `if`, it matches both the keyword `"if"` and the identifier pattern `[a-z]+`. By placing keywords **before** identifiers in Section 2, the keyword rule wins.

### 5.4 Practical Regular Expressions in Lex (From Notes Page 21)

```lex
/* Single-line comment */
"//".*                                  { /* ignore single-line comment */ }

/* Multi-line comment (handles arbitrary nested * not followed by /) */
"/*"([^*]|\*+[^*/])*\*+/               { /* ignore multi-line comment */ }

/* Whitespace */
[ \t\n]+                                { /* ignore whitespace */ }

/* Float number */
[0-9]+\.[0-9]+                          { printf("FLOAT: %s\n", yytext); }

/* Double-quoted string literal */
\"([^\"\\]|\\.)*\"                      { printf("STRING: %s\n", yytext); }

/* Character literal */
\'([^\'\\]|\\.)\'                       { printf("CHAR: %s\n", yytext); }

/* Keywords */
"int"|"float"|"char"|"double"           { printf("KEYWORD: %s\n", yytext); }

/* Identifiers */
[a-zA-Z_][a-zA-Z0-9_]*                  { printf("ID: %s\n", yytext); }

/* Catch-all Invalid Character */
.                                       { printf("INVALID: %s\n", yytext); }
```

---

## 6. Formal Grammars & Chomsky Hierarchy

### 6.1 Formal Definition of a Context-Free Grammar (CFG)
A CFG is formally defined as a 4-tuple:
$$G = (V_N, V_T, P, S)$$
Where:
- $V_N$ (or $N$): A finite set of **Non-terminal symbols** (syntactic variables).
- $V_T$ (or $T$, $\Sigma$): A finite set of **Terminal symbols** (lexical tokens). $V_N \cap V_T = \emptyset$.
- $P$: A finite set of **Production rules**, each of the form:
  $$A \to \alpha \quad \text{where } A \in V_N \text{ and } \alpha \in (V_N \cup V_T)^*$$
- $S$: The **Start symbol**, $S \in V_N$.

### 6.2 The Chomsky Hierarchy of Grammars

```
┌────────────────────────────────────────────────────────┐
│ Type 0: Unrestricted Grammars                          │
│ Recognized by: Turing Machine                          │
│ Form: α → β (no constraints on length)                 │
│ ┌────────────────────────────────────────────────────┐ │
│ │ Type 1: Context-Sensitive Grammars (CSG)           │ │
│ │ Recognized by: Linear Bounded Automaton (LBA)      │ │
│ │ Form: α → β where |α| ≤ |β| (non-contracting)      │ │
│ │ ┌────────────────────────────────────────────────┐ │ │
│ │ │ Type 2: Context-Free Grammars (CFG)            │ │ │
│ │ │ Recognized by: Non-Deterministic PDA (Pushdown)│ │ │
│ │ │ Form: A → α where A ∈ V_N, α ∈ (V_N ∪ V_T)*    │ │ │
│ │ │ ┌────────────────────────────────────────────┐ │ │ │
│ │ │ │ Type 3: Regular Grammars (RG)              │ │ │ │
│ │ │ │ Recognized by: Finite Automata (DFA/NFA)   │ │ │ │
│ │ │ │ Form: A → aB | a (Right Linear) OR        │ │ │ │
│ │ │ │       A → Ba | a (Left Linear)             │ │ │ │
│ │ │ └────────────────────────────────────────────┘ │ │ │
│ │ └────────────────────────────────────────────────┘ │ │
│ └────────────────────────────────────────────────────┘ │
└────────────────────────────────────────────────────────┘
```

> **Why are programming languages NOT Regular Grammars?**  
> Regular grammars have finite memory (states) and cannot count or balance arbitrary nested structures (such as matching parentheses `(( ... ))` or nested `begin ... end` blocks). CFGs utilize a pushdown stack memory, allowing arbitrary hierarchical nesting.

---

## 7. Ambiguity in Grammars & Operator Semantics

### 7.1 Definition of Ambiguity
A context-free grammar $G$ is **ambiguous** if there exists at least one string $w \in L(G)$ for which there are:
1. Two or more distinct **Parse Trees**, OR
2. Two or more distinct **Leftmost Derivations (LMD)**, OR
3. Two or more distinct **Rightmost Derivations (RMD)**.

#### Classic Ambiguous Expression Grammar:
$$E \to E + E \mid E * E \mid id$$
For string `id + id * id`, there are two distinct parse trees:
- Tree 1 groups `(id + id) * id` (interpreting $+$ as higher precedence than $*$).
- Tree 2 groups `id + (id * id)` (interpreting $*$ as higher precedence than $+$).
This ambiguity causes undefined semantic evaluation order.

---

### 7.2 How to Identify Operator Precedence from a Grammar

#### The Core Principle
Look at the vertical derivation hierarchy of the non-terminals:
$$\text{Start Symbol } S \longrightarrow \dots \longrightarrow \text{Intermediate Non-terminals} \longrightarrow \dots \longrightarrow \text{Operands}$$

> $$\boxed{\textbf{The DEEPER (lower-level) the non-terminal in the grammar, the HIGHER its operator precedence!}}$$

#### Why This Works
In a parse tree, nodes near the bottom are evaluated first because their subtrees must be completely reduced before parents can be evaluated.

#### Walkthrough
Given the grammar:
$$E \to E + T \mid T$$
$$T \to T * F \mid F$$
$$F \to id$$

Look at the non-terminal levels:
$$E \quad (\text{handles } +)$$
$$\downarrow$$
$$T \quad (\text{handles } *)$$
$$\downarrow$$
$$F \quad (\text{handles operands } id)$$

- $T$ is derived from $E$. To compute $E \to E + T$, the value of $T$ must already be computed.
- Therefore, $*$ is handled deeper down at the $T$ level, while $+$ is at the shallower $E$ level.
- **Conclusion**:
  $$\boxed{* \text{ has higher precedence than } +}$$

---

### 7.3 How to Identify Associativity from a Grammar

Associativity determines how operators of the **same precedence level** are grouped when chained together (e.g., $a + b + c$ or $a \wedge b \wedge c$).

#### The Memory Trick & Direction Rules

$$\boxed{\begin{aligned}
A \to A \text{ op } B \mid B \quad &\implies \quad \textbf{LEFT-associative} \quad (a \text{ op } b) \text{ op } c \\
A \to B \text{ op } A \mid B \quad &\implies \quad \textbf{RIGHT-associative} \quad a \text{ op } (b \text{ op } c)
\end{aligned}}$$

#### 1. Left Associativity (Left Recursion)
In:
$$E \to E + T \mid T$$
The recursive non-terminal $E$ appears on the **left side** of operator $+$.
Evaluating `a + b + c`:
$$E \Rightarrow E + T \Rightarrow (E + T) + T \Rightarrow (a + b) + c$$
The parse tree grows downwards to the left (left-leaning). Therefore, $+$ is **left-associative**.

#### 2. Right Associativity (Right Recursion)
Consider the exponentiation operator $\wedge$:
$$F \to G \wedge F \mid G$$
The recursive non-terminal $F$ appears on the **right side** of operator $\wedge$.
Evaluating `a ^ b ^ c`:
$$F \Rightarrow G \wedge F \Rightarrow G \wedge (G \wedge F) \Rightarrow a \wedge (b \wedge c)$$
The parse tree grows downwards to the right (right-leaning). Therefore, $\wedge$ is **right-associative**.

---

### 7.4 Systematic Disambiguation Procedure

To write an unambiguous grammar given operators, precedence, and associativity:
1. **Create one non-terminal level per precedence tier**.
2. **Assign the lowest-precedence operator to the start symbol**.
3. **Assign higher-precedence operators to successively deeper non-terminals**.
4. **Enforce associativity at each level**:
   - For Left-associative: $A \to A \text{ op } B \mid B$
   - For Right-associative: $A \to B \text{ op } A \mid B$
5. **The deepest level generates primary units** (identifiers, numbers, parenthesized expressions $(E)$).

#### Comprehensive Multi-Level Worked Example (From Notes Page 9)
Construct an unambiguous grammar for arithmetic expressions supporting:
- $+$ (Addition): Lowest precedence, Left-associative
- $*$ (Multiplication): Middle precedence, Left-associative
- $\wedge$ (Exponentiation): Highest precedence, Right-associative
- Operands: `id` and parenthesized sub-expressions `(E)`

**Construction**:
- **Level 1 (Lowest Precedence: $+$, Left-assoc)**:
  $$E \to E + T \mid T$$
- **Level 2 (Middle Precedence: $*$, Left-assoc)**:
  $$T \to T * F \mid F$$
- **Level 3 (Highest Precedence: $\wedge$, Right-assoc)**:
  $$F \to G \wedge F \mid G$$
- **Level 4 (Operands / Primaries)**:
  $$G \to (E) \mid id$$

---

### 7.5 Dangling Else Ambiguity
Consider conditional branching:
$$S \to \text{if } C \text{ then } S \mid \text{if } C \text{ then } S \text{ else } S \mid \text{other}$$

Given string:
$$\text{if } C_1 \text{ then if } C_2 \text{ then } S_1 \text{ else } S_2$$
It can be parsed two ways:
1. The `else` binds to the inner `if` ($C_2$).
2. The `else` binds to the outer `if` ($C_1$).

#### Disambiguation Rule
In programming languages (like C, Java), an `else` is always associated with the **closest preceding unmatched `then`**.

#### The Unambiguous Grammar
Partition statements into **Matched Statements** ($MS$) and **Open / Unmatched Statements** ($UMS$):
$$S \to MS \mid UMS$$
$$MS \to \text{if } C \text{ then } MS \text{ else } MS \mid \text{other}$$
$$UMS \to \text{if } C \text{ then } S \mid \text{if } C \text{ then } MS \text{ else } UMS$$

---

## 8. Grammar Transformations for Top-Down Parsing

Top-down predictive parsers (like LL(1) and Recursive Descent) cannot handle:
1. **Left-recursive grammars** (leads to infinite recursion).
2. **Non-deterministic grammars** (parser cannot decide which production to pick).

---

### 8.1 Elimination of Left Recursion

A grammar is **left-recursive** if it has a non-terminal $A$ such that:
$$A \Rightarrow^+ A \alpha \quad \text{for some string } \alpha$$

#### Why Left Recursion Breaks Top-Down Parsers
A top-down parser attempting to expand $A \to A \alpha$ calls $A()$ before consuming any input token. This results in an infinite call loop without advancing the input pointer.

#### Mathematical Foundation
The production $A \to A \alpha \mid \beta$ generates strings matching the regular expression $\beta \alpha^*$.  
By introducing a new non-terminal $A'$, we convert left recursion into right recursion:

$$\boxed{\begin{aligned}
A &\to \beta A' \\
A' &\to \alpha A' \mid \epsilon
\end{aligned}}$$

#### General Direct Left Recursion Elimination Formula
Given:
$$A \to A\alpha_1 \mid A\alpha_2 \mid \dots \mid A\alpha_m \mid \beta_1 \mid \beta_2 \mid \dots \mid \beta_n$$
*(Where no $\beta_i$ begins with $A$)*

Replace with:
$$\boxed{\begin{aligned}
A &\to \beta_1 A' \mid \beta_2 A' \mid \dots \mid \beta_n A' \\
A' &\to \alpha_1 A' \mid \alpha_2 A' \mid \dots \mid \alpha_m A' \mid \epsilon
\end{aligned}}$$

#### Eliminating Indirect Left Recursion (Algorithm & Example)
Indirect left recursion occurs when $A \Rightarrow B \dots \Rightarrow A \dots$.

##### Algorithm
1. Impose an arbitrary order on all non-terminals: $A_1, A_2, \dots, A_n$.
2. For $i = 1$ to $n$:
   - For $j = 1$ to $i-1$:
     - Replace each production $A_i \to A_j \gamma$ by $A_i \to \delta_1 \gamma \mid \delta_2 \gamma \mid \dots \mid \delta_k \gamma$ where $A_j \to \delta_1 \mid \dots \mid \delta_k$.
   - Eliminate any direct left recursion on $A_i$.

##### Worked Example (From Notes Page 22)
**Given**:
$$A \to B a$$
$$B \to A b \mid c$$

**Step 1: Check Ordering**:
Let ordering be $A, B$.
Look at $B$: $B \to A b$ has $A$ preceding $B$. Substitute $A$ into $B$:
$$B \to (B a) b \mid c \implies B \to B a b \mid c$$

**Step 2: Eliminate Direct Left Recursion on $B$**:
Here $\alpha = ab$, $\beta = c$.
$$B \to c B'$$
$$B' \to a b B' \mid \epsilon$$

**Step 3: Update $A$**:
$$A \to B a \implies A \to c B' a$$

---

### 8.2 Left Factoring (Eliminating Non-Determinism)

When two alternative productions for non-terminal $A$ share a common prefix, a predictive top-down parser cannot decide which production to choose by looking at just one token.

#### Mathematical Transformation
Given:
$$A \to \alpha \beta_1 \mid \alpha \beta_2 \mid \dots \mid \alpha \beta_n \mid \gamma$$
*(Where $\alpha$ is the longest common prefix and $\gamma$ represents alternatives not starting with $\alpha$)*

Replace with:
$$\boxed{\begin{aligned}
A &\to \alpha A' \mid \gamma \\
A' &\to \beta_1 \mid \beta_2 \mid \dots \mid \beta_n
\end{aligned}}$$

#### Worked Example 1 (From Notes Page 11)
**Given**:
$$A \to a A B \mid a B C \mid a A C$$
Common prefix is $a$:
$$A \to a A'$$
$$A' \to A B \mid B C \mid A C$$

Notice that $A'$ still has a common prefix $A$:
$$A' \to A (B \mid C) \mid B C$$
Left factoring again:
$$A' \to A A'' \mid B C$$
$$A'' \to B \mid C$$

#### Worked Example 2: Conditional Statements
**Given**:
$$S \to \text{if } E \text{ then } S \text{ else } S \mid \text{if } E \text{ then } S$$
Common prefix $\alpha = \text{if } E \text{ then } S$:
$$S \to \text{if } E \text{ then } S \, S'$$
$$S' \to \text{else } S \mid \epsilon$$

---

# MODULE 2: Syntax Analysis, Backtracking & LL(1) Parsing

---

## 9. Parser Taxonomy & Top-Down vs Bottom-Up

```
                                  PARSERS
                                     │
           ┌─────────────────────────┴─────────────────────────┐
           ▼                                                   ▼
   TOP-DOWN PARSERS                                    BOTTOM-UP PARSERS
   (Start from Root S → Leaves)                        (Start from Leaves → Root S)
   (Traces Leftmost Derivation)                        (Traces Reverse Rightmost Derivation)
           │                                                   │
     ┌─────┴──────────────┐                           ┌────────┴────────┐
     ▼                    ▼                           ▼                 ▼
With Backtracking    Without Backtracking     Operator Precedence   LR Parsers
(Brute Force)        (Deterministic)          (Shift-Reduce)        (LR(0), SLR(1),
                          │                                          LALR(1), CLR(1))
              ┌───────────┴───────────┐                             [Post-Midsem]
              ▼                       ▼
      Recursive Descent       Non-Recursive
      Parsing (RDP)           Predictive LL(1)
```

| Dimension | Top-Down Parsing | Bottom-Up Parsing |
|---|---|---|
| **Tree Traversal** | From root node $S$ down to input leaf tokens. | From input leaf tokens up to root node $S$. |
| **Derivation Traced** | **Leftmost Derivation (LMD)** in forward order. | **Rightmost Derivation (RMD)** in reverse order. |
| **Primary Mechanism** | Production expansion ($A \to \alpha$). | Reduction of handle string to non-terminal ($\alpha \to A$). |
| **Grammar Restrictions** | Cannot handle Left Recursion; requires Left Factoring. | Handles Left Recursion naturally; larger class of grammars. |
| **Lookahead Complexity** | LL(1), LL($k$). | LR(0), SLR(1), LALR(1), CLR(1). |

---

## 10. Backtracking in Top-Down Parsers

### 10.1 What is Backtracking (Brute Force Parsing)?
Backtracking is a top-down parsing approach where the parser attempts to match an input string by trying alternative grammar productions one by one. If a chosen production alternative fails to match the incoming input tokens at any point:
1. The parser resets / rewinds the input pointer back to where the production started.
2. It discards the speculative subtree.
3. It tries the next alternative production rule.

### 10.2 Concrete Backtracking Example
Consider:
$$S \to c A d$$
$$A \to a b \mid a$$
Input string $w = cad$.

1. Parser expands $S \to c A d$. Input token `c` matches terminal `c`. Input pointer advances to `a`.
2. Parser expands $A$ using its first alternative: $A \to a b$.
3. Token `a` matches `a`. Input pointer advances to `d`.
4. Next symbol in production is `b`, but incoming token is `d`. **Mismatch!**
5. **Backtracking Occurs**:
   - Discard branch $A \to a b$.
   - Rewind input pointer back to position 2 (`a`).
   - Try second alternative: $A \to a$.
6. Token `a` matches `a`. Non-terminal $A$ succeeds.
7. Next terminal in $S$ is `d`, which matches `d`. String accepted.

### 10.3 Why Backtracking is Unacceptable in Production Compilers
1. **Exponential Time Complexity $\mathcal{O}(2^n)$**: In the worst-case, trying all combinations yields exponential runtime.
2. **Input Buffer Rewinding Overhead**: The lexical scanner must maintain an arbitrarily large rollback buffer to restore file read states.
3. **Semantic Side-Effects**: If semantic actions (such as symbol table insertions or type allocations) executed during a speculative path, undoing them upon backtrack is notoriously difficult.

---

## 11. Recursive Descent Parser (RDP) & Implementation

### 11.1 Theory & Architecture
Recursive Descent is a deterministic top-down parsing technique where:
- Every **non-terminal** in the grammar corresponds to a distinct **C/programming language function**.
- The body of each function mirrors the right-hand side of its production rules.
- Terminal symbols are verified using a helper function called `match(terminal)`.

### 11.2 Prerequisites for Deterministic RDP
To implement an RDP without backtracking, the underlying grammar **MUST**:
1. Be **free of left recursion** (direct or indirect).
2. Be **left-factored** (no shared leading prefixes among alternatives).
3. Be **unambiguous**.

---

### 11.3 Complete C Implementation of an RDP (From Notes Page 13)

For the canonical arithmetic expression grammar:
$$E \to T E'$$
$$E' \to + T E' \mid \epsilon$$
$$T \to F T'$$
$$T' \to * F T' \mid \epsilon$$
$$F \to (E) \mid id$$

```c
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char lookahead;

/* Forward function declarations */
void E();
void E_prime();
void T();
void T_prime();
void F();

/* Advance input pointer if lookahead matches expected token */
void match(char t) {
    if (lookahead == t) {
        lookahead = getchar();
    } else {
        printf("\nSYNTAX ERROR: Unexpected token '%c', expected '%c'\n", lookahead, t);
        exit(1);
    }
}

/* E -> T E' */
void E() {
    T();
    E_prime();
}

/* E' -> + T E' | epsilon */
void E_prime() {
    if (lookahead == '+') {
        match('+');
        T();
        E_prime();
    }
    /* epsilon production: do nothing and return */
}

/* T -> F T' */
void T() {
    F();
    T_prime();
}

/* T' -> * F T' | epsilon */
void T_prime() {
    if (lookahead == '*') {
        match('*');
        F();
        T_prime();
    }
    /* epsilon production: do nothing and return */
}

/* F -> ( E ) | id */
void F() {
    if (lookahead == '(') {
        match('(');
        E();
        match(')');
    } else if (isalnum(lookahead)) { /* represents identifier 'id' */
        match(lookahead);
    } else {
        printf("\nSYNTAX ERROR: Invalid factor '%c'\n", lookahead);
        exit(1);
    }
}

int main() {
    printf("Enter arithmetic expression (e.g. i+i*i): ");
    lookahead = getchar();
    E();
    if (lookahead == '\n' || lookahead == '$' || lookahead == EOF) {
        printf("\nSUCCESS: Expression parsed successfully!\n");
    } else {
        printf("\nSYNTAX ERROR: Trailing characters detected '%c'\n", lookahead);
    }
    return 0;
}
```

---

## 12. FIRST and FOLLOW Sets (Comprehensive Rules & Examples)

FIRST and FOLLOW sets allow a predictive parser to choose the correct production rule using only 1 lookahead token.

---

### 12.1 Rules for Computing $\text{FIRST}(X)$

$\text{FIRST}(\alpha)$ is the set of all terminal symbols that can appear as the first symbol of a string derived from $\alpha$. If $\alpha \Rightarrow^* \epsilon$, then $\epsilon \in \text{FIRST}(\alpha)$.

1. **Terminal Symbol**:
   $$\text{If } X \text{ is a terminal, then } \text{FIRST}(X) = \{ X \}$$
2. **$\epsilon$-Production**:
   $$\text{If } X \to \epsilon \text{ is a production, add } \epsilon \text{ to } \text{FIRST}(X)$$
3. **Compound Non-Terminal RHS ($X \to Y_1 Y_2 \dots Y_k$)**:
   - Add all non-$\epsilon$ terminals in $\text{FIRST}(Y_1)$ to $\text{FIRST}(X)$.
   - If $\epsilon \in \text{FIRST}(Y_1)$, add all non-$\epsilon$ terminals in $\text{FIRST}(Y_2)$ to $\text{FIRST}(X)$.
   - Continue until some $Y_i$ does **not** derive $\epsilon$.
   - If all $Y_1, Y_2, \dots, Y_k$ derive $\epsilon$ (i.e., $\epsilon \in \text{FIRST}(Y_i)$ for all $1 \le i \le k$), add $\epsilon$ to $\text{FIRST}(X)$.

> **Computation Strategy**: Always calculate $\text{FIRST}$ sets **Bottom-Up** (from terminal-adjacent non-terminals up to the start symbol).

---

### 12.2 Rules for Computing $\text{FOLLOW}(A)$

$\text{FOLLOW}(A)$ is the set of terminals that can appear immediately to the right of non-terminal $A$ in some sentential form.

> $$\boxed{\textbf{CRITICAL RULE: } \mathbf{\epsilon \text{ is NEVER in any FOLLOW set!}}}$$

1. **Start Symbol ($S$)**:
   $$\text{Place } \$ \text{ (input endmarker) into } \text{FOLLOW}(S)$$
2. **Production $A \to \alpha B \beta$**:
   $$\text{Everything in } \text{FIRST}(\beta) \setminus \{\epsilon\} \text{ is in } \text{FOLLOW}(B)$$
3. **Production $A \to \alpha B$ OR $A \to \alpha B \beta$ where $\epsilon \in \text{FIRST}(\beta)$**:
   $$\text{Everything in } \text{FOLLOW}(A) \text{ is in } \text{FOLLOW}(B)$$

> **Computation Strategy**: Always calculate $\text{FOLLOW}$ sets **Top-Down** (starting with start symbol $S$, then examining where each non-terminal appears on the **Right-Hand Side** of productions).

---

### 12.3 Canonical Worked Example 1: Arithmetic Expression Grammar

$$E \to T E'$$
$$E' \to + T E' \mid \epsilon$$
$$T \to F T'$$
$$T' \to * F T' \mid \epsilon$$
$$F \to (E) \mid id$$

#### Step 1: FIRST Computation (Bottom-Up)
- $F \to (E) \mid id \implies \text{FIRST}(F) = \{ (, id \}$
- $T' \to *FT' \mid \epsilon \implies \text{FIRST}(T') = \{ *, \epsilon \}$
- $T \to FT' \implies \text{FIRST}(T) = \text{FIRST}(F) = \{ (, id \}$
- $E' \to +TE' \mid \epsilon \implies \text{FIRST}(E') = \{ +, \epsilon \}$
- $E \to TE' \implies \text{FIRST}(E) = \text{FIRST}(T) = \{ (, id \}$

#### Step 2: FOLLOW Computation (Top-Down)
- Start symbol $E$: Add $\$$ to $\text{FOLLOW}(E)$. Also, $F \to (E) \implies ')' \in \text{FOLLOW}(E)$.
  $$\text{FOLLOW}(E) = \{ ), \$ \}$$
- For $E'$: In $E \to TE'$, $E'$ is at the end $\implies \text{FOLLOW}(E') = \text{FOLLOW}(E) = \{ ), \$ \}$.
- For $T$: In $E \to TE'$ and $E' \to +TE'$:
  $$\text{FOLLOW}(T) = (\text{FIRST}(E') \setminus \{\epsilon\}) \cup \text{FOLLOW}(E') = \{ + \} \cup \{ ), \$ \} = \{ +, ), \$ \}$$
- For $T'$: In $T \to FT'$, $T'$ is at the end $\implies \text{FOLLOW}(T') = \text{FOLLOW}(T) = \{ +, ), \$ \}$.
- For $F$: In $T \to FT'$ and $T' \to *FT'$:
  $$\text{FOLLOW}(F) = (\text{FIRST}(T') \setminus \{\epsilon\}) \cup \text{FOLLOW}(T') = \{ * \} \cup \{ +, ), \$ \} = \{ *, +, ), \$ \}$$

#### Summary Table

| Non-Terminal | $\text{FIRST}$ | $\text{FOLLOW}$ |
|---|---|---|
| $E$ | $\{ (, id \}$ | $\{ ), \$ \}$ |
| $E'$ | $\{ +, \epsilon \}$ | $\{ ), \$ \}$ |
| $T$ | $\{ (, id \}$ | $\{ +, ), \$ \}$ |
| $T'$ | $\{ *, \epsilon \}$ | $\{ +, ), \$ \}$ |
| $F$ | $\{ (, id \}$ | $\{ *, +, ), \$ \}$ |

---

### 12.4 Tricky Exam Example 2: Cascade of $\epsilon$-Derivations (From Notes Page 16)

$$S \to ABCDE$$
$$A \to a \mid \epsilon$$
$$B \to b \mid \epsilon$$
$$C \to c$$
$$D \to d \mid \epsilon$$
$$E \to e \mid \epsilon$$

**Calculations**:
- $\text{FIRST}(A) = \{a, \epsilon\}$
- $\text{FIRST}(B) = \{b, \epsilon\}$
- $\text{FIRST}(C) = \{c\}$ (no $\epsilon$)
- $\text{FIRST}(D) = \{d, \epsilon\}$
- $\text{FIRST}(E) = \{e, \epsilon\}$
- $\text{FIRST}(S)$:
  - Since $A$ derives $\epsilon$, look at $B$.
  - Since $B$ derives $\epsilon$, look at $C$.
  - Since $C$ does NOT derive $\epsilon$, stop!
  $$\text{FIRST}(S) = (\text{FIRST}(A)\setminus\{\epsilon\}) \cup (\text{FIRST}(B)\setminus\{\epsilon\}) \cup \text{FIRST}(C) = \{a, b, c\}$$
- $\text{FOLLOW}(B)$:
  - From $S \to ABCDE$, $B$ is followed by $CDE$.
  - $\text{FIRST}(CDE) = \text{FIRST}(C) = \{c\}$ (since $C$ cannot be $\epsilon$).
  $$\text{FOLLOW}(B) = \{c\}$$
- $\text{FOLLOW}(D)$:
  - From $S \to ABCDE$, $D$ is followed by $E$.
  - $\text{FIRST}(E) = \{e, \epsilon\}$.
  - Since $E$ can derive $\epsilon$, $D$ also inherits $\text{FOLLOW}(S)$.
  $$\text{FOLLOW}(D) = (\text{FIRST}(E) \setminus \{\epsilon\}) \cup \text{FOLLOW}(S) = \{e, \$\}$$

---

## 13. LL(1) Predictive Parsing Algorithm

### 13.1 What Does LL(1) Mean?
- **L**: Scans input **L**eft-to-Right.
- **L**: Produces a **L**eftmost Derivation.
- **1**: Uses **1** single lookahead token of context to make deterministic decisions.

### 13.2 Formal LL(1) Disjointness Conditions

A context-free grammar $G$ is **LL(1)** if and only if for every pair of alternative productions:
$$A \to \alpha \mid \beta$$
All three of the following conditions hold simultaneously:
1. **$\text{FIRST}(\alpha) \cap \text{FIRST}(\beta) = \emptyset$**  
   *(They cannot begin with the same terminal token)*
2. **At most one of $\alpha$ or $\beta$ can derive $\epsilon$**.
3. **If $\beta \Rightarrow^* \epsilon$, then $\text{FIRST}(\alpha) \cap \text{FOLLOW}(A) = \emptyset$**.  
   *(If one alternative derives empty string, the other cannot start with any token that can legitimately follow $A$)*

---

### 13.3 Construction of the LL(1) Parsing Table $M[A, a]$

#### Algorithm
Input: Grammar $G = (V_N, V_T, P, S)$  
Output: Parsing table $M[A, a]$ where $A \in V_N$ and $a \in V_T \cup \{\$\}$.

For each production $A \to \alpha$ in $P$:
1. For each terminal $a \in \text{FIRST}(\alpha)$, add $A \to \alpha$ to $M[A, a]$.
2. If $\epsilon \in \text{FIRST}(\alpha)$, then:
   - For each terminal $b \in \text{FOLLOW}(A)$ (including $\$$), add $A \to \alpha$ to $M[A, b]$.
3. Set all remaining undefined entries in $M$ to `Error` (blank).

> $$\boxed{\textbf{GOLDEN RULE: } \mathbf{\epsilon\text{-productions are placed ONLY under the columns in } FOLLOW(A)!}}$$

#### Definition of an LL(1) Conflict
If any single table cell $M[A, a]$ receives **two or more distinct productions**, the grammar is **NOT LL(1)** (contains a FIRST/FIRST conflict or a FIRST/FOLLOW conflict).

---

### 13.4 Parsing Table for the Arithmetic Expression Grammar

From Section 12.3:
- $E \to TE'$ has $\text{FIRST}(TE') = \{id, (\}$
- $E' \to +TE'$ has $\text{FIRST} = \{+\}$
- $E' \to \epsilon$ has $\text{FOLLOW}(E') = \{), \$\}$
- $T \to FT'$ has $\text{FIRST}(FT') = \{id, (\}$
- $T' \to *FT'$ has $\text{FIRST} = \{*\}$
- $T' \to \epsilon$ has $\text{FOLLOW}(T') = \{+, ), \$\}$
- $F \to id$ has $\text{FIRST} = \{id\}$
- $F \to (E)$ has $\text{FIRST} = \{(\}$

| Non-Terminal | $id$ | $+$ | $*$ | $($ | $)$ | $\$$ |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| **$E$** | $E \to T E'$ | | | $E \to T E'$ | | |
| **$E'$** | | $E' \to + T E'$ | | | $E' \to \epsilon$ | $E' \to \epsilon$ |
| **$T$** | $T \to F T'$ | | | $T \to F T'$ | | |
| **$T'$** | | $T' \to \epsilon$ | $T' \to * F T'$ | | $T' \to \epsilon$ | $T' \to \epsilon$ |
| **$F$** | $F \to id$ | | | $F \to (E)$ | | |

*Because every cell contains at most one production, this grammar is strictly LL(1).*

---

### 13.5 Non-LL(1) Grammar Conflict Example (From Notes Page 18)

Consider:
$$S \to a S b S \mid b S a S \mid \epsilon$$
- $\text{FIRST}(S) = \{a, b, \epsilon\}$
- $\text{FOLLOW}(S) = \{\$, a, b\}$

Let us fill the row for $S$:
1. For production $S \to aSbS$, add to column $a$: $M[S, a] = \{ S \to aSbS \}$.
2. For production $S \to bSaS$, add to column $b$: $M[S, b] = \{ S \to bSaS \}$.
3. For production $S \to \epsilon$, add to all columns in $\text{FOLLOW}(S) = \{a, b, \$\}$:
   - $M[S, a]$ gets $S \to \epsilon$
   - $M[S, b]$ gets $S \to \epsilon$
   - $M[S, \$]$ gets $S \to \epsilon$

**Resulting Table Row**:

| Non-Terminal | $a$ | $b$ | $\$$ |
|:---:|:---:|:---:|:---:|
| **$S$** | $\mathbf{S \to aSbS} \quad / \quad \mathbf{S \to \epsilon}$ | $\mathbf{S \to bSaS} \quad / \quad \mathbf{S \to \epsilon}$ | $S \to \epsilon$ |

**Conclusion**: Cell $M[S, a]$ and Cell $M[S, b]$ both contain multiple entries. The parser cannot decide whether to expand $S \to aSbS$ or $S \to \epsilon$ on lookahead `a`.  
**Therefore, the grammar is NOT LL(1).**

---

### 13.6 Stack-Based LL(1) Simulation Engine & Trace

```
INPUT BUFFER:   [  a_1  ][  a_2  ][ ... ][  a_n  ][  $  ]
                              ▲
                              │ Input Pointer
                   ┌──────────┴──────────┐
                   │    PARSER ENGINE    │ ◄───► [ PARSING TABLE M ]
                   └──────────┬──────────┘
                              │
                    STACK:  [  X  ]  ◄── Top of Stack
                            [  Y  ]
                            [  $  ]
```

#### Algorithm Execution Rules
Let $X$ be the symbol currently at the top of the stack, and $a$ be the current lookahead token:
1. **Match**: If $X = a \neq \$$, pop $X$ from the stack and advance the input pointer to the next token.
2. **Accept**: If $X = a = \$$, halt and declare **successful parsing**.
3. **Expand**: If $X$ is a non-terminal:
   - Consult table entry $M[X, a]$.
   - If $M[X, a] = X \to Y_1 Y_2 \dots Y_k$:
     - Pop $X$ from the stack.
     - Push $Y_k, Y_{k-1}, \dots, Y_1$ onto the stack in **reverse order** (so that $Y_1$ is at the top).
   - If $M[X, a] = \text{Error}$, call error recovery routine.

#### Complete Step-by-Step Stack Trace for Input `id + id * id $`

| Step | Stack Contents | Current Input | Action Executed |
|:---:|:---|:---|:---|
| 1 | `[$ E]` | `id + id * id $` | Pop $E$, Push $T E'$ (from $M[E, id]$) |
| 2 | `[$ E' T]` | `id + id * id $` | Pop $T$, Push $F T'$ (from $M[T, id]$) |
| 3 | `[$ E' T' F]` | `id + id * id $` | Pop $F$, Push $id$ (from $M[F, id]$) |
| 4 | `[$ E' T' id]` | `id + id * id $` | **Match `id`** (Pop `id`, advance input) |
| 5 | `[$ E' T']` | `+ id * id $` | Pop $T'$, Push $\epsilon$ (from $M[T', +]$) |
| 6 | `[$ E']` | `+ id * id $` | Pop $E'$, Push $+ T E'$ (from $M[E', +]$) |
| 7 | `[$ E' T +]` | `+ id * id $` | **Match `+`** (Pop `+`, advance input) |
| 8 | `[$ E' T]` | `id * id $` | Pop $T$, Push $F T'$ (from $M[T, id]$) |
| 9 | `[$ E' T' F]` | `id * id $` | Pop $F$, Push $id$ (from $M[F, id]$) |
| 10 | `[$ E' T' id]` | `id * id $` | **Match `id`** (Pop `id`, advance input) |
| 11 | `[$ E' T']` | `* id $` | Pop $T'$, Push $* F T'$ (from $M[T', *]$) |
| 12 | `[$ E' T' F *]` | `* id $` | **Match `*`** (Pop `*`, advance input) |
| 13 | `[$ E' T' F]` | `id $` | Pop $F$, Push $id$ (from $M[F, id]$) |
| 14 | `[$ E' T' id]` | `id $` | **Match `id`** (Pop `id`, advance input) |
| 15 | `[$ E' T']` | `$` | Pop $T'$, Push $\epsilon$ (from $M[T', \$]$) |
| 16 | `[$ E']` | `$` | Pop $E'$, Push $\epsilon$ (from $M[E', \$]$) |
| 17 | `[$]` | `$` | **ACCEPT (Parsing Successful!)** |

---

# MODULE 3: High-Yield Midsem Exam Questions & Solutions

---

### Q1. Compiler Architecture & Phases (2-Mark Question from Last Midsem)
**Question**: "List the six phases of a compiler. Draw a diagram showing their interaction with the Symbol Table and Error Handler."

**Answer**:
The six phases are:
1. Lexical Analyzer (Scanner)
2. Syntax Analyzer (Parser)
3. Semantic Analyzer
4. Intermediate Code Generator
5. Code Optimizer
6. Target Code Generator

**Diagram**:
```
Source Code ──► [ Lexical Analyzer ] 
                       │
                       ▼
                [ Syntax Analyzer ]   ◄──────►  [ SYMBOL TABLE ]
                       │                         Stores variable types,
                       ▼                         scope, line declarations,
                [ Semantic Analyzer ]             and memory offsets.
                       │
                       ▼              ◄──────►  [ ERROR HANDLER ]
                [ Intermediate Code ]            Logs line numbers,
                       │                         recovers gracefully,
                       ▼                         and prevents crashes.
                [ Code Optimizer ]
                       │
                       ▼
                [ Code Generator ] ──► Target Assembly
```

---

### Q2. Identifying Operator Precedence & Associativity (5 Marks)
**Question**:
Given the context-free grammar:
$$E \to E \ \& \ T \mid T$$
$$T \to T \mid F \mid F$$
$$F \to G \text{ ** } F \mid G$$
$$G \to id \mid (E)$$

1. Identify the operators present and their precedence order.
2. Identify the associativity of each operator.
3. Show the parse tree for: `id & id | id ** id ** id`.

**Solution**:
1. **Operator Precedence**:
   - Level 1 (Start symbol $E$): Operator `&`
   - Level 2 (Non-terminal $T$): Operator `|`
   - Level 3 (Non-terminal $F$): Operator `**`
   - Level 4 (Non-terminal $G$): Operands
   - Rule: **Deeper non-terminal = higher precedence.**
   $$\boxed{\text{Precedence: } ** \ > \ | \ > \ \&}$$

2. **Associativity**:
   - $E \to E \ \& \ T \implies$ Left recursion $\implies \mathbf{\& \text{ is LEFT-associative}}$.
   - $T \to T \mid F \implies$ Left recursion $\implies \mathbf{| \text{ is LEFT-associative}}$.
   - $F \to G \text{ ** } F \implies$ Right recursion $\implies \mathbf{** \text{ is RIGHT-associative}}$.

3. **Evaluation Structure of `id & id | id ** id ** id`**:
   $$\text{Equivalent to: } \Big(id \ \& \ \big(id \mid (id \text{ ** } (id \text{ ** } id))\big)\Big)$$

---

### Q3. Thompson's Construction & Subset Construction (10 Marks)
**Question**: Construct a minimal DFA for the regular expression $r = (a \mid b)^* a b$.

**Solution Outline**:
1. **Thompson's Construction**:
   - Build $\epsilon$-NFA with start state $q_0$ looping on $(a \mid b)$, transitioning on $a$ to $q_1$, and on $b$ to accepting state $q_2$.
2. **Subset Construction**:
   - State $A = \epsilon\text{-closure}(q_0) = \{q_0\}$
   - On $a$: $\text{move}(\{q_0\}, a) = \{q_0, q_1\} = B$
   - On $b$: $\text{move}(\{q_0\}, b) = \{q_0\} = A$
   - State $B = \{q_0, q_1\}$:
     - On $a$: $\text{move}(\{q_0, q_1\}, a) = \{q_0, q_1\} = B$
     - On $b$: $\text{move}(\{q_0, q_1\}, b) = \{q_0, q_2\} = C$ (Final State)
   - State $C = \{q_0, q_2\}$:
     - On $a$: $\text{move}(\{q_0, q_2\}, a) = \{q_0, q_1\} = B$
     - On $b$: $\text{move}(\{q_0, q_2\}, b) = \{q_0\} = A$
3. **DFA Minimization**:
   - Partition $P_0 = \{(A, B), (C)\}$
   - State $A$ on $b \to A \in (A, B)$; State $B$ on $b \to C \in (C)$.
   - $A$ and $B$ split!
   - Resulting partition: $\{ (A), (B), (C) \}$. Already minimal (3 states).

---

### Q4. FIRST and FOLLOW with LL(1) Table Construction (10 Marks)
**Question**:
Given grammar:
$$S \to a A B b$$
$$A \to c \mid \epsilon$$
$$B \to d \mid \epsilon$$

1. Calculate $\text{FIRST}$ and $\text{FOLLOW}$ sets for all non-terminals.
2. Construct the LL(1) Parsing Table.
3. Is the grammar LL(1)? Justify.

**Solution**:
1. **Sets**:
   - $\text{FIRST}(A) = \{c, \epsilon\}$
   - $\text{FIRST}(B) = \{d, \epsilon\}$
   - $\text{FIRST}(S) = \{a\}$
   - $\text{FOLLOW}(S) = \{\$\}$
   - $\text{FOLLOW}(A)$: From $S \to a A B b$, $A$ is followed by $Bb$.
     $$\text{FIRST}(Bb) = (\text{FIRST}(B) \setminus \{\epsilon\}) \cup \{b\} = \{d, b\}$$
     $$\text{FOLLOW}(A) = \{d, b\}$$
   - $\text{FOLLOW}(B)$: From $S \to a A B b$, $B$ is followed by $b$.
     $$\text{FOLLOW}(B) = \{b\}$$

2. **Parsing Table Construction**:
   - Production $S \to aABb$: $\text{FIRST}(aABb) = \{a\} \implies M[S, a] = S \to aABb$
   - Production $A \to c$: $\text{FIRST}(c) = \{c\} \implies M[A, c] = A \to c$
   - Production $A \to \epsilon$: $\text{FOLLOW}(A) = \{b, d\} \implies M[A, b] = A \to \epsilon, \ M[A, d] = A \to \epsilon$
   - Production $B \to d$: $\text{FIRST}(d) = \{d\} \implies M[B, d] = B \to d$
   - Production $B \to \epsilon$: $\text{FOLLOW}(B) = \{b\} \implies M[B, b] = B \to \epsilon$

| Non-Terminal | $a$ | $b$ | $c$ | $d$ | $\$$ |
|:---:|:---:|:---:|:---:|:---:|:---:|
| **$S$** | $S \to a A B b$ | | | | |
| **$A$** | | $A \to \epsilon$ | $A \to c$ | $A \to \epsilon$ | |
| **$B$** | | $B \to \epsilon$ | | $B \to d$ | |

3. **LL(1) Verification**:
   - Look at the parsing table: Every cell contains **at most one production entry**.
   - No cell contains multiple entries / conflicts.
   - **Conclusion: Yes, the grammar is strictly LL(1).**

---

### Q5. Left Recursion Elimination (Direct and Indirect) (5 Marks)
**Question**: Eliminate left recursion from:
$$S \to A a \mid b$$
$$A \to A c \mid S d \mid \epsilon$$

**Solution**:
1. Order non-terminals: $S, A$.
2. For $S$: No direct left recursion ($S \to A a \mid b$).
3. For $A$: Production $A \to S d$ has $S$ preceding $A$. Substitute $S \to A a \mid b$ into $A \to S d$:
   $$A \to A c \mid (A a \mid b) d \mid \epsilon$$
   $$A \to A c \mid A a d \mid b d \mid \epsilon$$
4. Group left-recursive and non-left-recursive productions for $A$:
   - Left-recursive terms: $A (c \mid ad)$ ($\alpha_1 = c, \alpha_2 = ad$)
   - Non-left-recursive terms: $\beta_1 = bd, \beta_2 = \epsilon$
5. Apply formula:
   $$\boxed{\begin{aligned}
   A &\to b d A' \mid A' \\
   A' &\to c A' \mid a d A' \mid \epsilon
   \end{aligned}}$$

---

### Q6. Recursive Descent Parsing Procedural Question (5 Marks)
**Question**: "Explain the structure of a Recursive Descent Parser. Write the C code parsing procedures for:
$$A \to a B \mid c$$
$$B \to b A \mid \epsilon$$
assuming `match()` and `lookahead` are provided."

**Solution**:
```c
void B();

void A() {
    if (lookahead == 'a') {
        match('a');
        B();
    } else if (lookahead == 'c') {
        match('c');
    } else {
        printf("Syntax Error in A at %c\n", lookahead);
        exit(1);
    }
}

void B() {
    if (lookahead == 'b') {
        match('b');
        A();
    } else if (lookahead == '$' || lookahead == ')' || lookahead == '\n') {
        /* epsilon production: follow of B check */
        return;
    } else {
        printf("Syntax Error in B at %c\n", lookahead);
        exit(1);
    }
}
```

---

## 💡 EXAM-DAY MEMORY SUMMARY & CHEAT CARD

```
1. LPS PIPELINE:
   Source -> Preprocessor -> Compiler -> Assembler -> Linker/Loader -> Target

2. COMPILER PHASES:
   Lexical -> Syntax -> Semantic -> ICG -> Optimization -> Target Gen
   (Interacting with Symbol Table and Error Handler)

3. THOMPSON'S PROPERTIES:
   - 1 start state, 1 final state
   - States <= 2 * length(r)
   - Max 2 epsilon transitions per state

4. MINIMIZATION:
   - Partition: P_0 = { F, Q \ F } -> refine until stable
   - Table-filling: Mark (p, q) with X if one in F, other not in F; inductively mark

5. OPERATOR PRECEDENCE:
   - DEEPER non-terminal = HIGHER precedence

6. ASSOCIATIVITY:
   - A -> A op B  =>  LEFT-associative (left recursion)
   - A -> B op A  =>  RIGHT-associative (right recursion)

7. DIRECT LEFT RECURSION:
   A -> A alpha | beta   ===>   A -> beta A'
                                A' -> alpha A' | epsilon

8. LEFT FACTORING:
   A -> alpha beta_1 | alpha beta_2 | gamma  ===>  A -> alpha A' | gamma
                                                   A' -> beta_1 | beta_2

9. FIRST & FOLLOW:
   - epsilon is NEVER in any FOLLOW set!
   - $ is ALWAYS in FOLLOW(S)
   - epsilon-productions go ONLY into cells M[A, b] where b in FOLLOW(A)

10. LL(1) CONFLICT:
    - Multiple productions in the same cell => NOT LL(1)
```
