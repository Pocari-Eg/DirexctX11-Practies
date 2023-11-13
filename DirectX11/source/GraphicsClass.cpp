#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "Shaderclass.h"
#include "ModelLoader.h"
#include "Object.h"
#include "graphicsclass.h"
#include "LightClass.h"
#include"TextClass.h"

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



	
	m_ModelLoader = new ModelLoader;
	if(!m_ModelLoader->Load(hwnd, m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../DirectX11/models/sphere.dae"))
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

	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(0.9f, 0.9f, 0.9f, 0.9f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}


	// 카메라 포지션 설정
	m_Camera->SetPosition(0.0f, 0.0f, -6.0f);

	DefaultCameraPos = m_Camera->GetPosition();
	m_Camera->Render();
	XMMATRIX BaseViewMatrix;
	m_Camera->GetViewMatrix(BaseViewMatrix);
	if(!m_Text->Initialize(m_Direct3D->GetDevice(),m_Direct3D->GetDeviceContext(),hwnd,screenWidth,screenHeight, BaseViewMatrix)){
	
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{


	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

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

	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	// Direct3D 객체 반환
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}


	
}


bool GraphicsClass::Frame(int mouseX,int mouseY)
{
	static float rotation = 0.0f;

	// 각 프레임의 rotation 변수를 업데이트합니다.
	rotation += (float)XM_PI * 0.002f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	if (!m_Text->SetMousePosition(mouseX, mouseY, m_Direct3D->GetDeviceContext()))
	{
		return false;
	}

	// 그래픽 랜더링 처리

	return Render(rotation);
}

bool GraphicsClass::WindowResize(int width, int height,HWND hwnd)
{

	// Direct3D 객체 초기화
	if (m_Direct3D->ReSize(width, height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{

		return true;
	}
	
	return false;
}

void GraphicsClass::MouseInput(DIMOUSESTATE mouseState)
{
	const float ZoomValue = 0.3f;

	XMFLOAT3 CameraPos = m_Camera->GetPosition();
	const XMFLOAT3 cameraForwardVec = m_Camera->GetCameraFoward();

	if (mouseState.lZ > 0)
	{
		XMFLOAT3 temp = CameraPos;
		CameraPos.x += cameraForwardVec.x* ZoomValue;
		CameraPos.y += cameraForwardVec.y * ZoomValue;
		CameraPos.z += cameraForwardVec.z * ZoomValue;
		if (DefaultCameraPos.x < CameraPos.x || DefaultCameraPos.y < CameraPos.y || DefaultCameraPos.z < CameraPos.z)
		{
			CameraPos = temp;
		}
	}
	else if (mouseState.lZ < 0)
	{
	

		CameraPos.x -= cameraForwardVec.x * ZoomValue;
		CameraPos.y -= cameraForwardVec.y * ZoomValue;
		CameraPos.z -= cameraForwardVec.z * ZoomValue;


	}

	m_Camera->SetPosition(CameraPos.x,CameraPos.y,CameraPos.z);

}


bool GraphicsClass::Render(float rotation)
{
	// 씬을 그리기 위해 버퍼를 지웁니다
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 월드, 뷰 및 투영 행렬을 가져옵니다
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	// 삼각형이 회전 할 수 있도록 회전 값으로 월드 행렬을 회전합니다.
	
	m_Direct3D->TurnZBufferOff();
	m_Direct3D->TurnOnAlphaBlending();


	if (!m_Text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix))
	{
		return false;
	}

	m_Direct3D->TurnOffAlphaBlending();
	m_Direct3D->TurnZBufferOn();

   worldMatrix = XMMatrixRotationY(rotation);

   LightData m_LightData = { m_Light->GetDirection(), m_Light->GetDffiuseColor(), m_Light->GetAmbientColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(),m_Light->GetSpecularPower() };

	m_Object->Draw(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_LightData);


	

	// 버퍼의 내용을 화면에 출력합니다
	m_Direct3D->EndScene();

	return true;
}