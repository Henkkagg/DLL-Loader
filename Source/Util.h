#pragma once

#include <Windows.h>
#include <string>

using namespace std;

template <typename T> void println(T value) {
	cout << value << endl;
}

inline std::wstring convertStringToWString(const std::string& string) {
	int characterCount = MultiByteToWideChar(
		CP_UTF8,
		0,
		string.c_str(),
		string.length(),
		NULL,
		0
	);
	std::wstring wString(characterCount, L' ');
	MultiByteToWideChar(
		CP_UTF8,
		0,
		string.c_str(),
		string.length(),
		&wString[0],
		characterCount
	);

	return wString;
}

inline std::string wideArrayToString(const WCHAR* wideArray) {
	int characterCount = WideCharToMultiByte(CP_UTF8,
		0,
		wideArray,
		-1,
		NULL,
		0,
		NULL,
		NULL
	);

	char* buffer = new char[characterCount];

	WideCharToMultiByte(
		CP_UTF8,
		0,
		wideArray,
		-1,
		buffer,
		characterCount,
		NULL,
		NULL
	);

	std::string result(buffer);
	delete[] buffer;

	return result;
}