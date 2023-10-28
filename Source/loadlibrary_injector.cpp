#include <Windows.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tchar.h>
#include <thread>
#include <chrono>
#include "Util.h"
#include "Result.h"
#include "RemoteMemory.h"
#include "Native.h"
#include <fstream>

using namespace std;

int main() {

	string processName;
	string dllName;

	ifstream file("config.cfg");
	if (file.is_open()) {

		getline(file, processName);
		getline(file, dllName);
	}

	if (processName.empty() || dllName.empty()) {

		println("Name (icluding .exe) of the target process:");
		cin >> processName;

		println("Name (including .dll) of the injectable library");
		cin >> dllName;
	}

	auto pidResult = getProcessId(processName);

	auto processHandle = getProcessHandle(*pidResult);

	auto dllPath = getDllPath(dllName);

	auto remoteMemory = RemoteMemory(*processHandle, (*dllPath).length());
	auto baseAddress = remoteMemory.getBaseAddress();

	auto bytesWritten = writeDllPathToMemory(*processHandle, *baseAddress, *dllPath);

	auto threadHandle = createRemoteThread(*processHandle, *baseAddress);

	return 0;
}