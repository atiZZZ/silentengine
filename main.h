#pragma once 
#include <Windows.h>
#include <iostream>

#pragma region d3d9
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma endregion

#pragma region Detours
#include "Detours/detours.h"
#include "Detours/Patternscaning.h"
#pragma endregion

#pragma region CryEngine
#include "CryEngine/cSDK.h"
#include "CryEngine/cClasses.h"
#include "CryEngine/cFunction.h"
#pragma endregion

#pragma region f_EndScene
const char* windowName = "Warface";

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall * f_EndScene)(IDirect3DDevice9 * pDevice); // Our function prototype 
f_EndScene oEndScene; // Original Endscene

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

WNDPROC oWndProc;
#pragma endregion