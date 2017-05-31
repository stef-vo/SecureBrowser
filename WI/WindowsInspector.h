#pragma once
#include "WindowsInspectorUtils.h"
#include <thread>
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <psapi.h>

namespace SBWindowsInspector
{

class WindowsInspector
{
public:
	WINDOWSINSPECTOR_API WindowsInspector();
	WINDOWSINSPECTOR_API ~WindowsInspector();

	WINDOWSINSPECTOR_API void StartWindowsInspector();
	WINDOWSINSPECTOR_API void StopWindowsInspector();
	void StopAndWait();
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd);

protected:
	void MessageLoop();

private:
	std::thread worker_;

};

}