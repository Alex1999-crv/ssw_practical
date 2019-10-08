# task_02
Simple example of lexical and syntax analyzer. With building AST (*abstract syntax tree*).

### What you have to do:
- add operations of '*'/div/()/-value (i.e. a := -3);
- add operators precedence check;
- add error recovery strategy;
- rewrite tree structure (OOP style);
- (optional, +2) implement the tree through smart pointers;
- (optional, +1) put the creation of nodes into separate tree entity methods;
- implement all 'TODO' tips (start with `TODO: ...`);


### Build
*Note:* all instructions here for machine with *nix OS. For Windows users may be need some another instructions (you can try to use `Cygwin` or just change `g++` on `mingw`).

To build by make (Makefile):
```
$ cd <to repo path>/task_02
$ make
```

To build by cmake (CMakeLists.txt):
```
$ cd <to repo path>/task_02
$ cmake .
$ make
```
