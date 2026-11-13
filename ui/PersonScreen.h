#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "../Application.h"

extern void showConfirmDialog(tgui::Gui* gui, const tgui::String& title, const tgui::String& message, const tgui::String& iconText, tgui::Color iconBgColor, tgui::Panel::Ptr detailPanel, std::function<void()> onYes);

extern const tgui::Color COL_FRAME_BORDER;
extern const tgui::Color COL_TEXTBOX_BORDER;

void loadPersonView(tgui::Panel::Ptr contentPanel, Application& app, tgui::Gui* gui);