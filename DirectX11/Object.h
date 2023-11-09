

#pragma once
#include "stdafx.h"

#include"Shaderclass.h"
#include"ModelLoader.h"




class Object
{
private:
	ModelLoader* m_Model;
	Shaderclass* m_Shader;

public:

	Object(){}
	Object(const Object&){}
	~Object(){}



	bool Initialize(ModelLoader* NewModel, Shaderclass* NewShader){

		if (!NewModel)return false;
		if (!NewShader)return false;

		m_Model = NewModel;
		m_Shader = NewShader;

		return true;
	}
	bool Draw(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,float r,XMFLOAT3 lightDirection,XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor,
		XMFLOAT3 CameraPos,XMFLOAT4 specularColor,float specularPower)
	{

		if (!m_Shader->Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix,lightDirection,diffuseColor, ambientColor,
			CameraPos,specularColor,specularPower))
		{
			return false;
		}


		for (int i = 0; i < m_Model->GetMeshCount(); i++) {


 			m_Model->Draw(deviceContext,i);
		}

	}

};

