// sampleWpfBackend.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//// Dependencies
#include <iostream>
#include <Windows.h>
#include <thread>

using namespace std;

typedef HWND(*CreateUserInterfaceFunc)(DWORD);
CreateUserInterfaceFunc CreateUserInterface = nullptr;
typedef void(*DisplayUserInterfaceFunc)(void);
DisplayUserInterfaceFunc DisplayUserInterface = nullptr;
typedef void(*DestroyUserInterfaceFunc)(void);
DestroyUserInterfaceFunc DestroyUserInterface = nullptr;
typedef void(*HideUserInterfaceFunc)(void);
HideUserInterfaceFunc HideUserInterface = nullptr;
typedef int(*UpdateProgressFunc)(void);
UpdateProgressFunc UpdateProgress = nullptr;
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
}/*
// Our Application Entry Point
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
}*/

BOOL rpCurrSide;
BOOL rpCurrFid;
BOOL rTopPass[3] = { FALSE,FALSE, FALSE };
BOOL rBtmPass[3] = { FALSE,FALSE, FALSE };
BOOL rTopPassAuto[3] = { TRUE,TRUE, TRUE };
BOOL rBtmPassAuto[3] = { TRUE,TRUE, FALSE };
BOOL rManual = TRUE;
BOOL rAuto = TRUE;


BOOL align(BOOL force) {
	BOOL rValue;
	if (force)
		if (rpCurrSide)
			rTopPass[rpCurrFid] = TRUE;
		else
			rBtmPass[rpCurrFid] = TRUE;
	else
		if (rpCurrSide)
			rTopPass[rpCurrFid] = rTopPassAuto[rpCurrFid];
		else
			rBtmPass[rpCurrFid] = rBtmPassAuto[rpCurrFid];
	rValue = rpCurrSide ? rTopPass[rpCurrFid] : rBtmPass[rpCurrFid];

	return rValue;
}

BOOL alignW(BOOL force) {
	BOOL rSuccess = FALSE;
	if (!rManual)
		rSuccess = align(FALSE);
	else {

	}

	std::cout << rpCurrFid << " TOP" << (!rSuccess ? " FAIL" : (force ? " LEARN" : " AUTOPASS")) << std::endl;
	return rSuccess;
}
BOOL alignarea(BOOL pIsTop, BOOL pIsBtm) {
	int maxFid = 3;
	BOOL rIsTop = pIsTop;
	BOOL rIsBtm = pIsBtm;

	BOOL rForceTop = FALSE;
	BOOL rForceBtm = FALSE;
	BOOL rSuccess = FALSE;
	BOOL rReset = FALSE;
	BOOL rpSkipBoard = FALSE;
	BOOL rpExit = FALSE;

	if (rManual && !rAuto) {
		rForceTop = pIsTop;
		rForceBtm = pIsBtm;
		if (pIsTop && pIsBtm)
			rIsBtm = FALSE;
	}
	for (rpCurrFid = 0; rpCurrFid < maxFid; rpCurrFid++)
	{
		if (rIsTop && (!rTopPass[rpCurrFid] || rForceTop)) {
			rpCurrSide = TRUE;
			rSuccess = align(rForceTop);
			std::cout << rpCurrFid << " TOP" << (!rSuccess ? " FAIL" : (rForceTop ? " LEARN" : " AUTOPASS")) << std::endl;
			//Entered only the first time that a fiducial fails, starts manual align of
			//that area side.
			if (rSuccess) {

			}
			else if (rManual && !rForceTop) {
				rForceBtm = TRUE;
				//If the other side is already in manual, give priority to that side
				//otherwise reset fiducial count
				if (rForceBtm)
					rIsTop = FALSE;
				else
					rReset = TRUE;
			}
		}

		if (rpSkipBoard || rpExit) {
			//CLOSE MANUAL ALIGN WINDOW
			//FAIL BOARD
			if (rpExit)
				//TODO invalidate all results
				break;
		}
		if (rIsBtm && (!rBtmPass[rpCurrFid] || rForceBtm)) {
			rpCurrSide = FALSE;
			rSuccess = align(rForceBtm);

			std::cout << rpCurrFid << " BTM" << (!rSuccess ? " FAIL" : (rForceBtm ? " LEARN" : " AUTOPASS")) << std::endl;
			//Entered only the first time that a fiducial fails, starts manual align of
			//that area side.
			if (rSuccess) {

			}
			else if (rManual && !rForceBtm) {
				rForceBtm = TRUE;
				//If the other side is already in manual, give priority to that side
				//otherwise reset fiducial count
				if (rForceTop)
					rIsBtm = FALSE;
				else
					rReset = TRUE;
			}

		}
		if (rpSkipBoard || rpExit) {
			//CLOSE MANUAL ALIGN WINDOW
			//FAIL BOARD
			if (rpExit)
				//TODO invalidate all results
				break;
		}

		//If BOTH sides are MANUAL and ONE side's fiducials have been LEARNT 
		if (!rReset && ((rpCurrFid == maxFid - 1) && rForceTop && rForceBtm)) {
			rIsTop = ~rIsTop;
			rIsBtm = ~rIsBtm;
			rForceTop = rIsTop;
			rForceBtm = rIsBtm;
			rpCurrFid = -1;
		}
		else if (rReset) {
			rpCurrFid = -1;
			rReset = FALSE;
		}
	}
	return TRUE;
}

int main() {
	rpCurrSide = TRUE;
	alignarea(TRUE, TRUE);
	getchar();
	return 0;
}
//
//# First law
//!(a&& b) == !a || !b
//# Second law
//!(a || b) == !a && !b
