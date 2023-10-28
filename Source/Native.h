#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include "Result.h"
#include "Util.h"

using namespace std;

Result<DWORD> getProcessId(string& targetProcessName) {

	HANDLE processSnapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
	if (processSnapshot == INVALID_HANDLE_VALUE) {
		return Error("Couldn't get snapshot of processes");
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(processSnapshot, &pe32)) {
		return Error("Couldn't access processes");
	}

	wstring wTargetProcessName = convertStringToWString(targetProcessName);

	//Loop through the snapshot, returning process id if matching process name is found
	do {
		if (wcscmp(pe32.szExeFile, wTargetProcessName.c_str()) == 0) {
			CloseHandle(processSnapshot);
			return pe32.th32ProcessID;
		}
	} while (Process32Next(processSnapshot, &pe32));

	CloseHandle(processSnapshot);

	return Error("Couldn't find process " + targetProcessName);
}

Result<HANDLE> getProcessHandle(const DWORD& processId) {

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
	if (processHandle == nullptr) return Error("Couldn't get process handle");

	return processHandle;
}

Result<string> getDllPath(string& dllName) {

	WCHAR wDllPath[MAX_PATH]{ 0 };
	wstring wDllName = convertStringToWString(dllName);

	DWORD pathLength = GetFullPathNameW(wDllName.c_str(), MAX_PATH, wDllPath, NULL);
	if (pathLength == 0) return Error("Couldn't create path for " + string(dllName));

	return wideArrayToString(wDllPath);
}

Result<SIZE_T> writeDllPathToMemory(HANDLE& processHandle, LPVOID& baseAddress, string& dllPath) {

	SIZE_T bytesWritten;

	bool success = WriteProcessMemory(processHandle, baseAddress, dllPath.c_str(), dllPath.length(), &bytesWritten);
	if (!success) return Error("Couldn't write DLL path to target process memory");

	return bytesWritten;
}

Result<HANDLE> createRemoteThread(HANDLE& processHandle, LPVOID& baseAddress) {

	HANDLE threadHandle = CreateRemoteThread(
		processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, baseAddress, NULL, NULL
	);
	if (threadHandle == NULL) return Error("Couldn't create remote thread");

	return threadHandle;
}