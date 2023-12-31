// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

#endif // UNIICODE
#endif // _DEBUG


#define _WIN32_WINNT 0x0600				// <== 추가
#define DIRECTINPUT_VERSION 0x0800


#include "targetver.h"
#include<xstring>
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "resource.h"

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "DxDefine.h"

//input
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<dinput.h>

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct  LightData
{

	XMFLOAT3 lightDirection;
	XMFLOAT4 diffuseColor;
	XMFLOAT4 ambientColor;
	XMFLOAT3 CameraPos;
	XMFLOAT4 specularColor;
	float specularPower;

};