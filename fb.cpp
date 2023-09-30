#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int main() {
    const char* hostname = "www.youtube.com";
    const char* port = "443"; // HTTPS port
    const char* path = "/watch?v=Nq4Mh_jTubA";   // Path to request

    // Initialize OpenSSL
    SSL_library_init();
    SSL_CTX* ctx = SSL_CTX_new(SSLv23_client_method());

    if (!ctx) {
        std::cerr << "OpenSSL initialization failed." << std::endl;
        return 1;
    }

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Resolve the hostname to an IP address
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        close(sockfd);
        return 1;
    }

    // Connect to the remote server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Connection failed");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    // Set up the SSL connection
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    if (SSL_connect(ssl) != 1) {
        perror("SSL connection failed");
        SSL_free(ssl);
        close(sockfd);
        freeaddrinfo(res);
        SSL_CTX_free(ctx);
        return 1;
    }

    // Build the HTTPS GET request
    std::string request = "GET " + std::string(path) + " HTTP/1.1\r\n";
    request += "Host: " + std::string(hostname) + "\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";

    // Send the request over SSL
    if (SSL_write(ssl, request.c_str(), request.length()) <= 0) {
        perror("SSL_write failed");
    }

    // Receive and print the response
    char buffer[1024];
    int bytes_received;
    while ((bytes_received = SSL_read(ssl, buffer, sizeof(buffer))) > 0) {
        std::cout.write(buffer, bytes_received);
    }

    // Cleanup and close the SSL and socket
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    freeaddrinfo(res);
    SSL_CTX_free(ctx);

    return 0;
}