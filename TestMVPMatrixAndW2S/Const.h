#pragma once
#include <Windows.h>

enum class W2S_Options_t {
	_OPENGL,
	_DIRECTX,
	_FOV
};

enum class TargetPlatform_t {
	X86,
	X64
};

struct float4x4 {
	float values[16];
};

namespace Const {
	// Window name (not the process name) of target game without the extension.
	constexpr LPCSTR windowName = "AssaultCube";

	// Is the process x86 or x64?
	// Also make sure to target correctly when builing the project.
	constexpr TargetPlatform_t targetPlatform = TargetPlatform_t::X86;

	// Screen width and height address, you can use hardcoded values if you don't have the addresses.
	const int screenWidth = *(int*)0x510C94;
	const int screenHeight = *(int*)0x510C98;

	// Select your World2Screen target.
	/*
		_OPENGL: Works good for OpenGL games.
		_DIRECTX: Works good for DirectX games.
		_FOV: You need FOV values and Left, Up and Forward vector.
	*/
	constexpr W2S_Options_t W2S_Option = W2S_Options_t::_OPENGL;

	// Entity list address.
	constexpr uintptr_t entityList = 0x50F4F8;

	// Entity list size address.
	const uintptr_t entityListSize = *(uintptr_t*)0x50F500;

	// Offset of where is the position/location/origin of the entity based on BaseLocalPlayer address from the EntityList.
	constexpr uintptr_t enemyPositionOffset = 0x34;

	// View Model Projection matrix address.
	const float4x4* const viewModelProjectionMatrix = (float4x4*)0x501AE8;
}