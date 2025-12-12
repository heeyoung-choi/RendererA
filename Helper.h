#pragma once
#include "Common.h"
#include <string>
#include <locale>
#include <codecvt>

static class Helper
{

public:
    static std::string wstring_to_string(const std::wstring& wideStr) {
        if (wideStr.empty()) return std::string();

        // 1. Ask Windows: "How big is the result string?"
        // CP_UTF8 = Convert to standard UTF-8 (Recommended for cross-platform compatibility)
        // CP_ACP  = Convert to system ANSI codepage (Use only for legacy Windows APIs)
        int size_needed = WideCharToMultiByte(
            CP_UTF8, 0, 
            wideStr.c_str(), (int)wideStr.length(), 
            NULL, 0, NULL, NULL
        );

        // 2. Allocate the string buffer
        std::string byteStr(size_needed, 0);

        // 3. Perform the actual conversion
        WideCharToMultiByte(
            CP_UTF8, 0, 
            wideStr.c_str(), (int)wideStr.length(), 
            &byteStr[0], size_needed, NULL, NULL
        );

        return byteStr;
    }
    static std::wstring string_to_wstring(std::string& byteStr)
    {
        if (byteStr.empty()) return std::wstring();

        // 1. Ask Windows: "How big is the result string?"
        int size_needed = MultiByteToWideChar(
            CP_UTF8, 0,
            byteStr.c_str(), (int)byteStr.length(),
            NULL, 0
        );

        // 2. Allocate the wide string buffer
        std::wstring wideStr(size_needed, 0);

        // 3. Perform the conversion
        MultiByteToWideChar(
            CP_UTF8, 0,
            byteStr.c_str(), (int)byteStr.length(),
            &wideStr[0], size_needed
        );

        return wideStr;
    }

};