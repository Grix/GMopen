#include "GMopen.h"

GMEXPORT double open_in_explorer(char* path) 
{
	return (double)(int)ShellExecuteW(NULL, L"open", convertCharArrayToLPCWSTR(path), NULL, NULL, SW_SHOW);
}
	
GMEXPORT double get_color_win(double c) { 
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = GetActiveWindow();
	cc.lpCustColors = (LPDWORD)get_colors;
	cc.rgbResult = (int)c;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	if (ChooseColor(&cc) == TRUE) {
		return cc.rgbResult;
	} else return c;
}

GMEXPORT double show_message_win(char* message)
{
	std::thread messageThread(MessageBoxThreaded, message);
	messageThread.detach();
	return 1.;
}

GMEXPORT double show_question_win(char* message)
{
	return 0.;
}

void MessageBoxThreaded(char* message)
{
	MessageBoxW(NULL, convertCharArrayToLPCWSTR(message), L"", MB_OK);
}

wchar_t *convertCharArrayToLPCWSTR(char* charArray)
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