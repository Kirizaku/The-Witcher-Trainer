#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui_custom.h"

constexpr std::array<std::string_view, 166> KeyNamesWin32 {
    "Unknown",
    "LBUTTON",
    "RBUTTON",
    "CANCEL",
    "MBUTTON",
    "XBUTTON1",
    "XBUTTON2",
    "Unknown",
    "BACK",
    "TAB",
    "Unknown",
    "Unknown",
    "CLEAR",
    "ENTER",
    "Unknown",
    "Unknown",
    "SHIFT",
    "CONTROL",
    "MENU",
    "PAUSE",
    "CAPS",
    "KANA",
    "Unknown",
    "JUNJA",
    "FINAL",
    "KANJI",
    "Unknown",
    "ESCAPE",
    "CONVERT",
    "NONCONVERT",
    "ACCEPT",
    "MODECHANGE",
    "SPACE",
    "PRIOR",
    "NEXT",
    "END",
    "HOME",
    "LEFT",
    "UP",
    "RIGHT",
    "DOWN",
    "SELECT",
    "PRINT",
    "EXECUTE",
    "SNAPSHOT",
    "INSERT",
    "DELETE",
    "HELP",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "LWIN",
    "RWIN",
    "APPS",
    "Unknown",
    "SLEEP",
    "NUMPAD0",
    "NUMPAD1",
    "NUMPAD2",
    "NUMPAD3",
    "NUMPAD4",
    "NUMPAD5",
    "NUMPAD6",
    "NUMPAD7",
    "NUMPAD8",
    "NUMPAD9",
    "*",
    "+",
    "SEPARATOR",
    "-",
    ".",
    "/",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "NUMLOCK",
    "SCROLL",
    "OEM_NEC_EQUAL",
    "OEM_FJ_MASSHOU",
    "OEM_FJ_TOUROKU",
    "OEM_FJ_LOYA",
    "OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "LSHIFT",
    "RSHIFT",
    "LCONTROL",
    "RCONTROL",
    "LALT",
    "RALT"
};

bool ImGuiCustom::KeyBind(const char* label, int* k, const ImVec2& size_arg, const int id_button)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    if (window->SkipItems)
        return false;

    ImVec2 cursor_pos = window->DC.CursorPos;
    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(id_button);
    const ImVec2 label_size = ImGui::CalcTextSize(label, 0, true);
    ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
    const ImRect frame_bb(cursor_pos + ImVec2(label_size.x + style.ItemInnerSpacing.x, 0.0f), cursor_pos + size);
    const ImRect total_bb(cursor_pos, frame_bb.Max);

    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    const bool is_hovered = ImGui::ItemHoverable(frame_bb, id);

    if (is_hovered)
    {
        ImGui::SetHoveredID(id);
        g.MouseCursor = ImGuiMouseCursor_TextInput;

        if (g.IO.MouseDown[0]) { 
            ImGui::FocusWindow(window); 
        }
        else if (g.IO.MouseReleased[0]) { 
            ImGui::SetActiveID(id, window); 
        }
    }

    std::string display_text = "...";
    bool value_changed = false;
    int key = *k;

    if (g.ActiveId == id) {
        if (!value_changed) {
            for (auto i = VK_MBUTTON; i <= VK_RMENU; i++) {
                if (GetAsyncKeyState(i))
                {
                    for (int x = 0; x < 14; x++) {
                        if (settings::key_bind_[x] == i) {
                            settings::key_bind_[x] = 0;
                        }
                    }
                    key = i;
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }
        }
        if (ImGui::IsKeyPressedMap(ImGuiKey_Escape)) {
            *k = 0;
            ImGui::ClearActiveID();
        }
        else { *k = key; }
    }

    const ImU32 color = ImGui::GetColorU32((is_hovered) ? ImGuiCol_ButtonHovered : is_hovered ? ImGuiCol_ButtonActive : ImGuiCol_Button);
    ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, color, true, style.FrameRounding);

    if (*k != 0 && g.ActiveId != id) { 
        display_text = KeyNamesWin32[*k]; 
    }
    else if (g.ActiveId == id) { 
        display_text = "<Press a key>"; 
    }

    const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y);
    ImVec2 render_pos = frame_bb.Min + style.FramePadding;
    ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, 
        display_text.c_str(), 0, 0, style.ButtonTextAlign, &clip_rect);

    if (label_size.x > 0)
        ImGui::RenderText(ImVec2(total_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;
}

void ImGuiCustom::TextURL(const char* label, const char* url)
{
    ImGui::TextColored(ImVec4(0.0f, 0.3f, 0.8f, 1.0f), label);
    if (ImGui::IsItemHovered()) { 
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) { 
            ShellExecute(0, 0, url, 0, 0, SW_SHOW); }
    }
}