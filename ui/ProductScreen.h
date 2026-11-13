#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "../Application.h"

extern std::string formatCurrency(double amount);

void loadMenuView(tgui::Panel::Ptr contentPanel, Application& app);