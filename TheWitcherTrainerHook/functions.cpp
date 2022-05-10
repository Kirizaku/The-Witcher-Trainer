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

void __declspec(naked) Hacks::UnlimitedInventory() {
	__asm {
		mov dword ptr ds:[edi+0x424], 0xFF
		jmp jmp_back_inventory
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

	int inventory_size = 6;
	DWORD inventory = module_witcher + 0x4EEF0C;
	jmp_back_inventory = inventory + inventory_size;

	bool temp_check = false;

	json j;

	while (true)
	{
		std::ifstream l(config_dir);
		j = json::parse(l, nullptr, false, true);
		Config().ReadBool(j, "hooking", is_hooking_);
		Config().ReadBool(j, "enable_unlimited_inventory", is_unlimited_inventory_);

		if (!is_hooking_) break;

		if (is_unlimited_inventory_ && !temp_check) {
			Hook((LPVOID)inventory, Hacks::UnlimitedInventory, inventory_size);
			temp_check = true;
		}
		else if (!is_unlimited_inventory_ && temp_check) {
			Patch((LPVOID)inventory, "\x8B\x8F\x24\x04\x00\x00", inventory_size);
			temp_check = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	Patch((LPVOID)inventory, "\x8B\x8F\x24\x04\x00\x00", inventory_size);
}