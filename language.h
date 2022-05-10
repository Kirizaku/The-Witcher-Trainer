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

class Language
{
public:
    static inline const char* kUnlimitedHealthLabel = "";
	static inline const char* kUnlimitedOrensLabel = "";
	static inline const char* kUnlimitedEnduranceLabel = "";
	static inline const char* kUnlimitedPointsLabel = "";
	static inline const char* kUnlimitedInventoryLabel = "";
	static inline const char* kFreeCamera = "";
	static inline const char* kNoToxicityLabel = "";
	static inline const char* kDebugModeLabel = "Debug Mode";
	static inline const char* kWireFrameLabel = "Wireframe";
	static inline const char* kGUILabel = "";
	static inline const char* kMaxLvlLabel = "";
	static inline const char* kSvCoordsLabel = "";
	static inline const char* kLoadCoordsLabel = "";
	static inline const char* kLoadCustomCoordsLabel = "";
	static inline const char* kWaitForGameLabel = "";
	static inline const char* kProcNameLabel = "";
	static inline const char* kProcIdLabel = "";
	static inline const char* kSupportProjectLabel = "";
	static inline const char* kSourceCode = "";
	static inline const char* kCheckForUpdatesLabel = "";
	static inline const char* kTurnOffOnTheSound = "";
	static inline const char* kThemeLabel = "";
	static inline const char* kThemeComboSelector = "";
	static inline const char* kLangSelectorLabel = "";
	static inline const char* kFunctionsLabel = "";
	static inline const char* kSettingsLabel = "";
	static inline const char* kAboutLabel = "";
	static inline const char* kVersionLabel = "";
	static inline const char* kReportLevel = "";
	static inline const char* kAdditionalCopyrightsLabel = "";
	static inline const char* kLicenseLabel = "";

	void LangEng() noexcept;
	void LangRus() noexcept;
	bool ShowLanguageSelector(const char* label);
};

