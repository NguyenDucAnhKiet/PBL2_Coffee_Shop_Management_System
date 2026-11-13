#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "../Application.h"
#include "OrderScreen.h"
#include "TableScreen.h"
#include "PersonScreen.h"
#include "ProductScreen.h"

extern std::string formatCurrency(double amount);

void loadDashboardScreen(tgui::Gui& gui, sf::RenderWindow& window, Application& app);
void loadHomeView(tgui::Panel::Ptr contentPanel, Application& app);