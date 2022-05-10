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
#include <fstream>

#include "../../json/json.hpp"

using json = nlohmann::json;

class Config
{
private:
	void ReadBool(const json& j, const char* key, bool& o);
	void ReadInt(const json& j, const char* key, int& o);
public:
	void LoadSettingsConfig();
	void SaveSetiingsConfig();
	void SaveHookConfig();
};