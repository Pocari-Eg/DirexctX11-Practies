#pragma once
#include<iostream>
#include<imgui/imgui.h>
#include<imgui/imconfig.h>
#include<imgui/imgui_internal.h>
#include<imgui/imstb_rectpack.h>
#include<imgui/imstb_textedit.h>
#include<imgui/imstb_truetype.h>
#include<imgui/FileBrowser/ImGuiFileBrowser.h>
#include<imgui/imgui_impl_dx11.h>
#include<imgui/imgui_impl_win32.h>
#include"stdafx.h"



class GraphicsClass;
class GUIClass
{
public:
	GUIClass();
	GUIClass(const GUIClass&);
	~GUIClass();

	bool Initialize(GraphicsClass*,HWND, ID3D11Device*, ID3D11DeviceContext*, ID3D11RenderTargetView*);
	void Shutdown();
	void Draw();
private:
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	HWND m_hwnd;

	imgui_addons::ImGuiFileBrowser file_dialog;
	ImGuiIO* m_io;
	ID3D11RenderTargetView* g_mainRenderTargetView;

	GraphicsClass* m_Graphics = nullptr;
};

