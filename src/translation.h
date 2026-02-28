#pragma once

#include <format>
#include <string>

bool LoadTranslations(const std::string& lang);

std::string tr(const char* key);
std::string tr(const std::string& key);
