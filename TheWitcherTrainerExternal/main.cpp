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

#include <wininet.h>
#pragma comment (lib, "Wininet.lib")
#include <Urlmon.h>
#pragma comment (lib, "urlmon.lib")

static void glfw_error_callback(int error, const char* description)
{
    MessageBox(0, description, "Glfw Error", MB_OK);
    exit(1);
}

bool CheckUpdate()
{
    if (settings::is_check_update_ == true)
    {
        if (std::filesystem::is_directory("config")) 
        { remove("config\\version.json"); } 
        else { std::filesystem::create_directories("config"); }

        std::string dwnld_URL = "https://alshoping.ru/kirizaku/games/trainers/The_Witcher_Enhanced_Edition_Directors_Cut/version.json";
        std::string savepath = "config\\version.json";
        DeleteUrlCacheEntry(dwnld_URL.c_str());
        URLDownloadToFile(0, dwnld_URL.c_str(), savepath.c_str(), 0, 0);

        json j;
        std::ifstream l("config\\version.json");
        if (l.peek() == std::ifstream::traits_type::eof())
        {
            MessageBox(0, "Failed to check for update. Error Code: 0", "Update Checker", MB_ICONERROR);
            return false;
        }

        std::string update_version;
        j = json::parse(l, nullptr, false, true);
        if (const auto& val = j["version_win"]; val.type() == json::value_t::string)
        {
            val.get_to(update_version);
        } else {
            MessageBox(0, "Failed to check for update. Error Code: 1", "Update Checker", MB_ICONERROR);
            return false;
        }

        if (TRAINER_VERSION != update_version)
        {
            const int result = MessageBox(0, "A new version of Trainer is available. Update?", "Update Checker", MB_YESNO);

            switch (result)
            {
                case IDYES:
                ShellExecute(0, 0, "https://github.com/Kirizaku/The-Witcher-Trainer", 0, 0, SW_SHOW);
                exit(0);
                break;
            }
        }
    }
    return true;
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    std::thread Functions_thr(&Hacks::Functions, Hacks());
    Functions_thr.detach();

    Config().LoadSettingsConfig();
    CheckUpdate();
    menu().Initialize();

    return 0;
}