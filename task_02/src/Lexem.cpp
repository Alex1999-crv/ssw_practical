//
// Created by vovan on 28.09.2019.
//

#include "Lexem.h"


/**
 * @brief Return name of lexeme
 * @param none
 *
 * @return name of lexeme
 */
std::string Lexem::GetName() {
    return name;
}


/**
 * @brief Return type of lexeme
 * @param none
 *
 * @return type (token) of lexeme
 */
tokens Lexem::GetToken() {
    return token;
}


/**
 * @brief Return line (from pascal file) of lexeme
 * @param none
 *
 * @return line of lexeme
 */
int Lexem::GetLine() {
    return line;
}