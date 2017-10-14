//Library of some windows-specific utilities for Game Maker
//By Gitle Mikkelsen
//version 1.2


#include <Windows.h>
#include <thread>

#define GMEXPORT extern "C" __declspec (dllexport)

#define EVENT_OTHER_SOCIAL 70

static COLORREF getColors[16];

int dialogId = 100;

GMEXPORT double OpenInExplorer(char* path);
GMEXPORT double GetColor(double c);
GMEXPORT double ShowMessage(char* message, char* caption, double flags);
GMEXPORT double ShowQuestion(char* message, char* caption, double flags);
GMEXPORT double FileCopy(char* source, char* dest);

void ShowMessageThreaded(char* message, char* caption, unsigned int flags);
void ShowQuestionThreaded(char* message, char* caption, unsigned int flags, int id);

void(*CreateAsynEventWithDSMap)(int, int);
int(*CreateDsMap)(int _num, ...);
bool(*DsMapAddDouble)(int _index, char *_pKey, double value);
bool(*DsMapAddString)(int _index, char *_pKey, char *pVal);

GMEXPORT void RegisterCallbacks(char *arg1, char *arg2, char *arg3, char *arg4);