#include "SpecialKeys.h"

bool SpecialKeys::KeyIsDownNow(UINT key)
{
	if (GetKeyState(key) < 0) // higher bit
		return true;
	else
		return false;
}

bool SpecialKeys::KeyWasDownBefore(UINT key)
{
	if ((GetKeyState(key) & 0x1) == 0) // higher bit
		return true;
	else
		return false;
}

bool SpecialKeys::KeyIsToggled(UINT key)
{
	if (GetKeyState(key) & 0x1) // lower bit
		return true;
	else
		return false;
}

