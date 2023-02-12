#pragma once
#include <Windows.h>

bool Detour32(BYTE* src, BYTE* dst, size_t len);