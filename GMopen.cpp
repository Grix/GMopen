#include "GMopen.h"

GMEXPORT double OpenInExplorer(char* path) 
{
	if (path == NULL)
		return 0.;
	wchar_t* pathw = ConvertCharArrayToRawLPCWSTR(path);
	int result = (int)ShellExecuteW(NULL, L"open", pathw, NULL, NULL, SW_SHOW);
	delete pathw;
	return (double)result;
}
	
//credits to yal.cc for this
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
	if ((message == NULL) || (caption == NULL))
		return 0.;
	std::thread messageThread(ShowMessageThreaded, message, caption, (unsigned int)(flags + 0.5));
	messageThread.detach();
	return 1.;
}

GMEXPORT double ShowQuestion(char* message, char* caption, double flags)
{
	if ((message == NULL) || (caption == NULL))
		return 0.;
	int id = dialogId++;
	std::thread messageThread(ShowQuestionThreaded, message, caption, (unsigned int)(flags+0.5), id);
	messageThread.detach();
	return (double)id;
}

void ShowMessageThreaded(char* message, char* caption, unsigned int flags)
{
	wchar_t* msg = ConvertCharArrayToLPCWSTR(message);
	wchar_t* cap = ConvertCharArrayToLPCWSTR(caption);
	MessageBoxW(NULL, ConvertCharArrayToLPCWSTR(message), ConvertCharArrayToLPCWSTR(caption), MB_OK | flags);
	delete msg;
	delete cap;
}

void ShowQuestionThreaded(char* message, char* caption, unsigned int flags, int id)
{
	wchar_t* msg = ConvertCharArrayToLPCWSTR(message);
	wchar_t* cap = ConvertCharArrayToLPCWSTR(caption);
	int result = MessageBoxW(NULL, msg, cap, MB_YESNO | flags);
	delete msg;
	delete cap;
	int resultMap = CreateDsMap(0);
	DsMapAddDouble(resultMap, "id", (double)id);
	DsMapAddDouble(resultMap, "status", (double)(result == IDYES));
	CreateAsynEventWithDSMap(resultMap, EVENT_OTHER_SOCIAL);
}

GMEXPORT double FileCopy(char* source, char* dest)
{
	if ((source == NULL) || (dest == NULL))
		return 0.;
	wchar_t* sourcew = ConvertCharArrayToRawLPCWSTR(source);
	wchar_t* destw = ConvertCharArrayToRawLPCWSTR(dest);
	double result = (double)(CopyFileW(sourcew, destw, false) == TRUE);
	delete sourcew;
	delete destw;
	return result;
}

GMEXPORT double FileExists(char* path)
{
	if (path == NULL)
		return 0.;
	WIN32_FIND_DATAW FindFileData;
	wchar_t* pathw = ConvertCharArrayToRawLPCWSTR(path);
	HANDLE handle = FindFirstFileW(pathw, &FindFileData);
	delete pathw;
	if (handle == INVALID_HANDLE_VALUE)
		return 0.;
	else
		return 1.;
}

wchar_t *ConvertCharArrayToLPCWSTR(char* charArray)
{
	int i = 0;
	for (i = 0; i < 4096; i++)
	{
		if (charArray[i] == '\0')
			break;
		if (charArray[i] == '#')
			charArray[i] = '\n';
	}
	wchar_t* wString = new wchar_t[i+1];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, i + 1);
    return wString;
}

wchar_t *ConvertCharArrayToRawLPCWSTR(char* charArray)
{
	int i = 0;
	for (i = 0; i < 4096; i++)
	{
		if (charArray[i] == '\0')
			break;
	}
	wchar_t* wString = new wchar_t[i+1];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, i+1);
	return wString;
}

//called automatically when GM loads the extension
GMEXPORT void RegisterCallbacks(char *arg1, char *arg2, char *arg3, char *arg4)
{
	void(*CreateAsynEventWithDSMapPtr)(int, int) = (void(*)(int, int))(arg1);
	int(*CreateDsMapPtr)(int _num, ...) = (int(*)(int _num, ...)) (arg2);
	CreateAsynEventWithDSMap = CreateAsynEventWithDSMapPtr;
	CreateDsMap = CreateDsMapPtr;

	bool(*DsMapAddDoublePtr)(int _index, char *_pKey, double value) = (bool(*)(int, char*, double))(arg3);
	bool(*DsMapAddStringPtr)(int _index, char *_pKey, char *pVal) = (bool(*)(int, char*, char*))(arg4);

	DsMapAddDouble = DsMapAddDoublePtr;
	DsMapAddString = DsMapAddStringPtr;
}