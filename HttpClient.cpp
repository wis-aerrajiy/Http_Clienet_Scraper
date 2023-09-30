#include "Include/Http.hpp"

int main()
{
    HTTP::HttpRequest *Client = new HTTP::HttpRequest("https://m.facebook.com");
    Client->SendHttpRequest();
    return 0;
}
