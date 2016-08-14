#define GMEXPORT extern "C" __declspec (dllexport)
#include <Windows.h>

wchar_t *convertCharArrayToLPCWSTR(const char* charArray)
	{
    wchar_t* wString=new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
	}

GMEXPORT double open_in_explorer(char* path) 
	{
	return (double)(int)ShellExecute(NULL, L"open", convertCharArrayToLPCWSTR(path), NULL, NULL, SW_SHOW);
	}