#ifndef DIRECTX
#define DIRECTX

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


struct dx_t {
	IDirect3D9Ex* Object;
	IDirect3DDevice9Ex* Device;
	D3DPRESENT_PARAMETERS Param;
	ID3DXFont* espFont;
	ID3DXLine* Line;
};
extern dx_t DirectX;

namespace DXFunctions {
	void Init(HWND hwnd);
	void Render();
}

#endif