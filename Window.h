#include <Windows.h>
#include <iostream>
#include "Input.h"

#pragma once
class Window
{
public:
	//Default constructor
	Window();
	//Initialise the window
	bool InitialiseWindow(HINSTANCE hInstance, std::string wTitle, std::string wClass, int width, int height);
	//Manage the window messages
	bool ManageMessages();
	//Get the window handle
	HWND GetHandle() { return handle; };
	//Get the window width and height
	int GetWidth() { return width; };
	int GetHeight() { return height; };
	//WindowProc for getting inputs to the window
	LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//Create the keyboard and mouse
	Keyboard keyboard;
	Mouse mouse;
	~Window();

private: 
	//Functions for creating the window class
	void WindowClass();
	//Variables for creating window
	HWND handle = NULL;
	int width = 0;
	int height = 0;
	HINSTANCE hInstance = NULL;
	std::string wTitle = "";
	char wTitle_char[100];
	std::string wClass = "";
	char wClass_char[100];
	
	LPCWSTR titleName;
	LPCWSTR className;
	
};

