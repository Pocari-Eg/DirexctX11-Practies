#pragma once


class InputClass;
class GraphicsClass;


class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();



private:
	LPCWSTR m_applicationName;

	HWND m_hwnd;

	InputClass* m_Input = nullptr;



};
static bool InitWinodw = false;
static GraphicsClass* m_Graphics = nullptr;
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;
static 	HINSTANCE m_hinstance;