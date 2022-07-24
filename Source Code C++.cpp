#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetPID(const char* ProcessName) {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);


	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!strcmp(processInfo.szExeFile, ProcessName))
	{
		CloseHandle(processesSnapshot);
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!strcmp(processInfo.szExeFile, ProcessName))
		{
			CloseHandle(processesSnapshot);
		}
	}
	CloseHandle(processesSnapshot);
	return processInfo.th32ProcessID;
}

MODULEENTRY32 GetModule(const char* moduleName, unsigned long ProcessID)
{
	MODULEENTRY32 modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 curr = { 0 };

		curr.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &curr))
		{
			do
			{
				if (!strcmp(curr.szModule, moduleName))
				{
					modEntry = curr;
					break;
				}
			} while (Module32Next(hSnapshot, &curr));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	MessageBox(NULL,"Controls:\nDEL - Exits Albs Software\nINSERT - Preforms ALBS Auto Login Bypass\nHOME - Brings Back The Controlls Page","MC-EDU ALBS",NULL);
	unsigned long long pid = GetPID("Minecraft.Windows.exe");
	MODULEENTRY32 module = GetModule("Minecraft.Windows.exe", pid);
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (FindWindow(NULL, "Minecraft: Education Edition") == NULL)
	{
		MessageBox(NULL, "An Error Occured:\nPlease Launch Minecraft: Education Edition", "MC-EDU ALBS", NULL);
		exit(-1);
	}

	int result;
	int lgin = 8;
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) & 0x8000)
		{
			if (FindWindow(NULL, "Minecraft: Education Edition") == NULL)
			{
				MessageBox(NULL, "An Error Occured:\nCould Not Login Bypass Try Reopening Minecraft: Education Edition", "MC-EDU ALBS", NULL);
			}
			if (FindWindow(NULL, "Minecraft: Education Edition") != NULL)
			{
				pid = GetPID("Minecraft.Windows.exe");
				MODULEENTRY32 module = GetModule("Minecraft.Windows.exe", pid);
				HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

				ReadProcessMemory(phandle, (void*)((unsigned long long)module.modBaseAddr + 0x02CC015C), &result, sizeof(result), 0);
				ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0x0), &result, sizeof(result), 0);
				ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0x7C), &result, sizeof(result), 0);
				ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0x0), &result, sizeof(result), 0);
				ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0x64), &result, sizeof(result), 0);
				ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0x0), &result, sizeof(result), 0);
				ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0x2E4), &result, sizeof(result), 0);
				WriteProcessMemory(phandle, (void*)((unsigned long long)result + 0x8), &lgin, sizeof(lgin), 0);
			}
			Sleep(200);
		}
		if (GetAsyncKeyState(VK_DELETE) & 0x8000)
		{
			exit(1);
		}
		if (GetAsyncKeyState(VK_HOME) & 0x8000)
		{
			MessageBox(NULL, "Controls:\nDEL - Exits Albs Software\nINSERT - Preforms ALBS Auto Login Bypass\nHOME - Brings Back The Controlls Page", "MC-EDU ALBS", NULL);
			Sleep(500);
		}
	}

}
