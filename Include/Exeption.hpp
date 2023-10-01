#ifndef _EXEPTION_HTTP
#define _EXEPTION_HTTP 1

#pragma GCC system_header

# include <exception>
# include <iostream>
# include <unistd.h>
# include <openssl/ssl.h>
# include <openssl/err.h>

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

#endif