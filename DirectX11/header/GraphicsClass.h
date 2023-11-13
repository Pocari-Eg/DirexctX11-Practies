#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false ;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class CameraClass;
class Shaderclass;
class ModelLoader;
class Object;

class TextClass;
class LightClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int,int);
	bool Render(float);
	
	bool WindowResize(int width, int height,HWND hwnd);

	void MouseInput(DIMOUSESTATE mouseState);
private:



private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelLoader* m_ModelLoader = nullptr;
	Shaderclass* m_Shader = nullptr;
	Object* m_Object = nullptr;

	LightClass* m_Light = nullptr;

	TextClass* m_Text = nullptr;


	XMFLOAT3 DefaultCameraPos;

};