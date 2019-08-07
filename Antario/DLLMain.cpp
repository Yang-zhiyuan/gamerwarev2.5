#include <thread>
#include "Hooks.h"
#include "Utils\Utils.h"
#include "Utils\GlobalVars.h"
#include <bitset>
#include "curl/curl.h"
#include <stdio.h>
#pragma comment(lib, "urlmon.lib")
#include <urlmon.h>
#include <sstream>
#include "xor.h"
#include <IPTypes.h>
#include <iphlpapi.h>
#include <windows.h>
#include "Utils/curlplusplus.h"
#include "Menu/Menu.h"


HINSTANCE HThisModule;

/*int OnDllAttach()
{
	std:string myhwid = get_hwid(); // get your hwid
	CURLplusplus client; // https://stackoverflow.com/a/27026683
	string x = client.Get(xorstr("put site here").crypt_get()); // if u actually wanna use my "hwid" bullshit, u can use this right here by replacing the site with ur site. i stored my hwids in a text file on my site btw
	if (x.find(myhwid) != std::string::npos) {
		Hooks::Init(); 
	}
	else {
		toClipboard(hwnd, myhwid);
		exit(0);
	}
	return 0;
}*/

int OnDllAttach()
{
	Hooks::Init();

	while (true)
		Sleep(10000000);

	Hooks::Restore();

	FreeLibraryAndExitThread(HThisModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH && GetModuleHandleA("csgo.exe"))
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OnDllAttach, NULL, NULL, NULL);
	}

	return TRUE;
}


