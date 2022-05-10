/*
* Copyright (c) 2022 Rudeus Kirigaya <https://github.com/kirizaku>
*
* This file is part of TheWitcherTrainerExternal.
* TheWitcherTrainerExternal is free software: you can redistribute it and/or modify it under the terms of the GNU General 
* Public License as published by the Free Software Foundation, either version 3 of the License, or (at your 
* option) any later version.
* 
* TheWitcherTrainerExternal is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even 
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along with TheWitcherTrainerExternal. If not, see 
<https://www.gnu.org/licenses/>.
*/

#include "functions.h"

DWORD WINAPI MainThread(LPVOID lpReserved) {
	Hacks().Functions();
	FreeLibraryAndExitThread((HMODULE)lpReserved, 0);
	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		break;
	} return TRUE;
}