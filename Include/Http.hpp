#ifndef _HTTP_HTTP
#define _HTTP_HTTP 1

#pragma GCC system_header

# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <openssl/ssl.h>
# include <openssl/err.h>
# include <regex>
# include <vector>
# include <map>
# include <utility>
# include <sys/select.h>
# include <unistd.h>
# include <fcntl.h>

namespace HTTP {
    class HttpRequest;
    class RequestSender;

    typedef struct s_HostInfo {
        std::string Host;
        std::string Protocol;
        std::string Port;
    }   t_HostInfo;

    class   HttpResponse {
        private :
            bool        headerParsed;
            std::string header;
            std::map <std::string, std::string> headersMap;
            std::string BodyResponsePath;
            std::string subBodySave;
            void        HeaderParser(std::string);
            int         headerPart(std::string);
        public :
            HttpResponse();
            ~HttpResponse();
            bool        ParsResponse(std::string);
    };

    class   RequestSender {
        private :
            HttpResponse    *Response;
            HttpRequest     *HttpConnection;
            SSL             *sslConnection;
            fd_set          read_set;
        public :
            RequestSender(HTTP::HttpRequest *connection);
            ~RequestSender();
            void    NonBodyMethod();
            void    ResponseManager();
            void    SendHttpHttpsRequest(std::string request);
            std::pair <int, std::string> reciveHttpHttpsResponse();
            // void    BodyMethod();
    };

    class   HttpRequest {
        private :
            std::string             RequestUrl;
            t_HostInfo              *RequestUrlInfo;
            struct addrinfo         dataToresolve;
            struct addrinfo         *resolvedData;
            HTTP::RequestSender     *RequestProcess;
            SSL_CTX                 *ctx;
            int                     socketFd;

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
            SSL_CTX *getOpenSslObj() const;

            ~HttpRequest();
    };
}

#endif