#pragma once

#include <Windows.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../TheWitcherTrainerExternal/menu.h"

namespace ImGuiCustom
{
	bool KeyBind(const char* label, int* k, const ImVec2& size_arg, int id_button);
	void TextURL(const char* label, const char* url);
}