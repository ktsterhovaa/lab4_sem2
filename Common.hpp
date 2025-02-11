#pragma once


#include <algorithm>
#include <string>


#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(*(a)))
#endif


class IndexOutOfRange : public std::exception {
    std::string message_;

public:
    explicit IndexOutOfRange(const std::string &message) : message_(message) {}

    explicit IndexOutOfRange(const char *message) : message_(message) {}

    const char *what() const noexcept override {
        return message_.c_str();
    }
};


static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}


static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline std::string trim_copy(std::string s) {
    ltrim(s);
    rtrim(s);
    return s;
}
