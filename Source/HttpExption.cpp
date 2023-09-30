#include "../Include/Exeption.hpp"

Exeption::HttpExption::HttpExption(std::string ErrorMessage) {
    this->ExeptionErrorMessage = new std::string(ErrorMessage);
}

const char * Exeption::HttpExption::what() const throw() {
    return (this->ExeptionErrorMessage->c_str());
}

Exeption::HttpExption::~HttpExption() {
    delete this->ExeptionErrorMessage;
}