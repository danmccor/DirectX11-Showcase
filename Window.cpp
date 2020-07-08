#include "Window.h"
//Initialise the window
Window::Window()
{
	//Set up raw input
	static bool raw_input_initialised = false;
	if (!raw_input_initialised) {
		//Set a raw input device
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;
		//Register the raw input devices
		RegisterRawInputDevices(&rid, 1, sizeof(rid));
		raw_input_initialised = true;
	}
}

bool Window::InitialiseWindow(HINSTANCE hInstance, std::string wTitle, std::string wClass, int width, int height)
{
	//ASSIGN CLASS VARIABLES
	this->hInstance = hInstance;
	this->wTitle = wTitle;
	this->width = width;
	this->height = height;
	this->wClass = wClass;

	//CONVERT STRINGS TO CHAR ARRAY
	strcpy_s(wTitle_char, wTitle.c_str());
	strcpy_s(wClass_char, wClass.c_str());
	
	std::wstring tempTitle = std::wstring(wTitle.begin(), wTitle.end());
	std::wstring tempClass = std::wstring(wClass.begin(), wClass.end());

	titleName = tempTitle.c_str();
	className = tempClass.c_str();

	//CREATE WINDOW CLASS
	this->WindowClass();

	//Create window rectangle
	int centreScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	int centreScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
	RECT wr;
	wr.left = centreScreenX;
	wr.top = centreScreenY;
	wr.right = wr.left + width;
	wr.bottom = wr.top + height;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	

	//CREATE HANDLE
	handle = CreateWindowEx(0, className, titleName, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, this);
	if (handle == NULL) {
		return false;
	}

	//Show the window
	ShowWindow(handle, SW_SHOW);
	//Set the winodw in the foreground
	SetForegroundWindow(handle);
	//Set the window as the focus
	SetFocus(handle);
	return true;
}


//Manage window messges
bool Window::ManageMessages()
{
	MSG msg; 
	ZeroMemory(&msg, sizeof(MSG));
	//While there are messages to peek
	while (PeekMessage(&msg, handle, 0, 0, PM_REMOVE)) {
		//Translate the message
		TranslateMessage(&msg);
		//Dispatch the message
		DispatchMessage(&msg);
	}
	//If message is null
	if (msg.message == WM_NULL) {
		//if window handle has been destroyed
		if (!IsWindow(handle)) {
			//set handle to null
			handle = NULL;
			//Unregister the class
			UnregisterClass(className, hInstance);
			return false;
		}
	}
	return true;
}



LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		//Keyboard
		//if there is a key down
	case WM_KEYDOWN:
	{
		//get character of key
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (keyboard.IsKeysAutoRepeat()) {
			//if auto repeat is on send ch
			keyboard.OnKeyPressed(ch);
		}
		else {
			//if it is not, to true or false inside lParam
			const bool wasPressed = lParam & 0X40000000;
			//if was not pressed
			if (!wasPressed) {
				//send ch
				keyboard.OnKeyPressed(ch);
			}
		}
		return 0;
	}
	//if key is released
	case WM_KEYUP:
	{
		//get character
		unsigned char ch = static_cast<unsigned char>(wParam);
		//send character
		keyboard.OnKeyReleased(ch);
		return 0;
	}
	case WM_CHAR:
	{
		//Char same as keydown except will pass the key value
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (keyboard.IsCharsAutoRepeat()) {
			keyboard.OnChar(ch);
		}
		else {
			const bool wasPressed = lParam & 0X40000000;
			if (!wasPressed) {
				keyboard.OnChar(ch);
			}
		}
		return 0;
	}
	//Mouse

	//The x Positions of the mouse are stored in the LOWORD, while y in HIWORD
	case WM_MOUSEMOVE:
	{
		//If the mouse has moved, update the mouse position
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		//If the mouse has clicked, update where the mouse clicked
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		//if the mouse has clicked, update where the mouse clicked
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		//if scroll wheel click, update where the mouse clicked
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		//if the mouse is released, update where the mouse was released
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		//if the mouse was released, update where the mouse was released
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		//if scroll wheel released, update where the mouse was released
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		//if the scroll wheel moved
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		//get how much the wheel was moved up
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
			mouse.OnWheelUp(x, y);
		}
		//get how much the wheel was moved down
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
			mouse.OnWheelDown(x, y);
		}
		break;
	}
	case WM_INPUT:
	{
		//Get mouse raw input to the datasize variable
		UINT dataSize;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
		//if data size is greater than 0
		if (dataSize > 0) {
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			//Get the raw input of the mouse
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE) {
					//update the mouse class with the raw data input
					mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}
		//Return default window proc
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	default:
		//by default return default window proc
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	//return default window proc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::~Window() {
	//on window close
	if (this->handle != NULL) {
		//unregister the class and destroy windows
		UnregisterClass(className, hInstance);
		DestroyWindow(handle);
	}

}

LRESULT CALLBACK HandleMsgRedirect(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		//if umsg is WM_CLOSE
	case WM_CLOSE:
		//destroy window
		DestroyWindow(hWnd);
		return 0;

	default:
		Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return window->WindowProc(hWnd, uMsg, wParam, lParam);

	}

}

LRESULT CALLBACK MessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* window = reinterpret_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return window->WindowProc(hWnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

}

void Window::WindowClass()
{
	//CREATE WINDOW CLASS
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW;
	wc.lpfnWndProc = MessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = className;
	RegisterClassEx(&wc);

}