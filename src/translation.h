#pragma once

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include <string>

bool LoadTranslations(const std::string& lang);

std::string tr(const char* key);

inline char* trc(const char* key) {
    std::string s   = tr(key);
    char*       ret = new char[s.size() + 1];
    strcpy(ret, s.c_str());
    return ret;
}

template <typename... Args>
inline std::string ftr(const char* key, Args&&... args) {
    return fmt::format(fmt::runtime(tr(key)), std::forward<Args>(args)...);
}

template <typename... Args>
inline char* ftrc(const char* key, Args&&... args) {
    std::string s   = ftr(key, std::forward<Args>(args)...);
    char*       ret = new char[s.size() + 1];
    strcpy(ret, s.c_str());
    return ret;
}
