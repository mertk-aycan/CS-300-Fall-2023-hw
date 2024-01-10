#include "strutils.h"
#include <cctype>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <algorithm>
using namespace std;

void ToLower(string& s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void ToUpper(string& s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
}

string UpperString(const string& s) {
    string word = s;
    ToUpper(word);
    return word;
}

string LowerString(const string& s) {
    string word = s;
    ToLower(word);
    return word;
}

void StripPunc(string& word) {
    word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
}

void StripWhite(string& word) {
    auto start = word.find_first_not_of(" \t\n\r\f\v");
    auto end = word.find_last_not_of(" \t\n\r\f\v");
    word = (start == string::npos) ? "" : word.substr(start, end - start + 1);
}

string itoa(int n) {
    ostringstream output;
    output << n;
    return output.str();
}

string tostring(int n) {
    return itoa(n);
}

string tostring(double n) {
    ostringstream output;
    output << n;
    return output.str();
}

int atoi(const string& s) {
    return std::atoi(s.c_str());
}

double atof(const string& s) {
    return std::atof(s.c_str());
}
