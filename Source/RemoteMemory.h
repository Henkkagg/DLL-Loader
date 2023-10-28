#pragma once

#include <Windows.h>
#include "result.h"

class RemoteMemory {

	HANDLE _processHandle;
	LPVOID _baseAddress;

	RemoteMemory(const RemoteMemory&) = delete;
	RemoteMemory& operator=(const RemoteMemory&) = delete;

public:

	~RemoteMemory() {
		if (_baseAddress) {
			VirtualFreeEx(_processHandle, _baseAddress, 0, MEM_RELEASE);
		}
	}

	RemoteMemory(HANDLE processHandle, int sizeInBytes)
		: _processHandle(processHandle) {
		_baseAddress = VirtualAllocEx(
			processHandle, NULL, sizeInBytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE
		);
	}

	Result<LPVOID> getBaseAddress() const {
		if (_baseAddress != NULL) {
			return _baseAddress;
		}
		else {
			return Error("Couldn't allocate remote memory");
		}
	}
};