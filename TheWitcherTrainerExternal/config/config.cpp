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

#include "config.h"
#include "../menu.h"

void Config::ReadBool(const json& j, const char* key, bool& o)
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.type() == json::value_t::boolean)
        val.get_to(o);
}

void Config::ReadInt(const json& j, const char* key, int& o)
{
    if (!j.contains(key))
        return;

    if (const auto& val = j[key]; val.type() == json::value_t::number_unsigned)
        val.get_to(o);
}

void Config::LoadSettingsConfig()
{
    json j;
    settings st;

    std::ifstream l("config\\settings.json");
    j = json::parse(l, nullptr, false, true);

    ReadBool(j, "check_for_updates",        st.is_check_update_);
    ReadBool(j, "sound",                    st.is_sound);
    ReadInt(j, "theme",                     st.style_idx_);
    ReadInt(j, "unlimited_health_Key",      st.key_bind_[0]);
    ReadInt(j, "unlimited_orens_key",       st.key_bind_[1]);
    ReadInt(j, "unlimited_endurance_key",   st.key_bind_[2]);
    ReadInt(j, "unlimited_points_key",      st.key_bind_[3]);
    ReadInt(j, "unlimited_inventory_key",   st.key_bind_[4]);
    ReadInt(j, "enable_free_camera",        st.key_bind_[5]);
    ReadInt(j, "no_toxicity_key",           st.key_bind_[6]);
    ReadInt(j, "debug_mod_key",             st.key_bind_[7]);
    ReadInt(j, "wire_frame_key",            st.key_bind_[8]);
    ReadInt(j, "gui_key",                   st.key_bind_[9]);
    ReadInt(j, "max_level_key",             st.key_bind_[10]);
    ReadInt(j, "save_coords_key",           st.key_bind_[11]);
    ReadInt(j, "load_coords_key",           st.key_bind_[12]);
    ReadInt(j, "load_custom_coords_key",    st.key_bind_[13]);
    ReadInt(j, "language",                  st.lang_);
}

void Config::SaveSetiingsConfig()
{
    json j;
    settings st;

    j["check_for_updates"] =                st.is_check_update_;
    j["sound"] =                            st.is_sound;
    j["theme"] =                            st.style_idx_;
    j["unlimited_health_Key"] =             st.key_bind_[0];
    j["unlimited_orens_key"] =              st.key_bind_[1];
    j["unlimited_endurance_key"] =          st.key_bind_[2];
    j["unlimited_points_key"] =             st.key_bind_[3];
    j["unlimited_inventory_key"] =          st.key_bind_[4];
    j["enable_free_camera"] =               st.key_bind_[5];
    j["no_toxicity_key"] =                  st.key_bind_[6];
    j["debug_mod_key"] =                    st.key_bind_[7];
    j["wire_frame_key"] =                   st.key_bind_[8];
    j["gui_key"] =                          st.key_bind_[9];
    j["max_level_key"] =                    st.key_bind_[10];
    j["save_coords_key"] =                  st.key_bind_[11];
    j["load_coords_key"] =                  st.key_bind_[12];
    j["load_custom_coords_key"] =           st.key_bind_[13];
    j["language"] =                         st.lang_;

    if (std::ofstream out{"config\\settings.json"}; out.good())
        out << std::setw(4) << j;
}

void Config::SaveHookConfig()
{
    json j;
    Hacks hx;

    j["hooking"]                            = hx.is_hooking_;
    j["enable_unlimited_inventory"]         = hx.is_unlimited_inventory_;

    if (std::ofstream out{ "config\\hook.json" }; out.good())
        out << std::setw(4) << j;
}