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

#pragma once

#include <Windows.h>
#include <thread>
#include <string>

#include "config/config.h"

class Hacks
{
private:
	static inline DWORD jmp_back_health;
	static inline DWORD jmp_back_inventory;
	static inline DWORD jmp_back_orens;

	static inline bool is_hooking_ = false;
	static inline bool is_unlimited_inventory_ = false;
	static inline bool is_unlimited_orens_ = false;
	static inline DWORD local_player_;

	static void GetLocalPlayer();
	static void UnlimitedInventory();
	static void UnlimitedOrens();

public:
	void Functions();
};

