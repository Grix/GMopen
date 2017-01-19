#include "GMopen.h"

GMEXPORT double OpenInExplorer(char* path) 
{
	return (double)(int)ShellExecuteW(NULL, L"open", ConvertCharArrayToLPCWSTR(path), NULL, NULL, SW_SHOW);
}
	
GMEXPORT double GetColor(double c) { 
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = GetActiveWindow();
	cc.lpCustColors = (LPDWORD)getColors;
	cc.rgbResult = (int)c;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColor(&cc) == TRUE)
		return cc.rgbResult;
	else 
		return c;
}

GMEXPORT double ShowMessage(char* message, char* caption, double flags)
{
	std::thread messageThread(ShowMessageThreaded, message, caption, (unsigned int)(flags + 0.5));
	messageThread.detach();
	return 1.;
}

GMEXPORT double ShowQuestion(char* message, char* caption, double flags)
{
	std::thread messageThread(ShowQuestionThreaded, message, caption, (unsigned int)(flags+0.5));
	messageThread.detach();
	return 1.;
}

void ShowMessageThreaded(char* message, char* caption, unsigned int flags)
{
	MessageBoxW(NULL, ConvertCharArrayToLPCWSTR(message), ConvertCharArrayToLPCWSTR(caption), MB_OK | flags);
}

void ShowQuestionThreaded(char* message, char* caption, unsigned int flags)
{
	MessageBoxW(NULL, ConvertCharArrayToLPCWSTR(message), ConvertCharArrayToLPCWSTR(caption), MB_YESNO | flags);

	int my_map_index = CreateDsMap(0);
	DsMapAddDouble(my_map_index, "another_number", 42);
	DsMapAddString(my_map_index, "another_string", "hello, world");
	CreateAsynEventWithDSMap(my_map_index, EVENT_OTHER_SOCIAL);
}

wchar_t *ConvertCharArrayToLPCWSTR(char* charArray)
{
	for (int i = 0; i < 4096; i++)
	{
		if (charArray[i] == '\n')
			break;
		else if (charArray[i] == '#')
			charArray[i] = '\n';
	}
    wchar_t* wString=new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

__declspec (dllexport) void RegisterCallbacks(char *arg1, char *arg2, char *arg3, char *arg4)
{
	//MessageBoxW(NULL, L"tesT", L"tesT", MB_OK);
	void(*CreateAsynEventWithDSMapPtr)(int, int) = (void(*)(int, int))(arg1);
	int(*CreateDsMapPtr)(int _num, ...) = (int(*)(int _num, ...)) (arg2);
	CreateAsynEventWithDSMap = CreateAsynEventWithDSMapPtr;
	CreateDsMap = CreateDsMapPtr;

	bool(*DsMapAddDoublePtr)(int _index, char *_pKey, double value) = (bool(*)(int, char*, double))(arg3);
	bool(*DsMapAddStringPtr)(int _index, char *_pKey, char *pVal) = (bool(*)(int, char*, char*))(arg4);

	DsMapAddDouble = DsMapAddDoublePtr;
	DsMapAddString = DsMapAddStringPtr;
}