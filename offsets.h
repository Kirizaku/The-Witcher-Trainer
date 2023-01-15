/*
* Copyright (c) 2022 Rudeus Kirigaya <https://github.com/kirizaku>
*
* This program is free software: you can redistribute itand /or modify it under the terms of the GNU
* General Public License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.

* You should have received a copy of the GNU General Public License along with this program. If not, see
* <https://www.gnu.org/licenses/>.
*/

#pragma once
#include "memory/memory.h"

namespace globals {
	inline mem::mem_pid_t pid						= 0;
	inline uintptr_t process_mod					= 0;
	inline uintptr_t process_mod_hook				= 0;
	inline mem::string_t process_check				= "";
	inline const mem::string_t kProcessName			= "witcher.exe";
	inline const mem::string_t kModuleWitcherSteam	= "witcher.exe";
	inline const mem::string_t kModuleWitcherGOG	= "witcher.EXE";
	inline const mem::string_t kModuleHook			= "TheWitcherTrainerHook.dll";
}

namespace offsets {
#pragma region kModuleWitcher
	constexpr uintptr_t kFreeCamera1				= 0x85046C;
	constexpr uintptr_t kFreeCamera2				= 0x850470;
	constexpr uintptr_t kDebugMode					= 0x85068C;
	constexpr uintptr_t kWireFrame					= 0x850510;
	constexpr uintptr_t kGUI						= 0x850CD4;
#pragma endregion
#pragma region kModuleHook
	constexpr uintptr_t kLocalPlayer				= 0x166C4;
#pragma endregion

	constexpr uintptr_t kLpOrens					= 0xA40;

	constexpr uintptr_t kLpHealth					= 0x48;
	constexpr uintptr_t kLpCoordsX					= 0x18;
	constexpr uintptr_t kLpCoordsY					= 0x1C;
	constexpr uintptr_t kLpEndurance				= 0x1AC;
	constexpr uintptr_t kLpToxicity					= 0x1B4;
	constexpr uintptr_t kLpLevelXp					= 0x15c;
	constexpr uintptr_t kLpBronzePoints				= 0x294;
	constexpr uintptr_t kLpSilverPoints				= 0x298;
	constexpr uintptr_t kLpGoldPoints				= 0x29C;
}