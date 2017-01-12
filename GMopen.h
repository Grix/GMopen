#include <Windows.h>
#include <thread>

#define GMEXPORT extern "C" __declspec (dllexport)

static COLORREF get_colors[16];

GMEXPORT double open_in_explorer(char* path);
GMEXPORT double get_color_win(double c);
GMEXPORT double show_message_win(char* message);
GMEXPORT double show_question_win(char* message);

void MessageBoxThreaded(char* message);
wchar_t *convertCharArrayToLPCWSTR(char* charArray);