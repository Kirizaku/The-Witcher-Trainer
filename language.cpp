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

#include "language.h"
#include "TheWitcherTrainerExternal/menu.h"

void Language::LangEng() noexcept
{
    kUnlimitedHealthLabel       = "Unlimited Health";
    kUnlimitedOrensLabel        = "Unlimited Orens";
    kUnlimitedEnduranceLabel    = "Unlimited Endurance";
    kUnlimitedPointsLabel       = "Unlimited Points";
    kUnlimitedInventoryLabel    = "Unlimited Inventory";
    kFreeCamera                 = "Free Camera";
    kNoToxicityLabel            = "No Toxicity";
    kGUILabel                   = "Disable GUI";
    kMaxLvlLabel                = "Maximum level";
    kSvCoordsLabel              = "Save coordinates";
    kLoadCoordsLabel            = "Load coordinates";
    kLoadCustomCoordsLabel      = "Load custom coordinates";
    kWaitForGameLabel           = "Waiting for game...";
    kProcNameLabel              = "Process Name:   %s";
    kProcIdLabel                = "Process ID:   %i";
    kSupportProjectLabel        = "Support me on";
    kSourceCode                 = "Source code available on";
    kCheckForUpdatesLabel       = "Check for Updates at startup";
    kTurnOffOnTheSound          = "Sound";
    kThemeLabel                 = "Theme";
    kThemeComboSelector         = "Dark\0Light\0";
    kLangSelectorLabel          = "Language";
    kFunctionsLabel             = "Functions";
    kSettingsLabel              = "Settings";
    kAboutLabel                 = "About trainer";
    kVersionLabel               = "Version %s";
    kReportLevel                = "\nReport bugs or request enhancements at:";
    kAdditionalCopyrightsLabel  = "\nAdditional Copyrights:";
    kLicenseLabel               = "License";
}

void Language::LangRus() noexcept
{
    kUnlimitedHealthLabel       = u8"Неограниченное здоровье";
    kUnlimitedOrensLabel        = u8"Неограниченные орены";
    kUnlimitedEnduranceLabel    = u8"Неограниченная энергия";
    kUnlimitedPointsLabel       = u8"Неограниченные очки талантов";
    kUnlimitedInventoryLabel    = u8"Неограниченный инвентарь";
    kFreeCamera                 = u8"Свободная камера";
    kNoToxicityLabel            = u8"Без токсичности";
    kGUILabel                   = u8"Отключить GUI";
    kMaxLvlLabel                = u8"Максимальный уровень";
    kSvCoordsLabel              = u8"Сохранить координаты";
    kLoadCoordsLabel            = u8"Загрузить координаты";
    kLoadCustomCoordsLabel      = u8"Загрузить польз координаты";
    kWaitForGameLabel           = u8"В ожидании игры...";
    kProcNameLabel              = u8"Имя процесса:   %s";
    kProcIdLabel                = u8"ID процесса:   %i";
    kSupportProjectLabel        = u8"Поддержать меня на";
    kSourceCode                 = u8"Исходный код доступен на";
    kCheckForUpdatesLabel       = u8"Проверять наличие обновлений при запуске";
    kTurnOffOnTheSound          = u8"Звук";
    kThemeLabel                 = u8"Тема";
    kThemeComboSelector         = u8"Темная\0Светлая\0";
    kLangSelectorLabel          = u8"Язык";
    kFunctionsLabel             = u8"Функции";
    kSettingsLabel              = u8"Настройки";
    kAboutLabel                 = u8"О трейнере";
    kVersionLabel               = u8"Версия %s";
    kReportLevel                = u8"\nСообщить об ошибке или предложить улучшение на веб-сайте:";
    kAdditionalCopyrightsLabel  = u8"\nДополнительные авторские права:";
    kLicenseLabel               = u8"Лицензия";
}

bool Language::ShowLanguageSelector(const char* label)
{
    if (ImGui::Combo(label, &settings::lang_, u8"English\0Русский\0"))
    {
        switch (settings::lang_)
        {
            case 0: LangEng(); break;
            case 1: LangRus(); break;
        }
        return true;
    }
    return false;
}