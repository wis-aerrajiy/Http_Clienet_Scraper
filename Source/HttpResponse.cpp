#include "../Include/Http.hpp"
#include "../Include/Exeption.hpp"

HTTP::HttpResponse::HttpResponse()
{
    this->ResponseInfo = new HTTP::t_ResponseInfo;
    this->ResponseInfo->ContentLenght = -1;
    this->headerParsed = false;
}

HTTP::HttpResponse::~HttpResponse()
{
}

HTTP::ContentType   HTTP::HttpResponse::ContentTypeDefine() {
    if (!this->headersMap["Content-Length"].empty()) {
        this->ResponseInfo->ContentLenght = std::stoi(this->headersMap["Content-Length"]);
        return (HTTP::ContentType::ContentLent);
    }
    else if (this->headersMap["Transfer-Encoding"].find("chunked") != std::string::npos) {
        return (HTTP::ContentType::Chunked);
    } else {
        throw Exeption::HttpExption("Program Ended Becouse we get a bad response");
    }
    return (HTTP::ContentType::ContentLent);
}

void HTTP::HttpResponse::HeaderParser(std::string headers)
{
    std::string key, value, line;
    size_t endLine, findDot;
    line = headers.substr(0, headers.find("\r\n"));
    std::cout << "Line : " << line << std::endl;
    headers.erase(0, headers.find("\r\n") + 2);
    while (headers.size())
    {
        findDot = headers.find(":");
        endLine = headers.find("\r\n");
        key = headers.substr(0, findDot);
        value = headers.substr(findDot + 1, endLine - findDot);

        std::cout << key << " :|: " << value << std::endl;
        this->headersMap[key] = value;
        headers.erase(0, endLine + 2);
    }
    this->ResponseInfo->Type = this->ContentTypeDefine();
}

int HTTP::HttpResponse::headerPart(std::string response)
{
    size_t _pos;
    if ((_pos = response.find("\r\n\r\n")) != std::string::npos)
    {
        this->header += response.substr(0, _pos + 2);
        this->BodyResponsePath = response.substr(_pos + 4, response.length() - _pos + 4);
        this->headerParsed = true;
        this->HeaderParser(this->header);
        return 0;
    }
    this->header += response;
    return 0;
}

bool    HTTP::HttpResponse::ParsResponse(std::string response) {
    if (!this->headerParsed)
        this->headerPart(response);
    else {
        std::cout << "The start of Body : " << (int) this->ResponseInfo->Type << std::endl;
        exit(12);
    }

    return (true);
}