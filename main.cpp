#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>

#include <ctime>
#include <iomanip>
#include <sstream>
#include "Application.h"

#include "ui/LoginScreen.h"
#include "ui/DashboardScreen.h"
#include "ui/OrderScreen.h"
#include "ui/PersonScreen.h"
#include "ui/ProductScreen.h"
#include "ui/TableScreen.h"

const tgui::Color COL_FRAME_BORDER(227, 217, 204);
const tgui::Color COL_TEXTBOX_BORDER(207, 194, 179);

using namespace std;

void showConfirmDialog(tgui::Gui* gui, const tgui::String& title, const tgui::String& message, const tgui::String& iconText, tgui::Color iconBgColor, tgui::Panel::Ptr detailPanel, std::function<void()> onYes) {
    auto overlay = tgui::Panel::create({"100%", "100%"});
    overlay->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 150));
    gui->add(overlay);

    auto dialog = tgui::Panel::create({"780", "540"});
    dialog->setPosition("50% - width/2", "50% - height/2");
    dialog->getRenderer()->setBackgroundColor(tgui::Color::White);
    dialog->getRenderer()->setRoundedBorderRadius(15);
    overlay->add(dialog);

    auto lblTitle = tgui::Label::create(title);
    lblTitle->setPosition(30, 20);
    lblTitle->setTextSize(24);
    lblTitle->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    dialog->add(lblTitle);

    auto btnClose = tgui::Button::create("x");
    btnClose->setPosition("100% - 50", 15);
    btnClose->setSize(35, 35);
    btnClose->setTextSize(24);
    btnClose->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    btnClose->getRenderer()->setTextColor(tgui::Color(100, 100, 100));
    btnClose->getRenderer()->setBorders(0);
    btnClose->onPress([gui, overlay]() { gui->remove(overlay); });
    dialog->add(btnClose);

    auto iconLabel = tgui::Button::create(iconText);
    iconLabel->setPosition(30, 80);
    iconLabel->setSize(60, 60);
    iconLabel->setTextSize(30);
    iconLabel->getRenderer()->setBackgroundColor(iconBgColor);
    iconLabel->getRenderer()->setTextColor(tgui::Color::White);
    iconLabel->getRenderer()->setRoundedBorderRadius(30);
    iconLabel->getRenderer()->setBorders(0);
    iconLabel->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    iconLabel->setEnabled(false);
    dialog->add(iconLabel);

    auto lblMessage = tgui::Label::create(message);
    lblMessage->setPosition(110, tgui::bindTop(iconLabel) + (tgui::bindHeight(iconLabel) - tgui::bindHeight(lblMessage)) / 2);
    lblMessage->setMaximumTextWidth(630);
    lblMessage->setTextSize(20);
    lblMessage->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    dialog->add(lblMessage);

    if (detailPanel) {
        detailPanel->setPosition(30, 170);
        
        float panelHeight = detailPanel->getSize().y;
        if (panelHeight <= 0) panelHeight = 240;
        
        detailPanel->setSize("100% - 60", panelHeight);
        detailPanel->getRenderer()->setBackgroundColor(tgui::Color(250, 245, 240));
        detailPanel->getRenderer()->setRoundedBorderRadius(8);
        dialog->add(detailPanel);
        
        dialog->setSize(780, 170 + panelHeight + 110);
    }

    auto btnYes = tgui::Button::create("Có");
    btnYes->setSize(150, 50);
    btnYes->setPosition("100% - 340", "100% - 90");
    btnYes->getRenderer()->setBackgroundColor(tgui::Color(160, 82, 45));
    btnYes->getRenderer()->setTextColor(tgui::Color::White);
    btnYes->getRenderer()->setRoundedBorderRadius(10);
    btnYes->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    btnYes->setTextSize(18);
    btnYes->onPress([gui, overlay, onYes]() {
        onYes();
        gui->remove(overlay);
    });
    dialog->add(btnYes);

    auto btnNo = tgui::Button::create("Không");
    btnNo->setSize(150, 50);
    btnNo->setPosition("100% - 170", "100% - 90");
    btnNo->getRenderer()->setBackgroundColor(tgui::Color::White);
    btnNo->getRenderer()->setTextColor(tgui::Color(20, 20, 20));
    btnNo->getRenderer()->setBorders(1);
    btnNo->getRenderer()->setBorderColor(tgui::Color(200, 200, 200));
    btnNo->getRenderer()->setRoundedBorderRadius(10);
    btnNo->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    btnNo->setTextSize(18);
    btnNo->onPress([gui, overlay]() { gui->remove(overlay); });
    dialog->add(btnNo);
}

string formatCurrency(double amount) {
    long long intAmount = static_cast<long long>(amount);
    string s = to_string(intAmount);
    int n = s.length() - 3;
    while (n > 0) {
        s.insert(n, ",");
        n -= 3;
    }
    return s + " đ";
}

int main() {
    Application app;
    
    if (!app.initialize()) {
        cout << "Canh bao: Mot so file du lieu chua duoc tai thanh cong!" << endl;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Coffee Shop Management - PBL2", sf::Style::Titlebar | sf::Style::Close);
    tgui::Gui gui(window);

    try {
        gui.setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    } catch (...) {}

    loadLoginScreen(gui, window, app);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            gui.handleEvent(event);

            if (event.type == sf::Event::Closed) {
                app.saveData();
                window.close();
            }
        }

        window.clear(sf::Color(245, 240, 230));
        gui.draw();
        window.display();
    }

    return 0;
}