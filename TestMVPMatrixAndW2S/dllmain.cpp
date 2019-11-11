#pragma once

#include "pch.h"
#include "Overlay.h"
#include "DirectX.h"
#include "Const.h"

LRESULT CALLBACK WinProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

DWORD WINAPI MainThread(HMODULE hModule) {
	OverlayFunctions::GetTargetWindow(Const::windowName);
	OverlayFunctions::CreateClass(WinProcedure, (char*)"overlay", hModule);
	OverlayFunctions::CreateWindowOverlay(hModule);
	DXFunctions::Init(Overlay.Window);

	while(true) {
		if(PeekMessage(&Overlay.Message, Overlay.Window, 0, 0, PM_REMOVE)) {
			DispatchMessage(&Overlay.Message);
			TranslateMessage(&Overlay.Message);
		}

		Sleep(1);
		OverlayFunctions::GetTargetWindow(Const::windowName);
	}

	return 0;
}

LRESULT CALLBACK WinProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_PAINT: {
			DXFunctions::Render();
			break;
		} case WM_DESTROY: {
			PostQuitMessage(1);
			break;
		} default: {
			return DefWindowProc(hWnd, Message, wParam, lParam);
			break;
		}
	}

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			DisableThreadLibraryCalls(hModule);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
		}
	}

	return TRUE;
}
