#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <exception>
#include <regex>
#include <unistd.h>

namespace HTTP {
    class HttpRequest;
    class RequestSender;

    typedef struct s_HostInfo {
        std::string Host;
        std::string Protocol;
        size_t      
    }   t_HostInfo;

    class RequestSender {
        private :
            std::string ResponseFilesStore;
            HttpRequest *HttpConnection;
        public :
            RequestSender(HTTP::HttpRequest *connection);
            void    NonBodyMethod();
            void    ResponseManager();
            // void    BodyMethod();
    };

    class HttpRequest {
        private :
            std::string     RequestUrl;
            t_HostInfo      *RequestUrlInfo;
            struct addrinfo dataToresolve;
            struct addrinfo *resolvedData;
            HTTP::RequestSender   *RequestProcess;
            int             socketFd;
    
        public :
            HttpRequest();
            HttpRequest(std::string url);
            HttpRequest(HttpRequest const &);
            HttpRequest & operator=(HttpRequest const &);

            // Make Connection with the server
            void    RequestUrlChecker();
            void    ResolveData(int __ai_family, int    ai_socktype);
            void    CreateSocket(int __domain, int __type);
            void    SocketConnection();
            void    SendHttpRequest();
 
            // getter
            int getSocketFd() const;
            t_HostInfo *getRequestUrlInfo() const;

            ~HttpRequest();
    };
}