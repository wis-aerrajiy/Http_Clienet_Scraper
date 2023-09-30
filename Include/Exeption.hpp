#pragma once

#include <exception>
#include <iostream>
#include <unistd.h>

namespace Exeption {
    class   HttpExption : public std::exception {
        private :
            std::string *ExeptionErrorMessage;
        public :
            HttpExption(std::string);
            ~HttpExption();
            virtual const char * what() const throw();
    };
};