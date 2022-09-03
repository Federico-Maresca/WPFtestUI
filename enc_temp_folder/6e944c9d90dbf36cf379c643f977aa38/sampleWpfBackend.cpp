// sampleWpfBackend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//// Dependencies
#include <iostream>
#include <Windows.h>
#include <thread>

using namespace std;

typedef HWND(*CreateUserInterfaceFunc)(DWORD);
CreateUserInterfaceFunc CreateUserInterface;
typedef void(*DisplayUserInterfaceFunc)(void);
DisplayUserInterfaceFunc DisplayUserInterface;
typedef void(*DestroyUserInterfaceFunc)(void);
DestroyUserInterfaceFunc DestroyUserInterface;
typedef void(*HideUserInterfaceFunc)(void);
HideUserInterfaceFunc HideUserInterface;
typedef int(*UpdateProgressFunc)(void);
UpdateProgressFunc UpdateProgress;
//// Global Objects
WNDCLASSEX HostWindowClass; /// Our Host Window Class Object
MSG loop_message; /// Loop Message for Host Window
HINSTANCE hInstance = GetModuleHandle(NULL); /// Application Image Base Address
HWND cpphwin_hwnd; /// Host Window Handle
HWND wpf_hwnd; /// WPF Wrapper Handle
int gui_thread;
RECT hwin_rect; /// Add this in global objects
//// Global Configs
const wchar_t cpphwinCN[] = L"CppMAppHostWinClass"; /// Host Window Class Name
bool isHWindowRunning = false; /// Host Window Running State

int amount;
void print() {
	int test = amount * 2;
	int percentVAl = test / 100;
	int minpercent = percentVAl;
	int count = 0;
	int result;
	while (test >= 0) {
		std::cout << test << std::endl;
		result = sqrt(test);
		std::cout << result << std::endl;
		test--;
		minpercent--;
		if (minpercent == 0) {
			UpdateProgress();
			minpercent = percentVAl;
			count++;
		}
	}
	std::cout << count << std::endl;
}

void sendint(int pamount) {
	amount = pamount;
	HideUserInterface();
	::Sleep(2000);
	DisplayUserInterface();
}


//// Host Window Callback
LRESULT CALLBACK HostWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyUserInterface(); //// Destroy WPF Control before Destroying Host Window
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		isHWindowRunning = false;
		break;
	case WM_SIZE: //// Resize WPF Control on Host Window Resizing
		if (wpf_hwnd != nullptr) {
			GetClientRect(cpphwin_hwnd, &hwin_rect);
			MoveWindow(wpf_hwnd, 0, 0, hwin_rect.right - hwin_rect.left,
				hwin_rect.bottom - hwin_rect.top, TRUE);
		}
		break;
	case 1025:
		print();
		break;
	case 1026:
		sendint((int)lParam);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
//// Our Application Entry Point
int main()
{
	cout << "C++ Main App Started..." << endl;
	// Creating Icon Object From Resources, Don't forget to include resource.h!
	HICON app_icon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_WINLOGO));

	///// Defining Our Host Window Class
	//HostWindowClass.cbSize = sizeof(WNDCLASSEX); HostWindowClass.lpfnWndProc = HostWindowProc;
	//HostWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//HostWindowClass.cbClsExtra = 0; HostWindowClass.style = 0;
	//HostWindowClass.cbWndExtra = 0;    HostWindowClass.hInstance = hInstance;
	//HostWindowClass.hIcon = app_icon; HostWindowClass.hIconSm = app_icon;
	//HostWindowClass.lpszClassName = cpphwinCN; HostWindowClass.lpszMenuName = NULL;
	////// Host Window Callback, NOTE :Define This Before Your Entrypoint Function
	////// Register Window
	//if (!RegisterClassEx(&HostWindowClass))
	//{
	//	cout << "Error, Code :" << GetLastError() << endl;
	//	getchar(); 
	//	return 0;
	//}
	///// Creating Unmanaged Host Window
	//cpphwin_hwnd = CreateWindowEx(
	//	WS_EX_CLIENTEDGE,
	//	cpphwinCN,
	//	L"TEST",
	//	WS_THICKFRAME | WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, CW_USEDEFAULT, 800, 500,
	//	NULL, NULL, hInstance, NULL);

	///// Check if How Window is valid
	//if (cpphwin_hwnd == NULL)
	//{
	//	cout << "Error, Code :" << GetLastError() << endl;
	//	getchar(); return 0;
	//}

	///// Making Window Fixed Size
	//::SetWindowLong(cpphwin_hwnd, GWL_STYLE,
	//	GetWindowLong(cpphwin_hwnd, GWL_STYLE) & ~WS_SIZEBOX);
	//cout << "C++ Main App Finished." << endl;

	///// Centering Host Window
	//RECT window_r; RECT desktop_r;
	//GetWindowRect(cpphwin_hwnd, &window_r); GetWindowRect(GetDesktopWindow(), &desktop_r);
	//int xPos = (desktop_r.right - (window_r.right - window_r.left)) / 2;
	//int yPos = (desktop_r.bottom - (window_r.bottom - window_r.top)) / 2;

	///// Set Window Position
	//::SetWindowPos(cpphwin_hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);



	/// Loading dotNet UI Library
	HMODULE dotNetGUILibrary = NULL;
	dotNetGUILibrary = LoadLibraryA("C:\\Users\\fmare\\source\\repos\\sampleWpfBackend\\UI\\bin\\Debug\\x86\\UI.dll");
	if (dotNetGUILibrary != NULL) {
		CreateUserInterface = (CreateUserInterfaceFunc)GetProcAddress
		(dotNetGUILibrary, "CreateUserInterface");
		DisplayUserInterface = (DisplayUserInterfaceFunc)GetProcAddress
		(dotNetGUILibrary, "DisplayUserInterface");
		DestroyUserInterface = (DestroyUserInterfaceFunc)GetProcAddress
		(dotNetGUILibrary, "DestroyUserInterface");
		HideUserInterface = (HideUserInterfaceFunc)GetProcAddress
		(dotNetGUILibrary, "HideUserInterface");
		UpdateProgress = (UpdateProgressFunc)GetProcAddress
		(dotNetGUILibrary, "UpdateProgress");
	}
	/// Creating .Net GUI
	wpf_hwnd = CreateUserInterface(GetThreadId(GetCurrentThread()));
	//gui_thread = GetGuiThread();
	/// Check if WPF Window is valid
	if (wpf_hwnd != nullptr) {

		///// Disable Host Window Updates & Draws
		//SendMessage(cpphwin_hwnd, WM_SETREDRAW, FALSE, 0);

		///// Disable Host Window Double Buffering
		//long dwExStyle = GetWindowLong(cpphwin_hwnd, GWL_EXSTYLE);
		//dwExStyle &= ~WS_EX_COMPOSITED;
		//SetWindowLong(cpphwin_hwnd, GWL_EXSTYLE, dwExStyle);

		///// Set WPF Window to a Child Control
		//SetWindowLong(wpf_hwnd, GWL_STYLE, WS_CHILD);

		///// Get your host client area rect
		//GetClientRect(cpphwin_hwnd, &hwin_rect);

		///// Set WPF Control Order, Size and Position
		//MoveWindow(wpf_hwnd, 0, 0, hwin_rect.right - hwin_rect.left,
		//	hwin_rect.bottom - hwin_rect.top, TRUE);
		//SetWindowPos(wpf_hwnd, HWND_TOP, 0, 0, hwin_rect.right - hwin_rect.left,
		//	hwin_rect.bottom - hwin_rect.top, SWP_NOMOVE);

		///// Set WPF as A Child to Host Window...
		//SetParent(wpf_hwnd, cpphwin_hwnd);

		/// Skadoosh!
		ShowWindow(wpf_hwnd, SW_SHOW);

		/// Display WPF Control by resetting its Opacity
		DisplayUserInterface();
	};
	/// Display Window
	//ShowWindow(cpphwin_hwnd, SW_SHOW);
	//UpdateWindow(cpphwin_hwnd);
	//BringWindowToTop(cpphwin_hwnd);
	//ShowWindow(wpf_hwnd, SW_SHOW);
	isHWindowRunning = true;
	/// Adding Message Loop
	while (GetMessage(&loop_message, NULL, 0, 0) > 0 && isHWindowRunning)
	{
		switch (loop_message.message)
		{
		case 1025:
			print();
			break;
		case 1026:
			sendint((int)loop_message.lParam);
			break;
		}
		TranslateMessage(&loop_message);
		DispatchMessage(&loop_message);
	}
}