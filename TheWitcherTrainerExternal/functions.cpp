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

#include "menu.h"
#include "config/config.h"

void Hacks::Functions()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (is_running_)
        {
            mem::read(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kLocalPlayer), &lp_base_[0], sizeof(lp_base_[0]));
            mem::read(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpOffset1), &lp_base_[0], sizeof(lp_base_[0]));
            mem::read(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpOffset2), &lp_base_[0], sizeof(lp_base_[0]));
            mem::read(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpOffset3), &lp_base_[1], sizeof(lp_base_[1]));

            if (!lp_base_[1])
                continue;

            UnlimitedHealth();
            UnlimitedOrens();
            UnlimitedEndurance();
            UnlimitedPoints();
            FreeCamera();
            NoToxicity();
            DebugMode();
            WireFrame();
            GameGUI();

            if (!globals::process_mod_hook)
                continue;

            UnlimitedInventory();
        }
    }
}

void Hacks::UnlimitedHealth()
{
    float write_health = 9999.0f;
    if (is_unlimited_health_) {
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[1] + offsets::kLpHealth), &write_health, sizeof(write_health));
    }
}

void Hacks::UnlimitedOrens()
{
    int write_orens = 999999;
    if (is_unlimited_orens_) {
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[0] + offsets::kLpOrens), &write_orens, sizeof(write_orens));
    }
}

void Hacks::UnlimitedEndurance()
{
    float write_endurance = 100.0f;
    if (is_unlimited_endurance_) {
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[1] + offsets::kLpEndurance), &write_endurance, sizeof(write_endurance));
    }
}

void Hacks::UnlimitedPoints()
{
    int write_points = 50;
    if (is_unlimited_points_) {
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[1] + offsets::kLpBronzePoints), &write_points, sizeof(write_points));
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[1] + offsets::kLpSilverPoints), &write_points, sizeof(write_points));
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[1] + offsets::kLpGoldPoints), &write_points, sizeof(write_points));
    }
}

void Hacks::FreeCamera()
{
    if (is_free_camera_) {
        int write_enable_free_camera = 1;
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kFreeCamera1), 
            &write_enable_free_camera, sizeof(write_enable_free_camera));
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kFreeCamera2), 
            &write_enable_free_camera, sizeof(write_enable_free_camera));
    }
    else {
        int write_disable_free_camera = 0;
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kFreeCamera1), 
            &write_disable_free_camera, sizeof(write_disable_free_camera));
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kFreeCamera2), 
            &write_disable_free_camera, sizeof(write_disable_free_camera));
    }
}

void Hacks::NoToxicity()
{
    int write_toxicity = 0;
    if (is_toxicity_) {
        mem::write(globals::pid, (void*)((uintptr_t)lp_base_[1] + offsets::kLpToxicity), &write_toxicity, sizeof(write_toxicity));
    }
}

void Hacks::DebugMode()
{
    if (is_debug_mode_) {
        int write_enable_debugmode = 1;
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kDebugMode), 
            &write_enable_debugmode, sizeof(write_enable_debugmode));
    } else {
        int write_disable_debugmode = 0;
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kDebugMode), 
            &write_disable_debugmode, sizeof(write_disable_debugmode));
    }
}

void Hacks::WireFrame()
{
    if (is_wire_frame_) {
        int write_enable_wireframe = 1;
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kWireFrame), 
            &write_enable_wireframe, sizeof(write_enable_wireframe));
    } else {
        int write_disable_wireframe = 0;
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kWireFrame), 
            &write_disable_wireframe, sizeof(write_disable_wireframe));
    }
}

void Hacks::GameGUI()
{
    if (is_gui_) {
        int write_enable_gui = 1;
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kGUI), 
            &write_enable_gui, sizeof(write_enable_gui));
    }
    else {
        int write_disable_gui = 0;
        mem::write(globals::pid, (void*)((uintptr_t)globals::process_mod + offsets::kGUI), 
            &write_disable_gui, sizeof(write_disable_gui));
    }
}

/*process_mod_hook*/

void Hacks::UnlimitedInventory() {
    if (is_unlimited_inventory_ && !is_temp_inventory_) { Config().SaveHookConfig(); is_temp_inventory_ = true; }
    else if (!is_unlimited_inventory_ && is_temp_inventory_) { Config().SaveHookConfig(); is_temp_inventory_ = false; }
}