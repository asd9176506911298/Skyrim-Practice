#include "hook.h"

bool Detour32(BYTE* src, BYTE* dst, size_t len)
{
	if (len < 5)
		return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	uintptr_t relativeAddr = dst - src - 5;

	*src = 0xE9;

	*(uintptr_t*)(src + 1) = relativeAddr;

	VirtualProtect(src, len, curProtection, &curProtection);
	return true;
}