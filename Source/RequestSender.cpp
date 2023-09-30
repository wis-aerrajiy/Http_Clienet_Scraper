#include "../Include/Http.hpp"
#include "../Include/Exeption.hpp"

HTTP::RequestSender::RequestSender(HttpRequest *Connection)
{
    this->HttpConnection = Connection;
}

void HTTP::RequestSender::NonBodyMethod()
{
    
    std::string getRequest = "GET " + std::string("/") + " HTTP/1.1\r\nHost: " + this->HttpConnection->getRequestUrlInfo()->Host + "\r\n\r\n";
    std::cout << getRequest << std::endl;
    if (send(this->HttpConnection->getSocketFd(), getRequest.c_str(), getRequest.size(), 0) == -1)
        throw Exeption::HttpExption("Can Send Request to the Host");
}

void HTTP::RequestSender::ResponseManager() {
    char *buffer =  new char[1024]();
    memset(buffer, 0, 1024);
    std::cout << "Here data" << std::endl;
    while (recv(this->HttpConnection->getSocketFd(), buffer, 1024 - 1, 0) > 0) {
        std::cout << "Here" << std::endl;
        std::cout << buffer;
        memset(buffer, 0, 1024);
    }
    delete buffer;
}