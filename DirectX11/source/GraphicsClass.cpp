#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "Shaderclass.h"
#include "ModelLoader.h"
#include "Object.h"
#include "graphicsclass.h"
#include "LightClass.h"


#include"GUIClass.h"

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
	// Direct3D ��ü ����
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}



	// Direct3D ��ü �ʱ�ȭ
	if(!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}
	
	m_Gui = new GUIClass;
	if (!m_Gui)
	{
		return false;
	}

	m_Gui->Initialize(this,hwnd, m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(),m_Direct3D->GetRenderTargetView());


	// m_Camera ��ü ����
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}


	// m_Shader ��ü ����
	m_Shader = new Shaderclass;
	if (!m_Shader)
	{
		return false;
	}

	// m_Shader ��ü �ʱ�ȭ
	if (!m_Shader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}


	m_ModelLoader = new ModelLoader;
	if (!m_ModelLoader)
	{
		return false;
	}

	m_Object = new Object;
	if (!m_Object)
	{
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




	// ī�޶� ������ ����
	m_Camera->SetPosition(0.0f, 0.0f, -6.0f);

	DefaultCameraPos = m_Camera->GetPosition();
	m_Camera->Render();
	XMMATRIX BaseViewMatrix;
	m_Camera->GetViewMatrix(BaseViewMatrix);


	return true;
}


void GraphicsClass::Shutdown()
{


	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// m_Shader ��ü ��ȯ
	if (m_Shader)
	{
		m_Shader->Shutdown();
		delete m_Shader;
		m_Shader = 0;
	}


	// m_Camera ��ü ��ȯ
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


	if (m_Gui)
	{
		m_Gui->Shutdown();
		delete m_Gui;
		m_Gui = 0;
	}

	// Direct3D ��ü ��ȯ
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

	// �� �������� rotation ������ ������Ʈ�մϴ�.
	rotation += (float)XM_PI * 0.002f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	

	// �׷��� ������ ó��

	return Render(rotation);
}

bool GraphicsClass::WindowResize(int width, int height,HWND hwnd)
{

	// Direct3D ��ü �ʱ�ȭ
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

bool GraphicsClass::LoadModeldData(HWND hwnd, std::string filename)
{

	m_ModelLoader = new ModelLoader;
	if (!m_ModelLoader)
	{
		return false;
	}

	m_Object = new Object;
	if (!m_Object)
	{
		return false;
	}

	if (!m_ModelLoader->Load(hwnd,m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), filename))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	if (!m_Object->Initialize(m_ModelLoader, m_Shader))
	{
		MessageBox(hwnd, L"Could not initialize the Main object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::CameraZoomIO(float Distance)
{

	XMFLOAT3 CameraPos = DefaultCameraPos;
	const XMFLOAT3 cameraForwardVec = m_Camera->GetCameraFoward();

	


	CameraPos.x -= cameraForwardVec.x * Distance;
	CameraPos.y -= cameraForwardVec.y * Distance;
	CameraPos.z -= cameraForwardVec.z * Distance;


	m_Camera->SetPosition(CameraPos.x, CameraPos.y, CameraPos.z);
}

void GraphicsClass::SetDiffuseColor(float r, float g, float b , float a)
{
	m_Light->SetDiffuseColor(r, g, b, a);
}

void GraphicsClass::SetClearColor(float r,  float g, float b, float a)
{
	ClearColor = { r,g,b,a };
}


bool GraphicsClass::Render(float rotation)
{

	// ���� �׸��� ���� ���۸� ����ϴ�
	m_Direct3D->BeginScene(ClearColor);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	// �ﰢ���� ȸ�� �� �� �ֵ��� ȸ�� ������ ���� ����� ȸ���մϴ�.
	





   worldMatrix = XMMatrixRotationY(rotation);

   if (m_Object->IsReady()) {
	   LightData m_LightData = { m_Light->GetDirection(), m_Light->GetDffiuseColor(), m_Light->GetAmbientColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(),m_Light->GetSpecularPower() };
	   m_Object->Draw(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_LightData);
   }

	m_Gui->Draw();
	

	// ������ ������ ȭ�鿡 ����մϴ�
	m_Direct3D->EndScene();

	return true;
}