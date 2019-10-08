//
// Created by vovan on 28.09.2019.
//

#ifndef LECS_PARS_SYNTAX_H
#define LECS_PARS_SYNTAX_H

#include <algorithm>
#include <iostream>
#include <chrono>
#include <list>
#include <map>
#include <vector>
#include "Lexem.h"
#include "Variable.h"

typedef struct tree_t {  // TODO: Here you have to rewrite to OOP
    tree_t *left_node;
    tree_t *rigth_node;
    tree_t *parent_node; // XXX: Should we have/use link to parent node?
    std::string value;
} tree_t;


class Syntax {
public:
    explicit Syntax(std::vector<Lexem> &&t_lex_table);
    int ParseCode();
    ~Syntax();
private:
    using lex_it = std::vector<Lexem>::iterator; // alias of vector iterator
    lex_it                          cursor;
    std::vector<Lexem>              lex_table;   // out table of lexemes
    std::map<std::string, Variable> id_map;      // our table of identifiers
    tree_t                         *pascal_tree; // our syntax tree

    lex_it getNextLex(lex_it &iter);
    lex_it peekLex(int N, lex_it t_iter);

    tree_t *buildTreeStub(tree_t *t_tree, const std::string &node_name);
    tree_t *createNode(const std::string &node_name);

    int   expressionParse            (lex_it &t_iter);
    int   stateParse                 (lex_it &t_iter);
    int   compoundParse              (lex_it &t_iter);
    int   vardpParse                 (lex_it &t_iter, tree_t *t_tree);
    std::list<std::string> vardParse (lex_it &t_iter);
    int   blockParse                 (lex_it &t_iter);
    int   programParse               (lex_it &t_iter);

    void printError    (errors t_err, Lexem lex);
    bool checkLexem    (const lex_it &t_iter, const tokens &t_tok);
    bool isVarExist    (const std::string &t_var_name);
    void updateVarTypes(const std::list<std::string> &t_var_list,
                        const std::string &t_type_name);
    void buildVarTree  (const std::list<std::string> &t_var_list, tree_t *t_tree);

    void createVarTree(tree_t *t_tree, tree_t *t_donor_tree, int lvl);
    void freeTreeNode (tree_t *&t_tree);
};


#endif //LECS_PARS_SYNTAX_H
