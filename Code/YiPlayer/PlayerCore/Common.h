#pragma once
using namespace std;
using namespace System;
char* FromStringHandleToAnsi(const String^ str);
wchar_t* FromStringHandleToUnicode(const String^ str);
wchar_t* FromAnsiToUnicode(const char* str);
void Wchar_tToString(std::string& szDst, wchar_t *wchar);
std::string FromAnsiToUtf8(const char* str);
std::wstring FromUtf8ToUnicode(const char* str);
std::string Unicode2Utf8(std::wstring szUnicode);
String^ FromAnsiToString(const char* str);