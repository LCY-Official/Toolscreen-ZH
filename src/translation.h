#pragma once

#include <format>
#include <string>

bool LoadTranslations(const std::string& lang);

std::string tr(const char* key);
inline std::string tr(const std::string& key) { return tr(key.c_str()); }

template <typename... Args>
inline std::string ftr(const char* key, Args&&... args) {
    return std::format(tr(key), std::forward<Args>(args)...);
}

template <typename... Args>
inline std::string ftr(const std::string& key, Args&&... args) {
    return ftr(key.c_str(), std::forward<Args>(args)...);
}
