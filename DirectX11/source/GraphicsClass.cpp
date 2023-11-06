#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "Shaderclass.h"
#include "ModelLoader.h"
#include "Object.h"
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if(!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}
	
	

	// m_Camera 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// 카메라 포지션 설정
	m_Camera->SetPosition(0.0f, 0.0f,-5.0f);
	m_Camera->SetRotation(0, 0.0f, 0.0f);
	
	m_ModelLoader = new ModelLoader;
	if(!m_ModelLoader->Load(hwnd, m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../DirectX11/models/cubes_with_names.fbx"))
	{
		 MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
	}

	// m_Shader 객체 생성
	m_Shader = new Shaderclass;
	if (!m_Shader)
	{
		return false;
	}

	// m_Shader 객체 초기화
	if (!m_Shader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	m_Object = new Object;
	if(!m_Object)
	{
		return false;
	}
	if (!m_Object->Initialize(m_ModelLoader, m_Shader))
	{
		MessageBox(hwnd, L"Could not initialize the Main object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{

	// m_Shader 객체 반환
	if (m_Shader)
	{
		m_Shader->Shutdown();
		delete m_Shader;
		m_Shader = 0;
	}


	// m_Camera 객체 반환
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Object)
	{
		delete m_Object;
		m_Object = 0;
	}
	// Direct3D 객체 반환
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	
}


bool GraphicsClass::Frame()
{
	static float rotation = 0.0f;

	// 각 프레임의 rotation 변수를 업데이트합니다.
	rotation += (float)XM_PI * 0.01f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// 그래픽 랜더링 처리
	return Render(rotation);
}

bool GraphicsClass::WindowResize(int width, int height,HWND hwnd)
{

	// Direct3D 객체 초기화
	if (m_Direct3D->ReSize(width, height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{

	/*	int NewZ = width/100;
		NewZ *= 625;
		NewZ /= 1000;
		NewZ *= -1;
		m_Camera->SetPosition(0.0f, 0.0f, (float)NewZ);*/
		return true;
	}
	
	return false;
}


bool GraphicsClass::Render(float rotation)
{
	// 씬을 그리기 위해 버퍼를 지웁니다
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// 삼각형이 회전 할 수 있도록 회전 값으로 월드 행렬을 회전합니다.
	//

	m_Object->Draw(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, rotation);

	// 버퍼의 내용을 화면에 출력합니다
	m_Direct3D->EndScene();

	return true;
}