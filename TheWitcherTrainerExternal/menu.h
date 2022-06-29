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

#include <array>
#include <thread>
#include <GLFW/glfw3.h>

#include "../offsets.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_custom.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../TheWitcherTrainerExternal/functions.h"

#define STB_IMAGE_IMPLEMENTATION
#define GL_CLAMP_TO_EDGE 0x812F

#define IMGUI_WINDOW_W 860
#define IMGUI_WINDOW_H 570

constexpr auto TRAINER_VERSION = "1.0.1";

class menu
{
public:
	void Initialize();

private:
	GLFWwindow* window_;
	GLFWwindow* window1_;
	int width_, height_;
	int display_w_, display_h_;
	ImFont* Ubuntu_Bold;

	void Draw();
	void Shutdown();
	void Style();
};

class settings
{
public:
	static inline bool is_check_update_ = true;
	static inline bool is_sound = true;
	static inline int style_idx_ = 0;
	static inline int lang_ = 0;
	static inline std::array<int, 14> key_bind_ { 97, 98, 99, 100, 101, 102, 103, 104, 105, 96, 110, 111, 106, 109 };
};