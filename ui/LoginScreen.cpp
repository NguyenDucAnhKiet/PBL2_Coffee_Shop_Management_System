#include "LoginScreen.h"
#include "DashboardScreen.h"
#include <iostream>

using namespace std;

void loadLoginScreen(tgui::Gui& gui, sf::RenderWindow& window, Application& app) {
    gui.removeAllWidgets();

    window.setSize(sf::Vector2u(800, 600));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i((desktop.width - 800) / 2, (desktop.height - 600) / 2));
    window.setView(sf::View(sf::FloatRect(0, 0, 800, 600)));

    try {
        gui.setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    } catch (...) {}

    auto loginCard = tgui::Panel::create({460, 480});
    loginCard->setPosition("50%", "50%");
    loginCard->setOrigin(0.5f, 0.5f);
    loginCard->getRenderer()->setBackgroundColor(tgui::Color::White);
    loginCard->getRenderer()->setBorders(1);
    loginCard->getRenderer()->setBorderColor(tgui::Color(210, 200, 190));
    loginCard->getRenderer()->setRoundedBorderRadius(15);
    gui.add(loginCard);

    auto labelTitle = tgui::Label::create("ĐĂNG NHẬP HỆ THỐNG");
    labelTitle->setTextSize(26);
    labelTitle->setPosition("50%", 35);
    labelTitle->setOrigin(0.5f, 0);
    labelTitle->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    labelTitle->getRenderer()->setTextColor(tgui::Color(111, 78, 55));
    loginCard->add(labelTitle);

    auto labelSub = tgui::Label::create("Coffee Shop Management System");
    labelSub->setTextSize(16);
    labelSub->setPosition("50%", 75);
    labelSub->setOrigin(0.5f, 0);
    labelSub->getRenderer()->setTextColor(tgui::Color(140, 120, 100));
    loginCard->add(labelSub);

    auto lblUser = tgui::Label::create("Tên đăng nhập:");
    lblUser->setTextSize(18);
    lblUser->setPosition(40, 125);
    lblUser->getRenderer()->setTextColor(tgui::Color(90, 45, 15));
    loginCard->add(lblUser);

    auto editUsername = tgui::EditBox::create();
    editUsername->setSize(380, 45);
    editUsername->setPosition(40, 155);
    editUsername->setTextSize(18);
    editUsername->setDefaultText("Nhập username (VD: admin1)");
    editUsername->getRenderer()->setBorders(1);
    editUsername->getRenderer()->setBorderColor(tgui::Color(190, 180, 170));
    loginCard->add(editUsername);

    auto lblPass = tgui::Label::create("Mật khẩu:");
    lblPass->setTextSize(18);
    lblPass->setPosition(40, 215);
    lblPass->getRenderer()->setTextColor(tgui::Color(90, 45, 15));
    loginCard->add(lblPass);

    auto editPassword = tgui::EditBox::create();
    editPassword->setSize(380, 45);
    editPassword->setPosition(40, 245);
    editPassword->setTextSize(18);
    editPassword->setDefaultText("Nhập mật khẩu");
    editPassword->setPasswordCharacter('*');
    editPassword->getRenderer()->setBorders(1);
    editPassword->getRenderer()->setBorderColor(tgui::Color(190, 180, 170));
    loginCard->add(editPassword);

    auto labelError = tgui::Label::create("");
    labelError->setPosition(40, 305);
    labelError->setTextSize(16);
    labelError->getRenderer()->setTextColor(tgui::Color::Red);
    loginCard->add(labelError);

    auto btnLogin = tgui::Button::create("ĐĂNG NHẬP");
    btnLogin->setSize(380, 50);
    btnLogin->setPosition(40, 345);
    btnLogin->setTextSize(20);
    btnLogin->getRenderer()->setBackgroundColor(tgui::Color(111, 78, 55));
    btnLogin->getRenderer()->setTextColor(tgui::Color::White);
    btnLogin->getRenderer()->setRoundedBorderRadius(8);
    loginCard->add(btnLogin);

    auto handleLogin = [&gui, &window, &app, editUsername, editPassword, labelError]() {
        string user = editUsername->getText().toStdString();
        string pass = editPassword->getText().toStdString();

        if (app.login(user, pass)) {
            loadDashboardScreen(gui, window, app); 
        } else {
            labelError->setText("Sai tài khoản, mật khẩu hoặc tài khoản bị khóa!");
        }
    };

    btnLogin->onPress(handleLogin);
    editUsername->onReturnKeyPress(handleLogin);
    editPassword->onReturnKeyPress(handleLogin);
}