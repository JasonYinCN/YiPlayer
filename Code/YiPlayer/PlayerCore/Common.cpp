#include "stdafx.h"
#include "Common.h"
char* FromStringHandleToAnsi(const String^ str)
{
	if (nullptr == str)
	{
		return new char(0);
	}

	pin_ptr<const wchar_t> wch = PtrToStringChars(str);

	int len = WideCharToMultiByte(CP_ACP, 0, wch, -1, NULL, 0, NULL, NULL);
	char* buf = new char[len + 1]();

	WideCharToMultiByte(CP_ACP, 0, wch, -1, buf, len, NULL, NULL);

	wch = nullptr;

	return buf;
}

wchar_t* FromStringHandleToUnicode(const String ^ str)
{
	if (nullptr == str)
	{
		return new wchar_t(0);
	}
	pin_ptr<const wchar_t> wch = PtrToStringChars(str);
	return const_cast<wchar_t*>(wch);
}

wchar_t* FromAnsiToUnicode(const char* str)
{
	if (NULL == str)
	{
		return L"";
	}

	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	wchar_t* buf = new wchar_t[len + 1]();

	MultiByteToWideChar(CP_ACP, 0, str, -1, buf, len);

	return buf;
}

void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);
	char *psText; 
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);
	szDst = psText;
	delete[] psText;
}

std::string FromAnsiToUtf8(const char * str)
{
	if (NULL == str)
	{
		return "";
	}

	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	wchar_t* buf = new wchar_t[len]();

	//ANSI先转UNICODE
	MultiByteToWideChar(CP_ACP, 0, str, -1, buf, len);

	int len1 = WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL);
	char* buf1 = new char[len1 + 1]();

	//UNICODE转UTF8
	WideCharToMultiByte(CP_UTF8, 0, buf, -1, buf1, len1, NULL, NULL);

	delete[] buf;

	std::string result(buf1);

	delete buf1;

	return result;
}

std::wstring FromUtf8ToUnicode(const char * str)
{
	if (NULL == str)
	{
		return L"";
	}

	int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_t* buf = new wchar_t[len + 1]();

	MultiByteToWideChar(CP_UTF8, 0, str, -1, buf, len);

	std::wstring result(buf);

	delete buf;

	return result;
}

std::string Unicode2Utf8(std::wstring szUnicode)
{
	#ifdef WIN32
	//calc block size to be returned
	int len = WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), NULL, 0, NULL, NULL);
	//malloc and fill the returned block
	char* szUtf8 = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, NULL, szUnicode.c_str(), szUnicode.size(), szUtf8, len, NULL, NULL);
	szUtf8[len] = 0;
	//std::string rs = szUtf8;
	std::string rs;
	rs.append(szUtf8, len);
	delete[] szUtf8;

	return rs;
	#else
	//setlocale(LC_ALL, "UTF-8");
	setlocale(LC_CTYPE, "UTF-8");
	std::string rs;
	//mbstowcs or wcstombs converting the string to the NULL but not with a length. so must invoke it cyclically.
	while (true) {
		//calc block size to be returned
		size_t len = wcstombs(0, szUnicode.c_str(), 0);
		//malloc and fill the returned block
		char* szUtf8 = new char[len + 1];
		wcstombs(szUtf8, szUnicode.c_str(), len);
		szUtf8[len] = 0;
		//std::string rs = szUtf8;
		rs.append(szUtf8, len);
		delete[] szUtf8;

		size_t pos = szUnicode.find(L'\0');
		if (pos == std::wstring::npos) break;
		rs.append(1, 0);
		szUnicode = szUnicode.substr(pos + 1);
	}

	return rs;
	#endif
}

String ^ FromAnsiToString(const char * str)
{
	return gcnew String(str);
	// TODO: 在此处插入 return 语句
}
