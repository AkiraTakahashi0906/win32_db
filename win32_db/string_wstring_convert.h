#ifndef STRING_WSTRING_CONVERT_HPP
#define STRING_WSTRING_CONVERT_HPP

#pragma once
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <string>
#include <windows.h>

class string_wstring_convert {
public:
	static std::wstring StringToWString(std::string oString);
};

#endif

