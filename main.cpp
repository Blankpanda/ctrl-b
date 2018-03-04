#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <algorithm>
#include <iostream>

void printDebugString(char* debugString);

std::string getClipboardText();
std::string createUrlString(std::string args);
std::string argsAsUrlStr(std::string args);
std::wstring strTowStr(const std::string & s);


int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       cmdShow)
{
	std::string clipBoardText;
	MSG msg = { 0 };
	
	if (RegisterHotKey(NULL,1,MOD_CONTROL,0x42))
	{
		printDebugString("Failed to register ctlr-b!");
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_HOTKEY)
		{
			clipBoardText = getClipboardText();

			if (clipBoardText.empty())
			{
				printDebugString("Clipboard failed.");
				return 0;
			}
			else
			{			
				const std::string urlString = createUrlString(clipBoardText);
				std::wstring wurlString = strTowStr(urlString);;
				std::wcout << wurlString;

				if (ShellExecute(nullptr, L"open", wurlString.c_str(), NULL, NULL, SW_SHOW))
				{

				}
			}
		}
	}
	return 0;
}

std::string getClipboardText()
{
	std::string clipText("");

	if (!OpenClipboard(nullptr)) {
		printDebugString("Failed to open clipboard!");
		return NULL;
	}

	HANDLE clipHandle = GetClipboardData(CF_TEXT);

	if (clipHandle == NULL)
	{
		printDebugString("Handle to clipboard is null!");
		return NULL;
	}

	// GlobalLock() returns a void pointer to a handle
	char* tmp = static_cast<char*>(GlobalLock(clipHandle));	
	clipText = tmp;

	CloseClipboard();
	GlobalUnlock(clipHandle);
	
	return clipText;
}

void printDebugString(char* debugString) {
	char string[256];
	sprintf_s(string, "[ERROR_CODE=%d]:%s\n", GetLastError(),debugString);
	OutputDebugStringA(string);
}

std::string createUrlString(std::string args) 
{	
	return std::string("https://google.com/search?q=" + argsAsUrlStr(args));
}

std::string argsAsUrlStr(std::string args)
{	
	//std::string tmp = args.replace(args.begin(), args.end(), ' ', '+');
	//return tmp;
	return args;
}

// https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
std::wstring strTowStr(const std::string & s) 
{
	int len;
	int slength = (int)s.length() + 1;
	
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	
	std::wstring r(buf);
	
	delete[] buf;
	
	return r;
}
