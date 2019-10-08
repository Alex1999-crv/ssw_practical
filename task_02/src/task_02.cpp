#include "task_02.h"


/**
 * Given grammar:
 * <Soft>        ::= program <id> ; <block> .
 * <block>       ::= <var part> <state part>
 * <var part>    ::= var <var dec> : <type> ;
 * <var dec>     ::= <id> { , <var dec> }
 * <state part>  ::= <compound>
 * <compound>    ::= begin <state> { ; <state> } end
 * <state>       ::= <assign> | <compound> | <your_other_operations>
 * <assign>      ::= <id> := <exp> ;
 * <exp>         ::= <id> | <constant>
 * <type>        ::= integer
 * <id>          ::= a-z
 * <constant>    ::= 0-9
 */


int Parse3(const std::string& file_path) {
    std::cout << "\tPart 3" << std::endl;

    Lexer lex(file_path.c_str());
    auto table = lex.ScanCode(); // create table of lexemes

    Syntax syntx(std::move(table));
    syntx.ParseCode();          // start parse code

    return EXIT_SUCCESS;
}
