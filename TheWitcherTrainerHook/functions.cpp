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

BOOL Hook(LPVOID src, LPVOID funct, int size) {
	if (size < 5) { return FALSE; }
	DWORD old_protect, temp;
	VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &old_protect);
	memset(src, 0x90, size);
	DWORD relative_address = ((DWORD)funct - (DWORD)src) - 5;
	*(BYTE*)src = 0xE9;
	*(DWORD*)((DWORD)src + 1) = relative_address;
	VirtualProtect(src, size, old_protect, &temp);

	return TRUE;
}

void Patch(LPVOID src, LPCVOID dst, size_t size)
{
	DWORD old_protect;
	VirtualProtect((LPVOID)src, size, PAGE_EXECUTE_READWRITE, &old_protect);
	memcpy((LPVOID)src, (BYTE*)dst, size);
	VirtualProtect((LPVOID)src, size, old_protect, &old_protect);
}

void __declspec(naked) Hacks::GetLocalPlayer() {
	__asm {
		fld dword ptr ds:[ecx + 0x54]
		fadd dword ptr ds:[ecx + 0x48]
		cmp dword ptr ds:[ecx + 0x184], 0x3FE00000
		jne ex
		mov [local_player_], ecx

		ex:
		jmp jmp_back_health
	}
}

void __declspec(naked) Hacks::UnlimitedInventory() {
	__asm {
		mov dword ptr ds:[edi + 0x424], 0xFF
		jmp jmp_back_inventory
	}
}

void __declspec(naked) Hacks::UnlimitedOrens() {
	__asm {
		mov ecx, 0xF423F
		cmp ecx, [esi + 0x00000A40]
		js originalcode
		mov[esi + 0x00000A40], ecx

		originalcode:
		mov ecx, [esi + 0x00000A40]
		jmp jmp_back_orens
	}
}

void Hacks::Functions() {

	char data_dir[MAX_PATH];
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) & "TheWitcherTrainerHook.dll", &hModule);
	GetModuleFileName(hModule, data_dir, sizeof(data_dir));
	std::string::size_type pos = std::string(data_dir).find_last_of("\\/");
	std::string config_dir = std::string(data_dir).substr(0, pos);
	config_dir += std::string("\\config\\hook.json");

	DWORD module_witcher = (DWORD)GetModuleHandle("witcher.exe");

	int local_player_size = 6;
	DWORD local_player_health = module_witcher + 0x37A59D;
	jmp_back_health = local_player_health + local_player_size;

	int inventory_size = 6;
	DWORD inventory = module_witcher + 0x4EEF0C;
	jmp_back_inventory = inventory + inventory_size;

	int orens_size = 6;
	DWORD orens = module_witcher + 0x4EED5B;
	jmp_back_orens = orens + orens_size;

	bool is_temp_inventory_ = false;
	bool is_temp_get_local_player_ = false;
	bool is_temp_orens_ = false;

	json j;

	while (true)
	{
		std::ifstream l(config_dir);
		j = json::parse(l, nullptr, false, true);
		Config().ReadBool(j, "hooking", is_hooking_);
		Config().ReadBool(j, "enable_unlimited_inventory", is_unlimited_inventory_);
		Config().ReadBool(j, "enable_unlimited_orens", is_unlimited_orens_);

		if (!is_hooking_) break;

		if (!is_temp_get_local_player_)
		{
			Hook((LPVOID)local_player_health, Hacks::GetLocalPlayer, local_player_size);
			is_temp_get_local_player_ = true;
		}

		if (is_unlimited_inventory_ && !is_temp_inventory_) {
			Hook((LPVOID)inventory, Hacks::UnlimitedInventory, inventory_size);
			is_temp_inventory_ = true;
		}
		else if (!is_unlimited_inventory_ && is_temp_inventory_) {
			Patch((LPVOID)inventory, "\x8B\x8F\x24\x04\x00\x00", inventory_size);
			is_temp_inventory_ = false;
		}

		if (is_unlimited_orens_ && !is_temp_orens_) {
			Hook((LPVOID)orens, Hacks::UnlimitedOrens, orens_size);
			is_temp_orens_ = true;
		}
		else if (!is_unlimited_orens_ && is_temp_orens_) {
			Patch((LPVOID)orens, "\x8B\x8E\x40\x0A\x00\x00", orens_size);
			is_temp_orens_ = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	Patch((LPVOID)inventory, "\x8B\x8F\x24\x04\x00\x00", inventory_size);
	Patch((LPVOID)local_player_health, "\xD9\x41\x54\xD8\x41\x48", local_player_size);
	Patch((LPVOID)orens, "\x8B\x8E\x40\x0A\x00\x00", orens_size);
}