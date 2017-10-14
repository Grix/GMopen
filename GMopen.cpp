//Library of some windows-specific utilities for Game Maker
//By Gitle Mikkelsen
//version 1.2


#include "GMopen.h"

GMEXPORT double OpenInExplorer(char* path) 
{
	if (path == NULL)
		return 0.;

	int pathLength = strlen(path);
	wchar_t* pathw = new wchar_t[pathLength];
	MultiByteToWideChar(CP_ACP, 0, path, -1, pathw, pathLength);

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
	int pathLength = strlen(message);
	wchar_t* messagew = new wchar_t[pathLength];
	MultiByteToWideChar(CP_ACP, 0, message, -1, messagew, pathLength);
	pathLength = strlen(caption);
	wchar_t* captionw = new wchar_t[pathLength];
	MultiByteToWideChar(CP_ACP, 0, caption, -1, captionw, pathLength);

	MessageBoxW(NULL, messagew, captionw, MB_OK | flags);

	delete messagew;
	delete captionw;
}

void ShowQuestionThreaded(char* message, char* caption, unsigned int flags, int id)
{
	int pathLength = strlen(message);
	wchar_t* messagew = new wchar_t[pathLength];
	MultiByteToWideChar(CP_ACP, 0, message, -1, messagew, pathLength);
	pathLength = strlen(caption);
	wchar_t* captionw = new wchar_t[pathLength];
	MultiByteToWideChar(CP_ACP, 0, caption, -1, captionw, pathLength);

	int result = MessageBoxW(NULL, messagew, captionw, MB_YESNO | flags);

	delete messagew;
	delete captionw;

	int resultMap = CreateDsMap(0);
	DsMapAddDouble(resultMap, "id", (double)id);
	DsMapAddDouble(resultMap, "status", (double)(result == IDYES));
	CreateAsynEventWithDSMap(resultMap, EVENT_OTHER_SOCIAL);
}

GMEXPORT double FileCopy(char* source, char* dest)
{
	if ((source == NULL) || (dest == NULL))
		return 0.;

	int pathLength = strlen(source);
	wchar_t* sourcew = new wchar_t[pathLength];
	MultiByteToWideChar(CP_ACP, 0, source, -1, sourcew, pathLength);
	pathLength = strlen(dest);
	wchar_t* destw = new wchar_t[pathLength];
	MultiByteToWideChar(CP_ACP, 0, dest, -1, destw, pathLength);

	double result = (double)(CopyFileW(sourcew, destw, false) == TRUE);

	delete sourcew;
	delete destw;

	return result;
}

GMEXPORT double FileExists(char* path)
{
	if (path == NULL)
		return 0.;

	int pathLength = strlen(path);
	wchar_t* pathw = new wchar_t[pathLength];
	MultiByteToWideChar(CP_ACP, 0, path, -1, pathw, pathLength);

	WIN32_FIND_DATAW FindFileData;
	HANDLE handle = FindFirstFileW(pathw, &FindFileData);

	delete pathw;

	if (handle == INVALID_HANDLE_VALUE)
		return 0.;
	else
		return 1.;
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