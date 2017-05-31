#pragma  comment(lib,"Wtsapi32.lib")
#include "SessionInspector.h"
#include <windows.h>
#include <Wtsapi32.h>

constexpr LPCSTR TERMINAL_SERVER_KEY = "SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\";
constexpr LPCSTR GLASS_SESSION_ID = "GlassSessionId";

using namespace SI;

bool SessionInstector::IsCurrentSessionRemoteable()
{
	bool is_remotable = false;
	if (GetSystemMetrics(SM_REMOTESESSION))
	{
		is_remotable = true;
	}
	else
	{
		HKEY reg_key = NULL;
		LONG result;

		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TERMINAL_SERVER_KEY,
			0, KEY_READ, &reg_key);

		if (result == ERROR_SUCCESS)
		{
			DWORD dw_glass_session_id = 0;
			DWORD cb_glass_session_id = sizeof(dw_glass_session_id);
			DWORD type = 0;

			result = RegQueryValueEx(reg_key, GLASS_SESSION_ID, NULL, &type
				, (BYTE*)&dw_glass_session_id, &cb_glass_session_id);

			if (result == ERROR_SUCCESS)
			{
				DWORD dw_current_session_id = 0;

				if (ProcessIdToSessionId(GetCurrentProcessId(), &dw_current_session_id))
				{
					is_remotable = (dw_current_session_id != dw_glass_session_id);
				}
			}
		}
		if (reg_key)
		{
			RegCloseKey(reg_key);
		}
	}
	return is_remotable;
}

bool SessionInstector::IsInsideVBox()
{
	if (CreateFile("\\\\.\\VBoxMiniRdrDN", GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) != INVALID_HANDLE_VALUE)
	{
		return true;
	}

	else
	{
		return false;
	}
}