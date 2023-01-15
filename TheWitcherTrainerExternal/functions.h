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

class Hacks
{
public:
	static inline bool is_running_ = false;
	static inline bool is_hooking_ = false;

	static inline bool is_unlimited_health_ = false;
	static inline bool is_unlimited_orens_ = false;
	static inline bool is_unlimited_endurance_ = false;
	static inline bool is_unlimited_points_ = false;
	static inline bool is_unlimited_inventory_ = false;
	static inline bool is_free_camera_ = false;
	static inline bool is_toxicity_ = false;
	static inline bool is_debug_mode_ = false;
	static inline bool is_wire_frame_ = false;
	static inline bool is_gui_ = false;

	static inline uintptr_t	local_player = 0;

	static inline std::array<uintptr_t, 2> camera_pos_xy_ = { 0,0 };

	static inline float x_, y_ = 0.0f;
	static inline float real_time_x_, real_time_y_ = 0.0f;
	static inline float custom_x_, custom_y_ = 0.0f;

	void Functions();

private:
	bool is_temp_freecamera_ = false;
	bool is_temp_inventory_ = false;
	bool is_temp_orens_ = false;

	void UnlimitedHealth();
	void UnlimitedOrens();
	void UnlimitedEndurance();
	void UnlimitedPoints();
	void UnlimitedInventory();
	void FreeCamera();
	void NoToxicity();
	void DebugMode();
	void WireFrame();
	void GameGUI();
};