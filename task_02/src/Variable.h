//
// Created by vovan on 29.09.2019.
//

#ifndef LECS_PARS_VARIABLE_H
#define LECS_PARS_VARIABLE_H

#include <string>

class Variable {
public:
    explicit Variable(std::string t_type, std::string t_value) : type(t_type),
                                                                 value(t_value) {};
    std::string type;
    std::string value;
};


#endif //LECS_PARS_VARIABLE_H
