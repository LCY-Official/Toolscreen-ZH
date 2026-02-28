#include "translation.h"

#include <nlohmann/json.hpp>

#include "utils.h"

inline nlohmann::json g_translations;

bool LoadTranslations(const std::string& lang) {
    std::string langResName = "lang_" + lang;
    try {
        HMODULE hModule = nullptr;
        GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&RenderWelcomeToast, &hModule);
        if (!hModule) {
            throw std::exception("GetModuleHandleExW failed");
        }

        HRSRC resSrc = FindResourceA(hModule, langResName.c_str(), MAKEINTRESOURCEA(10));
        if (!resSrc) {
            throw std::exception("FindResourceA failed");
        }

        HGLOBAL resHandle = LoadResource(hModule, resSrc);
        if (!resHandle) {
            throw std::exception("LoadResource failed");
        }

        auto* const resPtr = LockResource(resHandle);
        if (!resPtr) {
            throw std::exception("LockResource failed");
        }

        const auto resSize = SizeofResource(hModule, resSrc);
        if (resSize == 0) {
            throw std::exception("SizeofResource failed");
        }

        g_translations = nlohmann::json::parse((const char*)resPtr, (const char*)resPtr + resSize);
    } catch (const std::exception& e) {
        Log("Failed to load translations of " + lang + ": " + e.what());
        return false;
    }
    return true;
}

std::string tr(const char* key) {
    const auto translation = g_translations[key].get<std::string>();
    return translation.empty() ? key : translation;
}

std::string tr(const std::string& key) { return tr(key.c_str()); }
