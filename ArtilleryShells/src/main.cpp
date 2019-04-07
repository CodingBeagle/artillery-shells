/*
	For all functions with text arguments, applications should normally use the generic function prototypes.

	If an application defines UNICODE BEFORE including the header (like Windows.h), these functions will be compiled into
	the Unicode version of those functions.
*/
#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*
	Every Windows program includes an entry-point function.

	It's named either WinMain or wWinMain.

	WinMain and wWinMain are identical in function. The only difference is that the command line arguments are
	passed as ANSI in "WinMain" and as Unicode in "wWinMain". Unicode is to be preferred.

	WINAPI = Calling convention. A calling convention defines how a function receives its parameters from the caller,
	like the order that parameters appear on the stack.

	In order to make use of the wWinMain entry point, remember that the SUBSYSTEM parameter for the project has to be "Windows".
	The SUBSYSTEM parameter determines the entry point to be used.
*/
int WINAPI wWinMain(
	HINSTANCE hInstance, // Handle to an instance. The OS uses this handle to identify the EXE when it is loaded in memory.
	HINSTANCE hPrevInstance, // hPrevInstance. It has no meaning. Not used. Still present due to historical reasons. 
	PWSTR pCmdLine, // Contains the command-line argument as unicode string
	int nCmdShow) // A flag that says whether the main application window will be minimized, maximized, or shown normally
{
	/*
		A Window Class defines a set of behaviours that several windows might have in common.

		Even though we only want to create a single main window, the WinAPI requires that we still register a Window Class which it
		will use.
	*/

	// The L prefix denotes a wide character/string literal. That is, type of wchar_t instead of char.
	const wchar_t WINDOW_CLASS_NAME[] = L"Game Window Class";

	/*
		WNDCLASSEX contains window class information to be registered with RegisterClassEx.
	*/
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX); // The size of the structure in bytes. Always set to this value.
	wc.style = 0; // Class Style. It defines additional elements of the window behaviour. See: https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-class-styles
	wc.lpfnWndProc = WndProc; // A pointer to the window procedure.
	wc.cbClsExtra = 0; // Number of extra bytes to allocate following the window-class structure.
	wc.cbWndExtra = 0; // The number of extra bytes to allocate following the window instance.
	wc.hInstance = hInstance; // A handle to the instance that contains the window procedure for the class.
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION); // A handle to the class icon. If null, the system provides a default icon.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // A handle to the class cursor. IDC_ARROW = Standard Windows arrow.
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);; // A handle to the class background brush. It can be used for painting the background.
	wc.lpszMenuName = nullptr; // Pointer to null-terminated string specifying the resource name of the class menu. If NULL, windows belonging to this window class have no default menu.
	wc.lpszClassName = WINDOW_CLASS_NAME; // A pointer to a null-terminated string specifying the class name.
	wc.hIconSm = nullptr; // Handle to a small icon that is associated with the window class. If null, the system searches the icon resource specified by hIcon.

	// Now that we've prepared the WNDCLASSEX struct, we can register the window class
	// If the function fails, 0 is returned.
	auto windowClassHandle = RegisterClassEx(&wc);
	if (!windowClassHandle)
		return -1;

	/*
		Now that the window class has been registered, we have create a window! :D

		CreateWindowEx will return NULL if creation failed.
	*/
	auto mainWindowHandle = CreateWindowEx(
		0, // Optional extended windows styles
		WINDOW_CLASS_NAME, // The name of the window class to base this window upon
		L"Artillery Shells", // Window title. To show the window title you need to choose a window style which supports it
		WS_OVERLAPPEDWINDOW, // Style of the window being created. An "OVERLAPPEDWINDOW" style has a border and a title bar, and also a system menu.
		// x, y, width, height. CW_USEDEFAULT will let the system pick a default value. This is only supported for WS_OVERLAPPED styled windows.
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		nullptr, // Handle to a parent window. We don't have any. This IS the parent >: )
		nullptr, // Handle to a menu. NULL means the window class menu will be used.
		hInstance, // Handle to this application
		nullptr // Additional application data that can be passed to the window procedure. Not needed for us.
	);

	if (!mainWindowHandle)
		return -1;

	// Let's show the window! :D
	ShowWindow(mainWindowHandle, nCmdShow);

	// Main game loop!
	bool bGotMsg;
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (msg.message != WM_QUIT) {
		// Process window events
		// Use PeekMessage() so we can use idle time to render the scene
		bGotMsg = (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE) != 0);

		if (bGotMsg) {
			// Translate and dispatch message
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// Update scene
			// renderer->Update();

			// Render frames
			// renderer->Render();

			// Present frame to screen
			// deviceResources->Present()
		}
	}

	return 0;
}

/*
	Windows uses a message-parsing model. The operating system communicates with your application by passing messages to it.

	Message = A numeric code which designates a particular event.

	Messages can be related to user events (like keystrokes, mouse clicks, etc...), or events from the operating system,
	like resizing the window, new hardware being plugged in, the operating system going to hibernate, etc, etc...

	For each thread that creates a window the operating system creates a queue for window messages.
	The queue holds messages for all the windows that are created on that thread.

	To get messages, an application needs a message loop which retrieves and dispatches these messages.

	The Window Class associated with a window determines the window procedure.
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	/*
		Any messages that we do not process should be passed along to the default window procedure.

		This function provides some fundamental behaviour shared by all windows.
	*/
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
