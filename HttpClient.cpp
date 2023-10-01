#include "Include/Http.hpp"

int main()
{
    try {
        HTTP::HttpRequest *Client = new HTTP::HttpRequest("http://example.com");
        Client->SendHttpRequest();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
