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

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	bool WindowResize(int width, int height,HWND hwnd);

private:
	bool Render(float);

private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelLoader* m_ModelLoader = nullptr;
	Shaderclass* m_Shader = nullptr;
	Object* m_Object = nullptr;

};