#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false ;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

using namespace DirectX;
class D3DClass;
class CameraClass;
class Shaderclass;
class ModelLoader;
class Object;


class LightClass;

class GUIClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int);
	bool Render(float);

	bool WindowResize(int width, int height, HWND hwnd);

	void MouseInput(DIMOUSESTATE mouseState);




	//imguiControl
	bool LoadModeldData(HWND hwnd, std::string filename);
	void CameraZoomIO(float Distance);
	void SetDiffuseColor(float, float, float, float);
	void SetClearColor(float, float, float, float);
private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	ModelLoader* m_ModelLoader = nullptr;
	Shaderclass* m_Shader = nullptr;
	Object* m_Object = nullptr;

	LightClass* m_Light = nullptr;


	GUIClass* m_Gui = nullptr;

	XMFLOAT3 DefaultCameraPos;

	XMFLOAT4 ClearColor = { 0.45f, 0.55f, 0.60f, 1.0f };
};