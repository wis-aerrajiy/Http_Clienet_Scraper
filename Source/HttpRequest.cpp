#include "../Include/Http.hpp"
#include "../Include/Exeption.hpp"

HTTP::HttpRequest::HttpRequest()
{
    this->RequestProcess = NULL;
    this->RequestUrlInfo = NULL;
}

HTTP::HttpRequest::HttpRequest(std::string url)
{
    this->RequestUrl = url;
    this->RequestUrlInfo = new HTTP::t_HostInfo;
    this->resolvedData = new addrinfo;
    SSL_library_init();
    this->ctx = SSL_CTX_new(SSLv23_client_method());
    try
    {
        this->RequestUrlChecker();
        this->ResolveData(AF_INET, SOCK_STREAM);
        this->CreateSocket(AF_INET, SOCK_STREAM);
        this->SocketConnection();
        this->RequestProcess = new HTTP::RequestSender(this);
        
    }
    catch (const std::exception &e)
    {
        std::cout << "error" << std::endl;
        throw Exeption::HttpExption("Error Detected In HttpRequest : " + std::string(e.what()));
    }
}

HTTP::HttpRequest::HttpRequest(HttpRequest const &copy)
{
    *this = copy;
}

HTTP::HttpRequest &HTTP::HttpRequest::operator=(HttpRequest const &copy)
{
    this->RequestUrl = copy.RequestUrl;
    return *this;
}

void HTTP::HttpRequest::CreateSocket(int __domain, int __type)
{
    int socketFd;
    if ((socketFd = socket(__domain, __type, 0)) < 0)
        throw Exeption::HttpExption("Error creating socket");
    this->socketFd = socketFd;
}

void HTTP::HttpRequest::RequestUrlChecker()
{
    size_t i;
    std::regex urlPattern("^(http|https)://[a-zA-Z0-9.-]+$");
    if (!this->RequestUrl.empty())
    {
        if (!std::regex_match(this->RequestUrl, urlPattern))
            throw Exeption::HttpExption("RequestUrl Should be Something Like `Http[s]://host` ");
        i = this->RequestUrl.find("://");
        this->RequestUrlInfo->Protocol = (this->RequestUrl.substr(0, i));
        this->RequestUrlInfo->Host = (this->RequestUrl.substr(i + 3, (this->RequestUrl.length() - (i + 3))));
        if (this->RequestUrlInfo->Protocol == "http")
            this->RequestUrlInfo->Port = "80";
        else
            this->RequestUrlInfo->Port = "443";
    }
    else
        throw Exeption::HttpExption("RequestUrl can't be empty()");
}

void HTTP::HttpRequest::ResolveData(int __ai_family, int __ai_socktype)
{
    memset(&this->dataToresolve, 0, sizeof(this->dataToresolve));
    this->dataToresolve.ai_family = __ai_family;
    this->dataToresolve.ai_socktype = __ai_socktype;

    if (getaddrinfo(this->RequestUrlInfo->Host.c_str(), this->RequestUrlInfo->Port.c_str(), &this->dataToresolve, &this->resolvedData))
        throw Exeption::HttpExption("The Host Can be resolved");
}

void HTTP::HttpRequest::SocketConnection()
{
    if (connect(this->socketFd, this->resolvedData->ai_addr, this->resolvedData->ai_addrlen) == -1)
        throw Exeption::HttpExption("Error connecting to server");
    std::cout << "Connection to " << this->RequestUrlInfo->Protocol + "://" + this->RequestUrlInfo->Host << " Done" << std::endl;
}

void HTTP::HttpRequest::SendHttpRequest()
{
    this->RequestProcess->NonBodyMethod();
    this->RequestProcess->ResponseManager();
}

HTTP::HttpRequest::~HttpRequest()
{
    delete this->RequestUrlInfo;
    delete this->RequestProcess;
    close(this->socketFd);
    SSL_CTX_free(ctx);
}

int HTTP::HttpRequest::getSocketFd() const
{
    return (this->socketFd);
}

HTTP::t_HostInfo *HTTP::HttpRequest::getRequestUrlInfo() const
{
    return (this->RequestUrlInfo);
}

SSL_CTX *HTTP::HttpRequest::getOpenSslObj() const
{
    return (this->ctx);
}