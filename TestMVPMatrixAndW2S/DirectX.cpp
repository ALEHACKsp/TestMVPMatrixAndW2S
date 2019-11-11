#include "pch.h"
#include "Const.h"
#include "DirectX.h"
#include "Overlay.h"
#include "Vector3.h"
#include "Mem.h"
#include "Vector4.h"

constexpr float M_PI = 3.1415927f;

dx_t DirectX;
overlay_t Overlay;
target_t Target;

void DXFunctions::Init(HWND hwnd) {
	if(FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX.Object))) {
		exit(1);
	}

	ZeroMemory(&DirectX.Param, sizeof(DirectX.Param));
	DirectX.Param.Windowed = true;
	DirectX.Param.BackBufferFormat = D3DFMT_A8R8G8B8;
	DirectX.Param.BackBufferHeight = Overlay.Height;
	DirectX.Param.BackBufferWidth = Overlay.Width;
	DirectX.Param.EnableAutoDepthStencil = true;
	DirectX.Param.AutoDepthStencilFormat = D3DFMT_D16;
	DirectX.Param.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	DirectX.Param.SwapEffect = D3DSWAPEFFECT_DISCARD;

	if(FAILED(DirectX.Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectX.Param, 0, &DirectX.Device))) {
		exit(1);
	}
	
	if(!DirectX.espFont) {
		D3DXCreateFont(DirectX.Device, 13, 0, 0, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial", &DirectX.espFont);
	}

	if(!DirectX.Line) {
		D3DXCreateLine(DirectX.Device, &DirectX.Line);
	}
}

void DrawString(char* String, int x, int y, int a, int r, int g, int b, ID3DXFont* font) {
	RECT textPosition;
	textPosition.left = x;
	textPosition.top = y;

	font->DrawTextA(0, String, strlen(String), &textPosition, DT_NOCLIP, D3DCOLOR_ARGB(a, r, g, b));
}

Vector3 Subtract(Vector3 source, Vector3 target) {
	Vector3 difference;

	difference.x = source.x - target.x;
	difference.y = source.y - target.y;
	difference.z = source.z - target.z;

	return difference;
}

float DotProduct(Vector3 source, Vector3 target) {
	return (source.x * target.x) + (source.y * target.y) + (source.z * target.z);
}

// Works good for OpenGL
bool WorldToScreen(Vector3 pos, Vector3& screen, const float4x4* matrix, const int windowWidth, const int windowHeight) {
	Vector4 clipCoords;

	if(Const::W2S_Option == W2S_Options_t::_OPENGL) {
		clipCoords.x = pos.x * matrix->values[0] + pos.y * matrix->values[4] + pos.z * matrix->values[8] + matrix->values[12];
		clipCoords.y = pos.x * matrix->values[1] + pos.y * matrix->values[5] + pos.z * matrix->values[9] + matrix->values[13];
		clipCoords.z = pos.x * matrix->values[2] + pos.y * matrix->values[6] + pos.z * matrix->values[10] + matrix->values[14];
		clipCoords.w = pos.x * matrix->values[3] + pos.y * matrix->values[7] + pos.z * matrix->values[11] + matrix->values[15];
	} else if(Const::W2S_Option == W2S_Options_t::_DIRECTX) {
		clipCoords.x = pos.x * matrix->values[0] + pos.y * matrix->values[1] + pos.z * matrix->values[2] + matrix->values[3];
		clipCoords.y = pos.x * matrix->values[4] + pos.y * matrix->values[5] + pos.z * matrix->values[6] + matrix->values[7];
		clipCoords.z = pos.x * matrix->values[8] + pos.y * matrix->values[9] + pos.z * matrix->values[10] + matrix->values[11];
		clipCoords.w = pos.x * matrix->values[12] + pos.y * matrix->values[13] + pos.z * matrix->values[14] + matrix->values[15];
	}

	if(clipCoords.w < 0.1f) {
		return false;
	}

	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	int halfWindowWidth = windowWidth / 2;
	int halfWindowHeight = windowHeight / 2;

	screen.x = (halfWindowWidth * NDC.x) + (NDC.x + halfWindowWidth);
	screen.y = -(halfWindowHeight * NDC.y) + (NDC.y + halfWindowHeight);

	return true;
}

// Using FOV and Left, Up and Forward vector
bool WorldToScreen(Vector3 source, Vector3 target, Vector3& screen, float fovX, float fovY, float windowWidth, float windowHeight, Vector3 left, Vector3 up, Vector3 forward) {
	Vector3 transform;
	float xc;
	float yc;
	float px;
	float py;
	float z;

	px = tan(fovX * M_PI / 360.f);
	py = tan(fovY * M_PI / 360.f);

	transform = Subtract(target, source);

	xc = windowWidth / 2.f;
	yc = windowHeight / 2.f;

	z = DotProduct(transform, left);

	if(z <= 0.1) {
		return false;
	}

	screen.x = xc - DotProduct(transform, up) * xc / (z * px);
	screen.y = yc - DotProduct(transform, forward) * yc / (z * py);

	return true;
}

void DXFunctions::Render() {
	DirectX.Device->BeginScene();

	if(GetForegroundWindow() == Target.Window) {
		for(unsigned int i = 1; i < Const::entityListSize; i++) {
			Vector3 enemyPosition = *(Vector3*)FindDMAAddy(Const::entityList, {(i * ((Const::targetPlatform == TargetPlatform_t::X86) ? 4u : 8u)), static_cast<unsigned int>(Const::enemyPositionOffset)});
			Vector3 vScreen;

			if(Const::W2S_Option == W2S_Options_t::_OPENGL || Const::W2S_Option == W2S_Options_t::_DIRECTX) {
				if(WorldToScreen(enemyPosition, vScreen, Const::viewModelProjectionMatrix, Const::screenWidth, Const::screenHeight)) {
					DrawString((char*)"Entity", static_cast<int>(vScreen.x), static_cast<int>(vScreen.y), 255, 255, 255, 255, DirectX.espFont);
				}
			} else if(Const::W2S_Option == W2S_Options_t::_FOV) {
				// TODO: Finish this part
			}
		}
	}

	DirectX.Device->EndScene();
	DirectX.Device->PresentEx(0, 0, 0, 0, 0);
	DirectX.Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
}