#pragma once

#include <Windows.h>
#include <filesystem>
#include <random>
#include <TlHelp32.h>

bool FileExists(const char* path)
{
	return std::filesystem::exists(path);
}

//CHANGE UR GAME PATH IN HERE
void WriteCustomSplashScreen()
{
	std::filesystem::remove("C:\\Program Files (x86)\\Origin Games\\Apex\\EasyAntiCheat\\Launcher\\SplashScreen.png");
	std::filesystem::copy("C:\\Users\\dynam\\Game Reversal\\ApexLegends-SDK\\Apex Legends SDK\\x64\\Release\\resources\\SplashScreen.png", "C:\\Program Files (x86)\\Origin Games\\Apex\\EasyAntiCheat\\Launcher\\SplashScreen.png");
}

void WriteDefaultSplashScreen()
{
	std::filesystem::remove("C:\\Program Files (x86)\\Origin Games\\Apex\\EasyAntiCheat\\Launcher\\SplashScreen.png");
	std::filesystem::copy("C:\\Users\\dynam\\Game Reversal\\ApexLegends-SDK\\Apex Legends SDK\\x64\\Release\\resources\\origSplashScreen.png", "C:\\Program Files (x86)\\Origin Games\\Apex\\EasyAntiCheat\\Launcher\\SplashScreen.png");
}

void OpenApex()
{
	ShellExecute(NULL, L"open", L"C:\\Program Files (x86)\\Origin Games\\Apex\\r5apex.exe", NULL, NULL, SW_SHOWDEFAULT);
}

uintptr_t GetPid(const wchar_t* processName)
{
	PROCESSENTRY32 procEntry32;
	uintptr_t pID = 0;

	procEntry32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (!hProcSnap || hProcSnap == INVALID_HANDLE_VALUE)
		return 0;

	while (Process32Next(hProcSnap, &procEntry32))
	{
		if (!wcscmp(processName, procEntry32.szExeFile))
		{
			pID = procEntry32.th32ProcessID;

			CloseHandle(hProcSnap);
		}
	}

	CloseHandle(hProcSnap);
	return pID;
}

std::string RandomString(const int len)
{
	const std::string alpha_numeric("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");

	std::default_random_engine generator{ std::random_device{}() };
	const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };

	std::string str(len, 0);
	for (auto& it : str)
		it = alpha_numeric[distribution(generator)];

	return str;
}