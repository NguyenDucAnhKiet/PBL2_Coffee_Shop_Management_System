#ifndef ORDER_SCREEN_H
#define ORDER_SCREEN_H

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "../Application.h"

void loadOrderView(tgui::Panel::Ptr contentPanel, Application& app, tgui::Gui* gui);

#endif