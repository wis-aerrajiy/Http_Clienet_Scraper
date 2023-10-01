#include "../Include/Http.hpp"
#include "../Include/Exeption.hpp"

HTTP::RequestSender::RequestSender(HttpRequest *Connection)
{
    this->sslConnection = SSL_new(Connection->getOpenSslObj());
    if (Connection->getRequestUrlInfo()->Protocol == "https")
        SSL_set_fd(this->sslConnection, Connection->getSocketFd());
    this->HttpConnection = Connection;
    if (this->HttpConnection->getRequestUrlInfo()->Protocol == "https")
    {
        if (SSL_connect(this->sslConnection) != 1)
            throw Exeption::HttpExption("SSL connection failed");
    }
}

void HTTP::RequestSender::SendHttpHttpsRequest(std::string Request)
{
    if (this->HttpConnection->getRequestUrlInfo()->Protocol == "https")
    {
        if (SSL_write(this->sslConnection, Request.c_str(), Request.length()) <= 0)
            throw Exeption::HttpExption("Can Send Request to the Host");
    }
    else
    {
        if (send(this->HttpConnection->getSocketFd(), Request.c_str(), Request.size(), 0) == -1)
            throw Exeption::HttpExption("Can Send Request to the Host");
    }
}

std::pair<int, std::string> HTTP::RequestSender::reciveHttpHttpsResponse()
{
    std::string response;
    char buffer[1024];
    int bytes_received;
    if (this->HttpConnection->getRequestUrlInfo()->Protocol == "https")
    {
        bytes_received = SSL_read(this->sslConnection, buffer, sizeof(buffer));
        response.append(buffer, bytes_received);
    }
    else
    {
        bytes_received = recv(this->HttpConnection->getSocketFd(), buffer, sizeof(buffer), 0);
        response.append(buffer, bytes_received);
    }
    return (std::make_pair(bytes_received, response));
}

void HTTP::RequestSender::NonBodyMethod()
{
    std::string Request = "GET " + std::string("/") + " HTTP/1.1\r\nHost: " + this->HttpConnection->getRequestUrlInfo()->Host + "\r\n\r\n";
    // std::cout << Request << std::endl;
    SendHttpHttpsRequest(Request);
}

void HTTP::RequestSender::ResponseManager()
{
    static int size = 0;
    this->Response = new HTTP::HttpResponse();
    std::pair<int, std::string> response;
    while (1)
    {
        FD_ZERO(&this->read_set);
        FD_SET(this->HttpConnection->getSocketFd(), &this->read_set);
        int status = select(this->HttpConnection->getSocketFd() + 1, &this->read_set, NULL, NULL, NULL);
        if (status < 0)
            throw Exeption::HttpExption("Error Detected while waiting response");
        response = this->reciveHttpHttpsResponse();
        if (response.first <= 0)
            throw Exeption::HttpExption("Error Detected while waiting response");
        size += response.first;
        std::cout << "Read ... : " << size << " | " << response.first << std::endl;
        this->Response->ParsResponse(response.second);
    }
}

HTTP::RequestSender::~RequestSender()
{
    SSL_shutdown(this->sslConnection);
    SSL_free(this->sslConnection);
}