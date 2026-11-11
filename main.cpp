#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>

#include <ctime>
#include <iomanip>
#include <sstream>
#include "Application.h"

const tgui::Color COL_FRAME_BORDER(227, 217, 204);
const tgui::Color COL_TEXTBOX_BORDER(207, 194, 179);

using namespace std;

// =========================================================================
// KHAI BÁO TRƯỚC CÁC HÀM GIAO DIỆN (Forward Declarations)
// Giúp các hàm có thể gọi qua lại lẫn nhau khi cần chuyển cảnh (Scene Routing)
// =========================================================================
void loadLoginScreen(tgui::Gui& gui, sf::RenderWindow& window, Application& app);
void loadDashboardScreen(tgui::Gui& gui, sf::RenderWindow& window, Application& app);
void loadHomeView(tgui::Panel::Ptr contentPanel, Application& app);
void loadMenuView(tgui::Panel::Ptr contentPanel, Application& app);
void loadPersonView(tgui::Panel::Ptr contentPanel, Application& app, tgui::Gui* gui);

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

void loadLoginScreen(tgui::Gui& gui, sf::RenderWindow& window, Application& app) {
    gui.removeAllWidgets();

    // --- Cấu hình kích thước cửa sổ cho màn hình Login ---
    window.setSize(sf::Vector2u(800, 600));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i((desktop.width - 800) / 2, (desktop.height - 600) / 2));
    window.setView(sf::View(sf::FloatRect(0, 0, 800, 600))); 

    // --- Tiêu đề form ---
    auto labelTitle = tgui::Label::create("ĐĂNG NHẬP HỆ THỐNG");
    labelTitle->setTextSize(30);
    labelTitle->setPosition("35%", "25%");
    gui.add(labelTitle);

    // --- Ô nhập Tên đăng nhập (Username) ---
    auto editUsername = tgui::EditBox::create();
    editUsername->setSize(300, 45);
    editUsername->setPosition("35%", "35%");
    editUsername->setDefaultText("Tên đăng nhập (VD: admin1)");
    gui.add(editUsername);

    // --- Ô nhập Mật khẩu (Password) ---
    auto editPassword = tgui::EditBox::create();
    editPassword->setSize(300, 45);
    editPassword->setPosition("35%", "45%");
    editPassword->setDefaultText("Mật khẩu");
    editPassword->setPasswordCharacter('*'); // Ẩn mật khẩu thành dấu sao
    gui.add(editPassword);

    // --- Nhãn hiển thị thông báo lỗi/thành công ---
    auto labelError = tgui::Label::create("");
    labelError->setPosition("35%", "53%");
    labelError->getRenderer()->setTextColor(tgui::Color::Red);
    gui.add(labelError);

    // --- Nút bấm Đăng Nhập ---
    auto btnLogin = tgui::Button::create("ĐĂNG NHẬP");
    btnLogin->setSize(300, 50);
    btnLogin->setPosition("35%", "58%");
    gui.add(btnLogin);

    // --- Xử lý sự kiện khi nhấn nút Đăng Nhập ---
    btnLogin->onPress([&gui, &window, &app, editUsername, editPassword, labelError]() {
        string user = editUsername->getText().toStdString();
        string pass = editPassword->getText().toStdString();

        if (app.login(user, pass)) {
            loadDashboardScreen(gui, window, app); 
        } else {
            labelError->setText("Sai tai khoan, mat khau hoac tai khoan bi khoa!");
        }
    });
}

void loadDashboardScreen(tgui::Gui& gui, sf::RenderWindow& window, Application& app) {
    gui.removeAllWidgets();
    Person* currentUser = app.getCurrentUser();

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int newWidth = desktop.width;
    int newHeight = desktop.height - 80;

    window.setSize(sf::Vector2u(newWidth, newHeight));
    window.setPosition(sf::Vector2i(0, 0));
    window.setView(sf::View(sf::FloatRect(0, 0, newWidth, newHeight)));

    auto headerPicture = tgui::Picture::create("assets/images/header.png");
    headerPicture->setSize({"100%", "18%"});
    headerPicture->setPosition(0, 0);
    gui.add(headerPicture);

    int roleValue = (int)currentUser->getAccount().getRole();
    string roleName = (roleValue == 1) ? "Quản Lý" : "Nhân Viên";

    //Tạo phần hiển thị Tên + Vai trò 
    auto shadowPanel = tgui::Panel::create({"10.3%", "14.5%"});
    shadowPanel->setPosition("65%", "2%");
    shadowPanel->getRenderer()->setBackgroundColor(tgui::Color(120, 120, 120, 60));
    shadowPanel->getRenderer()->setRoundedBorderRadius(15);
    gui.add(shadowPanel);

    auto userCard = tgui::Panel::create({"10%", "14%"});
    userCard->setPosition("65%", "2%");
    userCard->getRenderer()->setBackgroundColor(tgui::Color(255, 248, 235));
    userCard->getRenderer()->setBorders(1);
    userCard->getRenderer()->setBorderColor(tgui::Color(170, 130, 90));
    userCard->getRenderer()->setRoundedBorderRadius(15);
    gui.add(userCard);

    auto userIcon = tgui::Picture::create("assets/icons/user.png");
    userIcon->setSize("15%", "17%");
    userIcon->setPosition("9%", "40%");
    userCard->add(userIcon);

    auto labelUserName = tgui::Label::create(currentUser->getFullName());
    labelUserName->setTextSize(20);
    labelUserName->getRenderer()->setTextColor(tgui::Color(90, 45, 15));
    labelUserName->setPosition("27%", "42.5%");

    auto labelUserRole = tgui::Label::create(roleName);
    labelUserRole->setTextSize(16);
    labelUserRole->setPosition("27%", "57.5%");
    labelUserRole->getRenderer()->setTextColor(tgui::Color(50, 50, 50));

    auto labelUsername = tgui::Label::create("@" + currentUser->getAccount().getUsername());
    labelUsername->setTextSize(16);
    labelUsername->setPosition("27%", "27.5%");
    labelUsername->getRenderer()->setTextColor(tgui::Color(50, 50, 50));

    userCard->add(labelUsername);
    userCard->add(labelUserName);
    userCard->add(labelUserRole);
    
    auto menuBar = tgui::Panel::create({"100%", "6%"});
    menuBar->setPosition(0, "18%"); //Sửa chiều dọc
    menuBar->getRenderer()->setBackgroundColor(tgui::Color(160, 140, 120)); 
    gui.add(menuBar);

    auto contentPanel = tgui::Panel::create({"100%", "76%"});
    contentPanel->setPosition(0, "24%"); //Sửa chiều dọc
    contentPanel->getRenderer()->setBackgroundColor(tgui::Color(245, 240, 230)); 
    gui.add(contentPanel);

    vector<string> menuItems = {
        "TRANG CHỦ", "ĐƠN HÀNG", "LIST ĐƠN HÀNG", 
        "NHÂN VIÊN", "MENU", "BÀN", "THỐNG KÊ"
    };
    
    float buttonWidth = 100.0f / (menuItems.size() + 1);
    auto menuButtons = std::make_shared<std::vector<tgui::Button::Ptr>>();

    tgui::Gui* pGui = &gui;
    
    auto switchTab = [contentPanel, menuButtons, &app, pGui](int index) {
        // 1. Đổi màu trạng thái Active (Nút nào được bấm sẽ đậm màu hơn các nút còn lại)
        for (size_t i = 0; i < menuButtons->size(); ++i) {
            if (i == index) {
                (*menuButtons)[i]->getRenderer()->setBackgroundColor(tgui::Color(110, 90, 70)); // Màu đậm khi chọn
            } else {
                (*menuButtons)[i]->getRenderer()->setBackgroundColor(tgui::Color(160, 140, 120)); // Màu nhạt mặc định
            }
        }

        contentPanel->removeAllWidgets();

        if (index == 0) {
            loadHomeView(contentPanel, app);
        } else if (index == 3) {
            loadPersonView(contentPanel, app, pGui);
        } else if (index == 4) {
            loadMenuView(contentPanel, app);
        } else {
            auto giantNumber = tgui::Label::create(to_string(index + 1));
            giantNumber->setTextSize(200); 
            giantNumber->setPosition("50%", "40%"); 
            giantNumber->setOrigin(0.5f, 0.5f);     
            giantNumber->getRenderer()->setTextColor(tgui::Color(180, 180, 180));
            contentPanel->add(giantNumber);
        }
    };

    for (size_t i = 0; i < menuItems.size(); ++i) {
        auto btn = tgui::Button::create(menuItems[i]);
        btn->setTextSize(25);
        btn->setSize({tgui::String(to_string(buttonWidth) + "%"), "100%"});
        btn->setPosition({tgui::String(to_string(i * buttonWidth) + "%"), "0%"});
        
        btn->getRenderer()->setBorders(0);
        btn->getRenderer()->setTextColor(tgui::Color::White);
        
        menuBar->add(btn);
        menuButtons->push_back(btn);

        btn->onPress([switchTab, i]() {
            switchTab(i);
        });
    }

    auto btnLogout = tgui::Button::create("ĐĂNG XUẤT");
    btnLogout->setTextSize(25);
    btnLogout->setSize({tgui::String(to_string(buttonWidth) + "%"), "100%"});
    btnLogout->setPosition({tgui::String(to_string(menuItems.size() * buttonWidth) + "%"), "0%"});
    btnLogout->getRenderer()->setBorders(0);
    btnLogout->getRenderer()->setBackgroundColor(tgui::Color(160, 140, 120)); 
    btnLogout->getRenderer()->setTextColor(tgui::Color(255, 120, 120));
    menuBar->add(btnLogout);

    btnLogout->onPress([&gui, &window, &app]() {
        app.logout(); 
        loadLoginScreen(gui, window, app); 
    });

    switchTab(0);
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

void loadHomeView(tgui::Panel::Ptr contentPanel, Application& app) {
    // -----------------------------------------------------
    // 1. HÀM HỖ TRỢ TẠO THẺ THỐNG KÊ (4 ô chữ nhật trên cùng)
    // -----------------------------------------------------
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d/%m/%Y");
    string today = oss.str();

    int todayOrders = app.getOrderManager().getPaidOrderCountByDate(today);
    double todayRevenue = app.getOrderManager().getRevenueByDate(today);

    auto createStatCard = [&](const string& title, const string& value, const string& xPos, const string& yPos, tgui::Color valueColor) {
        auto card = tgui::Panel::create({"21.5%", "18%"});
        card->setPosition(xPos.c_str(), yPos.c_str());
        card->getRenderer()->setBackgroundColor(tgui::Color::White);
        card->getRenderer()->setBorders(1);
        card->getRenderer()->setBorderColor(tgui::Color(200, 200, 200));
        card->getRenderer()->setRoundedBorderRadius(10);

        auto lblTitle = tgui::Label::create(title);
        lblTitle->setPosition("10%", "20%");
        lblTitle->setTextSize(16);
        lblTitle->getRenderer()->setTextColor(tgui::Color(100, 100, 100));
        card->add(lblTitle);

        auto lblValue = tgui::Label::create(value);
        lblValue->setPosition("10%", "50%");
        lblValue->setTextSize(26);
        lblValue->getRenderer()->setTextColor(valueColor);
        card->add(lblValue);

        return card;
    };

    contentPanel->add(createStatCard("Số đơn hàng trong ngày", to_string(todayOrders) + " Đơn", "3%", "5%", tgui::Color(50, 50, 50)));
    contentPanel->add(createStatCard("Doanh thu ngày", formatCurrency(todayRevenue), "27.5%", "5%", tgui::Color(30, 150, 50)));
    contentPanel->add(createStatCard("Số đơn đang xử lý", "3 Đơn", "52%", "5%", tgui::Color(200, 120, 0)));
    contentPanel->add(createStatCard("Số đơn huỷ", "1 Đơn", "76.5%", "5%", tgui::Color(200, 50, 50)));

    auto leftPanel = tgui::Panel::create({"46%", "68%"});
    leftPanel->setPosition("3%", "27%");
    leftPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    leftPanel->getRenderer()->setBorders(1);
    leftPanel->getRenderer()->setBorderColor(tgui::Color(200, 200, 200));
    leftPanel->getRenderer()->setRoundedBorderRadius(10);
    
    auto leftTitle = tgui::Label::create("THỐNG KÊ TOP SẢN PHẨM BÁN CHẠY");
    leftTitle->setPosition("5%", "5%");
    leftTitle->setTextSize(25);
    leftTitle->getRenderer()->setTextColor(tgui::Color(111, 78, 55));
    leftPanel->add(leftTitle);
    
    auto leftPlaceholder = tgui::Label::create("(Biểu đồ hoặc danh sách sẽ được bổ sung sau)");
    leftPlaceholder->setPosition("5%", "15%");
    leftPlaceholder->getRenderer()->setTextColor(tgui::Color(90, 45, 15));
    leftPanel->add(leftPlaceholder);

    contentPanel->add(leftPanel);

    auto rightPanel = tgui::Panel::create({"46%", "68%"});
    rightPanel->setPosition("52%", "27%");
    rightPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    rightPanel->getRenderer()->setBorders(1);
    rightPanel->getRenderer()->setBorderColor(tgui::Color(200, 200, 200));
    rightPanel->getRenderer()->setRoundedBorderRadius(10);
    
    auto rightTitle = tgui::Label::create("ĐƠN HÀNG GẦN ĐÂY");
    rightTitle->setPosition("5%", "5%");
    rightTitle->setTextSize(25);
    rightTitle->getRenderer()->setTextColor(tgui::Color(111, 78, 55));
    rightPanel->add(rightTitle);

    auto listView = tgui::ListView::create();
    listView->setSize("90%", "80%");
    listView->setPosition("5%", "15%");
    listView->setTextSize(20);
    listView->setItemHeight(40);
    listView->setHeaderHeight(40);
    
    listView->addColumn("Mã Order", 120);
    listView->addColumn("Mã NV", 120);
    listView->addColumn("Nhân Viên", 275);
    listView->addColumn("Thời Gian", 205);
    listView->addColumn("Tổng Tiền", 150);
    listView->addColumn("Trạng Thái", 180);
    listView->setColumnExpanded(6, true);
    
    vector<Order*> recentOrders = app.getOrderManager().getRecentOrders(20);

    for (Order* order : recentOrders){
        Person* staff = app.getPersonManager().findPersonByID(order->getEmployeeID());
        string staffName = (staff != nullptr) ? staff->getFullName() : "Không xác định";

        string statusStr = "Hoàn thành";

        listView->addItem({
            order->getOrderID(),
            order->getEmployeeID(),
            staffName,
            order->getOrderDate(),
            formatCurrency(order->calculateTotal()),
            statusStr
        });
    }
    
    rightPanel->add(listView);
    contentPanel->add(rightPanel);
}

void loadMenuView(tgui::Panel::Ptr contentPanel, Application& app) {
    // KHU VỰC TRÁI: DANH SÁCH MÓN & TÌM KIẾM (62% chiều rộng)
    auto leftPanel = tgui::Panel::create({"62%", "90%"});
    leftPanel->setPosition("2%", "5%");
    leftPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    leftPanel->getRenderer()->setBorders(1);
    leftPanel->getRenderer()->setBorderColor(tgui::Color(200, 200, 200));
    leftPanel->getRenderer()->setRoundedBorderRadius(10);
    contentPanel->add(leftPanel);

    auto lblListTitle = tgui::Label::create("DANH SÁCH MÓN");
    lblListTitle->setPosition("5%", "4%");
    lblListTitle->setTextSize(22);
    lblListTitle->getRenderer()->setTextColor(tgui::Color(111, 78, 55)); // Màu nâu cafe
    lblListTitle->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");
    lblListTitle->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    leftPanel->add(lblListTitle);

    auto searchBox = tgui::EditBox::create();
    searchBox->setSize("43%", 45);
    searchBox->setPosition("5%", "11%");
    searchBox->setDefaultText("Nhập tên món cần tìm...");
    searchBox->setTextSize(20);
    searchBox->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");
    leftPanel->add(searchBox);

    auto filterCombo = tgui::ComboBox::create();
    filterCombo->setSize("30%", 45);
    filterCombo->setPosition("50%", "11%");
    filterCombo->setTextSize(20);
    filterCombo->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");
    filterCombo->addItem("Tất cả");
    filterCombo->addItem("Cà phê");
    filterCombo->addItem("Trà trái cây");
    filterCombo->addItem("Bánh");
    filterCombo->setSelectedItem("Tất cả");
    leftPanel->add(filterCombo);

    auto btnSearch = tgui::Button::create("Tìm");
    btnSearch->setSize("13%", 45);
    btnSearch->setPosition("82%", "11%");
    btnSearch->setTextSize(20);
    btnSearch->getRenderer()->setBackgroundColor(tgui::Color(160, 140, 120));
    btnSearch->getRenderer()->setTextColor(tgui::Color::White);
    leftPanel->add(btnSearch);

    auto listView = tgui::ListView::create();
    listView->setSize("90%", "72%");
    listView->setPosition("5%", "20%");
    listView->setTextSize(20);
    listView->setItemHeight(45);
    listView->setHeaderHeight(50);
    
    listView->addColumn("Mã Món", 175);
    listView->addColumn("Tên Món", 200);
    listView->addColumn("Phân Loại", 175);
    listView->addColumn("Đơn Giá", 175);
    listView->addColumn("Trạng Thái", 175);
    listView->setColumnExpanded(1, true);

    static vector<Product*> currentProducts;
    static int currentPage = 1;

    auto btnFirst = tgui::Button::create("<<");
    btnFirst->setSize("8%", 35);
    btnFirst->setPosition("22%", "94%");
    leftPanel->add(btnFirst);

    auto btnPrev = tgui::Button::create("<");
    btnPrev->setSize("8%", 35);
    btnPrev->setPosition("32%", "94%");
    leftPanel->add(btnPrev);

    auto lblPage = tgui::Label::create("1 / 1");
    lblPage->setPosition("44%", "94.5%");
    lblPage->setTextSize(18);
    lblPage->getRenderer()->setTextColor(tgui::Color(90, 45, 15));
    leftPanel->add(lblPage);

    auto btnNext = tgui::Button::create(">");
    btnNext->setSize("8%", 35);
    btnNext->setPosition("54%", "94%");
    leftPanel->add(btnNext);

    auto btnLast = tgui::Button::create(">>");
    btnLast->setSize("8%", 35);
    btnLast->setPosition("64%", "94%");
    leftPanel->add(btnLast);

    auto updatePageDisplay = [=]() {
        listView->removeAllItems();
        int totalItems = currentProducts.size();
        int totalPages = (totalItems + 19) / 20;
        
        if (totalPages == 0) totalPages = 1;
        if (currentPage > totalPages) currentPage = totalPages;
        if (currentPage < 1) currentPage = 1;

        int startIndex = (currentPage - 1) * 20;
        int endIndex = startIndex + 20;
        if (endIndex > totalItems) endIndex = totalItems;

        for (int i = startIndex; i < endIndex; i++) {
            Product* p = currentProducts[i];
            string statusText = p->isAvailable() ? "Đang bán" : "Ngừng bán";
            listView->addItem({
                p->getProductID(),
                p->getName(),
                p->getCategory(),
                formatCurrency(p->getPrice()),
                statusText
            });
        }
        lblPage->setText(to_string(currentPage) + " / " + to_string(totalPages));
    };

    auto refreshListView = [=, &app]() {
        string keyword = searchBox->getText().toStdString();
        string selectedFilter = filterCombo->getSelectedItem().toStdString();
        
        vector<Product*> baseProducts;
        if (keyword.empty()) {
            baseProducts = app.getProductManager().getAllProducts();
        } else {
            baseProducts = app.getProductManager().searchProducts(keyword);
        }

        currentProducts.clear();
        for (Product* p : baseProducts) {
            if (selectedFilter == "Tất cả" || p->getCategory() == selectedFilter) {
                currentProducts.push_back(p);
            }
        }
        updatePageDisplay();
    };

    btnFirst->onPress([=]() { currentPage = 1; updatePageDisplay(); });
    btnPrev->onPress([=]() { if (currentPage > 1) { currentPage--; updatePageDisplay(); } });
    btnNext->onPress([=]() { 
        int totalPages = (currentProducts.size() + 19) / 20; 
        if (currentPage < totalPages) { currentPage++; updatePageDisplay(); } 
    });
    btnLast->onPress([=]() { 
        int totalPages = (currentProducts.size() + 19) / 20; 
        if (totalPages == 0) totalPages = 1; 
        currentPage = totalPages; 
        updatePageDisplay(); 
    });

    refreshListView();
    leftPanel->add(listView);

    // KHU VỰC PHẢI: THÔNG TIN CHI TIẾT & CHỨC NĂNG (32% chiều rộng)
    auto rightPanel = tgui::Panel::create({"32%", "90%"});
    rightPanel->setPosition("66%", "5%");
    rightPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    rightPanel->getRenderer()->setBorders(1);
    rightPanel->getRenderer()->setBorderColor(tgui::Color(200, 200, 200));
    rightPanel->getRenderer()->setRoundedBorderRadius(10);
    contentPanel->add(rightPanel);

    auto lblDetailTitle = tgui::Label::create("THÔNG TIN CHI TIẾT");
    lblDetailTitle->setPosition("5%", "4%");
    lblDetailTitle->setTextSize(22);
    lblDetailTitle->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");
    lblDetailTitle->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    lblDetailTitle->getRenderer()->setTextColor(tgui::Color(111, 78, 55));
    rightPanel->add(lblDetailTitle);

    auto productImg = tgui::Picture::create();
    productImg->setSize("35%", tgui::bindWidth(productImg));
    productImg->setPosition("32.5%", "7.5%");
    productImg->getRenderer()->setTexture("assets/images/default.png");
    
    auto imgBorder = tgui::Panel::create();
    imgBorder->setSize("35%", tgui::bindWidth(imgBorder));
    imgBorder->setPosition("32.5%", "7.5%");
    imgBorder->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    imgBorder->getRenderer()->setBorders(1);
    imgBorder->getRenderer()->setBorderColor(tgui::Color(200, 200, 200));
    
    rightPanel->add(productImg);
    rightPanel->add(imgBorder);

    // Hàm hỗ trợ tạo Label + Input form dọc
    float startY = 39.0f; 
    float rowSpacing = 9.5f;
    
    auto createInputGroup = [&](const string& labelStr, const string& placeholder, float yOffset) {
        auto lbl = tgui::Label::create(labelStr);
        lbl->setPosition("5%", (to_string(yOffset + 2.0f) + "%").c_str());
        lbl->setTextSize(20);
        lbl->getRenderer()->setTextColor(tgui::Color(90, 45, 15));
        lbl->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");
        rightPanel->add(lbl);

        auto edit = tgui::EditBox::create();
        edit->setSize("60%", 45);
        edit->setPosition("35%", (to_string(yOffset) + "%").c_str());
        edit->setTextSize(20);
        edit->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");
        edit->setDefaultText(placeholder);
        rightPanel->add(edit);
        return edit;
    };

    auto editID = createInputGroup("Mã món:", "Ví dụ: CF01", startY);
    auto editName = createInputGroup("Tên món:", "Ví dụ: Cà Phê Đen", startY + rowSpacing);
    auto editCategory = createInputGroup("Phân loại:", "Ví dụ: Cà phê", startY + rowSpacing * 2.0f);
    auto editPrice = createInputGroup("Đơn giá (VNĐ):", "Ví dụ: 20000", startY + rowSpacing * 3.0f);

    auto lblStatus = tgui::Label::create("Trạng thái:");
    lblStatus->setPosition("5%", (to_string(startY + rowSpacing * 4.0f + 2.0f) + "%").c_str());
    lblStatus->setTextSize(20);
    lblStatus->getRenderer()->setTextColor(tgui::Color(90, 45, 15));
    lblStatus->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");
    rightPanel->add(lblStatus);

    auto comboStatus = tgui::ComboBox::create();
    comboStatus->setSize("60%", 45);
    comboStatus->setPosition("35%", (to_string(startY + rowSpacing * 4.0f) + "%").c_str());
    comboStatus->setTextSize(20);
    comboStatus->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");
    comboStatus->addItem("Đang bán", "1"); 
    comboStatus->addItem("Ngừng bán", "0"); 
    rightPanel->add(comboStatus);
    
    // KHU VỰC NÚT CHỨC NĂNG (Thêm, Sửa, Xoá)
    auto btnAdd = tgui::Button::create("THÊM MỚI");
    btnAdd->setSize("28%", 45);
    btnAdd->setPosition("5%", "88%");
    btnAdd->setTextSize(20);
    btnAdd->getRenderer()->setBackgroundColor(tgui::Color(40, 167, 69)); // Xanh lá
    btnAdd->getRenderer()->setTextColor(tgui::Color::White);
    btnAdd->getRenderer()->setBackgroundColorDisabled(tgui::Color(200, 200, 200));
    btnAdd->getRenderer()->setTextColorDisabled(tgui::Color(150, 150, 150));
    btnAdd->getRenderer()->setRoundedBorderRadius(10);

    auto btnUpdate = tgui::Button::create("CẬP NHẬT");
    btnUpdate->setSize("28%", 45);
    btnUpdate->setPosition("36%", "88%");
    btnUpdate->setTextSize(20);
    btnUpdate->getRenderer()->setBackgroundColor(tgui::Color(255, 193, 7)); // Vàng cam
    btnUpdate->getRenderer()->setTextColor(tgui::Color(50, 50, 50));
    btnUpdate->getRenderer()->setBackgroundColorDisabled(tgui::Color(200, 200, 200));
    btnUpdate->getRenderer()->setTextColorDisabled(tgui::Color(150, 150, 150));
    btnUpdate->getRenderer()->setRoundedBorderRadius(10);

    auto btnDelete = tgui::Button::create("XÓA MÓN");
    btnDelete->setSize("28%", 45);
    btnDelete->setPosition("67%", "88%");
    btnDelete->setTextSize(20);
    btnDelete->getRenderer()->setBackgroundColor(tgui::Color(220, 53, 69)); // Đỏ
    btnDelete->getRenderer()->setTextColor(tgui::Color::White);
    btnDelete->getRenderer()->setBackgroundColorDisabled(tgui::Color(200, 200, 200));
    btnDelete->getRenderer()->setTextColorDisabled(tgui::Color(150, 150, 150));
    btnDelete->getRenderer()->setRoundedBorderRadius(10);

    rightPanel->add(btnAdd);
    rightPanel->add(btnUpdate);
    rightPanel->add(btnDelete);

    auto updateButtonStates = [=]() {
        bool hasSelection = !editID->isEnabled();
        bool isFilled = !editID->getText().empty() && !editName->getText().empty() && 
                    !editCategory->getText().empty() && !editPrice->getText().empty();

        if (hasSelection) {
            btnAdd->setText("HỦY CHỌN");
            btnAdd->getRenderer()->setBackgroundColor(tgui::Color(108, 117, 125));
            btnAdd->setEnabled(true);
            btnUpdate->setEnabled(true);
            btnDelete->setEnabled(true);
        } else if (isFilled) {
            btnAdd->setText("THÊM MỚI");
            btnAdd->getRenderer()->setBackgroundColor(tgui::Color(40, 167, 69));
            btnAdd->setEnabled(true);
            btnUpdate->setEnabled(false);
            btnDelete->setEnabled(false);
        } else {
            btnAdd->setText("THÊM MỚI");
            btnAdd->getRenderer()->setBackgroundColor(tgui::Color(40, 167, 69));
            btnAdd->setEnabled(false);
            btnUpdate->setEnabled(false);
            btnDelete->setEnabled(false);
        }
    };
    editID->onTextChange(updateButtonStates);
    editName->onTextChange(updateButtonStates);
    editCategory->onTextChange(updateButtonStates);
    editPrice->onTextChange(updateButtonStates);
    updateButtonStates();
    
    // SỰ KIỆN: Khi click vào 1 dòng trong bảng -> Đổ dữ liệu qua form phải
    listView->onItemSelect([=, &app](int index) {
        if (index >= 0) {
            string selectedID = listView->getItemCell(index, 0).toStdString();
            Product* p = app.getProductManager().findProductByID(selectedID);
            if (p != nullptr) {
                editID->setText(p->getProductID());
                editName->setText(p->getName());
                editCategory->setText(p->getCategory());
                editPrice->setText(to_string((long long)p->getPrice())); 
                
                editID->setEnabled(false);
                editName->setEnabled(false);
                editCategory->setEnabled(false);

                if (p->isAvailable()) {
                    comboStatus->setSelectedItemById("1");
                } else {
                    comboStatus->setSelectedItemById("0");
                }

                string imgPath = "assets/images/" + p->getProductID() + ".png";
                try {
                    productImg->getRenderer()->setTexture(imgPath.c_str());
                } catch (...){
                    productImg->getRenderer()->setTexture("assets/images/default.png");
                }
            }
            updateButtonStates();
        }
    });

    //CHỨC NĂNG 1: CẬP NHẬT GIÁ VÀ TRẠNG THÁI
    btnUpdate->onPress([=, &app](){
        string selectedID = editID->getText().toStdString();
        
        if (selectedID.empty()){
            cout << "[THÔNG BÁO] Vui lòng chọn 1 món để cập nhật!" << endl;
            return;
        }

        Product* p = app.getProductManager().findProductByID(selectedID);
        if (p == nullptr){
            cout << "[LỖI] Không tìm thấy dữ liệu món này trong hệ thống!" << endl;
            return;
        }

        auto msgBox = tgui::MessageBox::create("XÁC NHẬN CẬP NHẬT", 
            "Bạn có chắc chắn muốn cập nhật thông tin cho món " + p->getName() + " không?", 
            {"Đồng ý", "Hủy"});

        msgBox->setPosition("35%", "35%");
        msgBox->setTextSize(18);
        msgBox->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");

        msgBox->onButtonPress([=, &app](const tgui::String& button) {
            if (button == "Đồng ý") {
                try {
                    double newPrice = stod(editPrice->getText().toStdString());

                    if (!p->changePrice(newPrice)){
                        cout << "[LỖI] Giá tiền không được là số âm!" << endl;
                    } else {
                        bool isSelling = (comboStatus->getSelectedItemId() == "1");
                        if (isSelling){
                            p->markAvailable();
                        } else {
                            p->markUnavailable();
                        }

                        refreshListView();
                        
                        cout << "[THÀNH CÔNG] Đã cập nhật món: " << selectedID << endl;
                        updateButtonStates();
                    }
                } catch (...) {
                    cout << "[LỖI] Đơn giá nhập vào không hợp lệ (chỉ được nhập số)!" << endl;
                }
            }
            contentPanel->remove(msgBox);
        });

        contentPanel->add(msgBox);
    });

    //CHỨC NĂNG 2: THÊM MỚI MÓN
    btnAdd->onPress([=, &app]() {
        if (!editID->isEnabled()) {
            listView->deselectItems();

            editID->setText("");
            editName->setText("");
            editCategory->setText("");
            editPrice->setText("");
            comboStatus->setSelectedItemById("1");

            editID->setEnabled(true);
            editName->setEnabled(true);
            editCategory->setEnabled(true);

            productImg->getRenderer()->setTexture("assets/images/default.png");

            cout << "[THÔNG BÁO] Form đã dọn sạch. Hãy nhập thông tin món mới!" << endl;
            updateButtonStates();
            return;
        }

        string id = editID->getText().toStdString();
        string name = editName->getText().toStdString();
        string cat = editCategory->getText().toStdString();
        string priceStr = editPrice->getText().toStdString();

        if (id.empty() || name.empty() || cat.empty() || priceStr.empty()){
            cout << "[LỖI] Vui lòng điền đầy đủ tất cả thông tin!" << endl;
            return;
        }

        try {
            double price = stod(priceStr);
            if (price < 0){
                cout << "[LỖI] Giá tiền không được là số âm!" << endl;
                return;
            }

            if (app.getProductManager().findProductByID(id) != nullptr){
                cout << "[LỖI] Mã món '" << id << "'đã tồn tại! Vui lòng chọn mã khác." << endl;
                return;
            }
            
            if (app.getProductManager().checkNameExist(name)){
                cout << "[LỖI] Tên món '" << name << "' đã tồn tại! Vui lòng chọn tên khác." << endl;
                return;
            }

            auto msgBox = tgui::MessageBox::create("XÁC NHẬN THÊM MỚI", 
                "Bạn có chắc chắn muốn thêm món mới " + name + " vào menu không?", 
                {"Đồng ý", "Hủy"});

            msgBox->setPosition("35%", "35%");
            msgBox->setTextSize(18);
            msgBox->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");

            msgBox->onButtonPress([=, &app](const tgui::String& button) {
                if (button == "Đồng ý") {
                    bool isAvailable = (comboStatus->getSelectedItemId() == "1");
                    Product newProduct(id, name, price, cat, isAvailable);

                    app.getProductManager().addProduct(newProduct);

                    filterCombo->setSelectedItem("Tất cả");

                    refreshListView();

                    editID->setText("");
                    editName->setText("");
                    editCategory->setText("");
                    editPrice->setText("");

                    cout << "[THÀNH CÔNG] Đã thêm món mới: " << name << endl;
                    updateButtonStates();
                }
                contentPanel->remove(msgBox);
            });

            contentPanel->add(msgBox);

        } catch (...){
            cout << "[LỖI] Đơn giá nhập vào không hợp lệ (chỉ được nhập số)!" << endl;
        }
    });

    //CHỨC NĂNG 3: XOÁ MÓN
    btnDelete->onPress([=, &app](){
        string selectedID = editID->getText().toStdString();

        if (selectedID.empty()){
            cout << "[THÔNG BÁO] Vui lòng chọn 1 món từ danh sách để xoá!" << endl;
            return;
        }

        auto msgBox = tgui::MessageBox::create("XÁC NHẬN XÓA MÓN", 
            "Bạn có chắc chắn muốn xóa vĩnh viễn món " + selectedID + " khỏi hệ thống không?", 
            {"Đồng ý", "Hủy"});

        msgBox->setPosition("35%", "35%");
        msgBox->setTextSize(18);
        msgBox->getRenderer()->setFont("assets/fonts/Times-New-Roman.ttf");

        msgBox->onButtonPress([=, &app](const tgui::String& button) {
            if (button == "Đồng ý") {
                bool isDeleted = app.getProductManager().removeProduct(selectedID);

                if (isDeleted){
                    refreshListView();

                    editID->setText("");
                    editName->setText("");
                    editCategory->setText("");
                    editPrice->setText("");
                    comboStatus->setSelectedItemById("1");

                    editID->setEnabled(true);
                    editName->setEnabled(true);
                    editCategory->setEnabled(true);

                    cout << "[THÀNH CÔNG] Đã xóa món: " << selectedID << endl;
                    updateButtonStates();
                } else {
                    cout << "[LỖI] Không thể xóa! Có thể mã món không tồn tại." << endl;
                }
            }
            contentPanel->remove(msgBox);
        });

        contentPanel->add(msgBox);
    });

    auto performSearch = [=]() {
        currentPage = 1;
        refreshListView();
        
        listView->deselectItems();
        editID->setText("");
        editName->setText("");
        editCategory->setText("");
        editPrice->setText("");
        comboStatus->setSelectedItemById("1");
        
        editID->setEnabled(true);
        editName->setEnabled(true);
        editCategory->setEnabled(true);
        productImg->getRenderer()->setTexture("assets/images/default.png");
        updateButtonStates();
    };

    btnSearch->onPress(performSearch);
    searchBox->onReturnKeyPress(performSearch);
    filterCombo->onItemSelect(performSearch);
}

void loadPersonView(tgui::Panel::Ptr contentPanel, Application& app, tgui::Gui* gui) {
    contentPanel->removeAllWidgets();

    auto panelEmp = tgui::Panel::create();
    panelEmp->setSize("100%", "100%");
    panelEmp->getRenderer()->setBackgroundColor(tgui::Color(245, 240, 235));
    panelEmp->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");

    auto leftPanel = tgui::Panel::create();
    leftPanel->setPosition("24", "24");
    leftPanel->setSize("60% - 36", "100% - 80");
    leftPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    leftPanel->getRenderer()->setBorderColor(COL_FRAME_BORDER);
    leftPanel->getRenderer()->setRoundedBorderRadius(10);
    panelEmp->add(leftPanel, "leftPanel");

    auto rightPanel = tgui::Panel::create();
    rightPanel->setPosition("60% + 12", "24");
    rightPanel->setSize("40% - 36", "100% - 80");
    rightPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    rightPanel->getRenderer()->setBorderColor(COL_FRAME_BORDER);
    rightPanel->getRenderer()->setRoundedBorderRadius(10);
    panelEmp->add(rightPanel, "rightPanel");


    auto lblLeftTitle = tgui::Label::create("DANH SÁCH NHÂN VIÊN");
    lblLeftTitle->setPosition("4%", "2.5%");
    lblLeftTitle->setTextSize(26);
    lblLeftTitle->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    lblLeftTitle->getRenderer()->setTextColor(tgui::Color(120, 60, 30));
    lblLeftTitle->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    leftPanel->add(lblLeftTitle);
    lblLeftTitle->setTextSize(26);

    auto searchEmp = tgui::EditBox::create();
    searchEmp->setPosition("4%", tgui::bindBottom(lblLeftTitle) + 18);
    searchEmp->setSize("36%", 60);
    searchEmp->setTextSize(20);
    searchEmp->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    searchEmp->setDefaultText("Tìm theo tên nhân viên...");
    searchEmp->getRenderer()->setRoundedBorderRadius(10);
    searchEmp->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    leftPanel->add(searchEmp, "searchEmp");

    auto filterShift = tgui::ComboBox::create();
    filterShift->setPosition("42%", tgui::bindTop(searchEmp));
    filterShift->setSize("20%", 60);
    filterShift->setTextSize(20);
    filterShift->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    filterShift->getRenderer()->setRoundedBorderRadius(10);
    filterShift->addItem("Tất cả ca", "All");
    filterShift->addItem("Sáng", "Sáng");
    filterShift->addItem("Chiều", "Chiều");
    filterShift->addItem("Tối", "Tối");
    filterShift->addItem("Hành chính", "Hành chính");
    filterShift->setSelectedItemById("All");
    leftPanel->add(filterShift, "filterShift");

    auto filterRole = tgui::ComboBox::create();
    filterRole->setPosition("64%", tgui::bindTop(searchEmp));
    filterRole->setSize("22%", 60);
    filterRole->setTextSize(20);
    filterRole->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    filterRole->addItem("Tất cả chức vụ", "All");
    filterRole->addItem("Quản lí", "1");
    filterRole->addItem("Nhân viên", "0");
    filterRole->getRenderer()->setRoundedBorderRadius(10);
    filterRole->setSelectedItemById("All");
    leftPanel->add(filterRole, "filterRole");

    auto btnSearch = tgui::Button::create("Tìm");
    btnSearch->setPosition("88%", tgui::bindTop(searchEmp));
    btnSearch->setSize("8%", 60);
    btnSearch->setTextSize(20);
    btnSearch->getRenderer()->setBackgroundColor(tgui::Color(160, 138, 120));
    btnSearch->getRenderer()->setTextColor(tgui::Color::White);
    btnSearch->getRenderer()->setBorders(0);
    btnSearch->getRenderer()->setRoundedBorderRadius(10);
    btnSearch->getRenderer()->setFont("assets/fonts/BeVietnamPro-SemiBold.ttf");
    leftPanel->add(btnSearch, "btnSearch");

    auto listEmp = tgui::ListView::create();
    listEmp->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    listEmp->getRenderer()->setTextSize(20);
    listEmp->setHeaderTextSize(22);
    listEmp->setTextSize(20);
    listEmp->getRenderer()->setHeaderTextColor(tgui::Color(20, 20, 20));
    listEmp->getRenderer()->setTextColor(tgui::Color(70, 70, 70));

    listEmp->setPosition("4%", tgui::bindBottom(searchEmp) + 18);
    listEmp->setSize("92%", "100% - y - 96");
    listEmp->addColumn("MÃ NV", 180);
    listEmp->addColumn("HỌ TÊN", 0);
    listEmp->addColumn("CHỨC VỤ", 220);
    listEmp->addColumn("CA LÀM", 220);
    listEmp->addColumn("TRẠNG THÁI", 220);
    listEmp->setColumnExpanded(1, true);
    listEmp->getRenderer()->setBorderColor(COL_FRAME_BORDER);

    listEmp->setHeaderHeight(56);
    listEmp->setItemHeight(52);
    leftPanel->add(listEmp, "listEmp");

    // CỘT PHẢI: BỐ CỤC CHIA ĐỀU KHẮP RIGHTPANEL
    auto lblRightTitle = tgui::Label::create("THÔNG TIN CHI TIẾT");
    lblRightTitle->setPosition("4.5%", "2.5%");
    lblRightTitle->setTextSize(26);
    lblRightTitle->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    lblRightTitle->getRenderer()->setTextColor(tgui::Color(120, 60, 30));
    lblRightTitle->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    rightPanel->add(lblRightTitle);
    lblRightTitle->setTextSize(26);

    auto styleFormLabel = [](tgui::Label::Ptr lbl) {
        lbl->setTextSize(22);
        lbl->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
        lbl->getRenderer()->setTextStyle(tgui::TextStyle::Regular);
        lbl->getRenderer()->setTextColor(tgui::Color(70, 55, 45));
    };

    // --- DÒNG 1 ---
    auto lblID = tgui::Label::create("Mã NV");
    lblID->setPosition("4.5%", tgui::bindBottom(lblRightTitle) + 44);
    styleFormLabel(lblID);
    rightPanel->add(lblID);
    
    auto editEmpID = tgui::EditBox::create();
    editEmpID->setPosition("4.5%", tgui::bindBottom(lblID) + 8);
    editEmpID->setSize("44.5%", 66);
    editEmpID->setTextSize(20);
    editEmpID->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    editEmpID->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    editEmpID->getRenderer()->setRoundedBorderRadius(10);
    editEmpID->setDefaultText("Ví dụ: QL01");
    editEmpID->getRenderer()->setTextColor(tgui::Color(80, 70, 60));
    rightPanel->add(editEmpID, "editEmpID");

    auto lblName = tgui::Label::create("Họ tên");
    lblName->setPosition("51.0%", tgui::bindTop(lblID));
    styleFormLabel(lblName);
    rightPanel->add(lblName);
    
    auto editEmpName = tgui::EditBox::create();
    editEmpName->setPosition("51.0%", tgui::bindBottom(lblName) + 8);
    editEmpName->setSize("44.5%", 66);
    editEmpName->setTextSize(20);
    editEmpName->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    editEmpName->getRenderer()->setRoundedBorderRadius(10);
    editEmpName->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    editEmpName->setDefaultText("Nhập họ tên");
    rightPanel->add(editEmpName, "editEmpName");


    // --- DÒNG 2 ---
    auto lblGender = tgui::Label::create("Giới tính");
    lblGender->setPosition("4.5%", tgui::bindBottom(editEmpID) + 44);
    styleFormLabel(lblGender);
    rightPanel->add(lblGender);
    
    auto radioMale = tgui::RadioButton::create();
    radioMale->setText("Nam");
    radioMale->setSize(28, 28);
    radioMale->setTextSize(20);
    radioMale->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    radioMale->getRenderer()->setTextColor(tgui::Color(60, 50, 45));
    radioMale->setPosition("4.5%", tgui::bindBottom(lblGender) + 44);
    rightPanel->add(radioMale, "radioMale");
    
    auto radioFemale = tgui::RadioButton::create();
    radioFemale->setText("Nữ");
    radioFemale->setSize(28, 28);
    radioFemale->setTextSize(20);
    radioFemale->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    radioFemale->getRenderer()->setTextColor(tgui::Color(60, 50, 45));
    radioFemale->setPosition("4.5% + 110", tgui::bindTop(radioMale));
    rightPanel->add(radioFemale, "radioFemale");

    auto lblDOB = tgui::Label::create("Ngày sinh");
    lblDOB->setPosition("51.0%", tgui::bindTop(lblGender));
    styleFormLabel(lblDOB);
    rightPanel->add(lblDOB);
    
    auto editEmpDOB = tgui::EditBox::create();
    editEmpDOB->setInputValidator("[0-9/]*");
    editEmpDOB->setMaximumCharacters(10);
    editEmpDOB->setPosition("51.0%", tgui::bindBottom(lblDOB) + 8);
    editEmpDOB->setSize("44.5%", 66);
    editEmpDOB->setTextSize(20);
    editEmpDOB->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    editEmpDOB->getRenderer()->setRoundedBorderRadius(10);
    editEmpDOB->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    editEmpDOB->setDefaultText("dd/mm/yyyy");
    rightPanel->add(editEmpDOB, "editEmpDOB");


    // --- DÒNG 3 ---
    auto lblPhone = tgui::Label::create("Số điện thoại");
    lblPhone->setPosition("4.5%", tgui::bindBottom(editEmpDOB) + 44);
    styleFormLabel(lblPhone);
    rightPanel->add(lblPhone);
    
    auto editEmpPhone = tgui::EditBox::create();
    editEmpPhone->setMaximumCharacters(10);
    editEmpPhone->setInputValidator("[0-9]*");
    editEmpPhone->setPosition("4.5%", tgui::bindBottom(lblPhone) + 8);
    editEmpPhone->setSize("44.5%", 66);
    editEmpPhone->setTextSize(20);
    editEmpPhone->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    editEmpPhone->getRenderer()->setRoundedBorderRadius(10);
    editEmpPhone->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    editEmpPhone->setDefaultText("Nhập số điện thoại");
    rightPanel->add(editEmpPhone, "editEmpPhone");

    auto lblRole = tgui::Label::create("Chức vụ");
    lblRole->setPosition("51.0%", tgui::bindTop(lblPhone));
    styleFormLabel(lblRole);
    rightPanel->add(lblRole);
    
    auto comboEmpRole = tgui::ComboBox::create();
    comboEmpRole->setPosition("51.0%", tgui::bindBottom(lblRole) + 8);
    comboEmpRole->setSize("44.5%", 66);
    comboEmpRole->setTextSize(20);
    comboEmpRole->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    comboEmpRole->getRenderer()->setRoundedBorderRadius(10);
    comboEmpRole->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    comboEmpRole->addItem("Quản lí", "1");
    comboEmpRole->addItem("Nhân viên", "0");
    comboEmpRole->setDefaultText("Chọn chức vụ");
    rightPanel->add(comboEmpRole, "comboEmpRole");


    // --- DÒNG 4 ---
    auto lblSalary = tgui::Label::create("Lương theo giờ (VNĐ)");
    lblSalary->setPosition("4.5%", tgui::bindBottom(editEmpPhone) + 44);
    styleFormLabel(lblSalary);
    rightPanel->add(lblSalary);
    
    auto editEmpSalary = tgui::EditBox::create();
    editEmpSalary->setInputValidator("[0-9]*");
    editEmpSalary->setPosition("4.5%", tgui::bindBottom(lblSalary) + 8);
    editEmpSalary->setSize("44.5%", 66);
    editEmpSalary->setTextSize(20);
    editEmpSalary->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    editEmpSalary->getRenderer()->setRoundedBorderRadius(10);
    editEmpSalary->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    editEmpSalary->setDefaultText("Ví dụ: 25000");
    rightPanel->add(editEmpSalary, "editEmpSalary");

    auto lblShift = tgui::Label::create("Ca làm");
    lblShift->setPosition("51.0%", tgui::bindTop(lblSalary));
    styleFormLabel(lblShift);
    rightPanel->add(lblShift);
    
    auto comboEmpShift = tgui::ComboBox::create();
    comboEmpShift->setPosition("51.0%", tgui::bindBottom(lblShift) + 8);
    comboEmpShift->setSize("44.5%", 66);
    comboEmpShift->setTextSize(20);
    comboEmpShift->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    comboEmpShift->getRenderer()->setRoundedBorderRadius(10);
    comboEmpShift->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    comboEmpShift->addItem("Sáng", "Sáng");
    comboEmpShift->addItem("Chiều", "Chiều");
    comboEmpShift->addItem("Tối", "Tối");
    comboEmpShift->addItem("Hành chính", "Hành chính");
    comboEmpShift->setDefaultText("Chọn ca");
    rightPanel->add(comboEmpShift, "comboEmpShift");


    // --- DÒNG 5 ---
    auto lblHire = tgui::Label::create("Ngày thuê");
    lblHire->setPosition("4.5%", tgui::bindBottom(editEmpSalary) + 44);
    styleFormLabel(lblHire);
    rightPanel->add(lblHire);
    
    auto editEmpHireDate = tgui::EditBox::create();
    editEmpHireDate->setInputValidator("[0-9/]*");
    editEmpHireDate->setMaximumCharacters(10);
    editEmpHireDate->setPosition("4.5%", tgui::bindBottom(lblHire) + 8);
    editEmpHireDate->setSize("44.5%", 66);
    editEmpHireDate->setTextSize(20);
    editEmpHireDate->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    editEmpHireDate->getRenderer()->setRoundedBorderRadius(10);
    editEmpHireDate->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    editEmpHireDate->setDefaultText("dd/mm/yyyy");
    rightPanel->add(editEmpHireDate, "editEmpHireDate");

    auto lblStatus = tgui::Label::create("Trạng thái");
    lblStatus->setPosition("51.0%", tgui::bindTop(lblHire));
    styleFormLabel(lblStatus);
    rightPanel->add(lblStatus);
    
    auto comboEmpStatus = tgui::ComboBox::create();
    comboEmpStatus->setPosition("51.0%", tgui::bindBottom(lblStatus) + 8);
    comboEmpStatus->setSize("44.5%", 66);
    comboEmpStatus->setTextSize(20);
    comboEmpStatus->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    comboEmpStatus->getRenderer()->setRoundedBorderRadius(10);
    comboEmpStatus->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);
    comboEmpStatus->addItem("Đang làm", "1");
    comboEmpStatus->addItem("Đã nghỉ việc", "0");
    comboEmpStatus->setSelectedItemById("1");
    rightPanel->add(comboEmpStatus, "comboEmpStatus");


    // --- CỤM NÚT THAO TÁC (Thêm mới, Cập nhật, Cho nghỉ việc) ---
    auto btnAddEmp = tgui::Button::create("THÊM MỚI");
    btnAddEmp->setPosition("4.5%", tgui::bindBottom(editEmpHireDate) + 60); 
    btnAddEmp->setSize("29.0%", 66);
    btnAddEmp->setTextSize(20);
    btnAddEmp->getRenderer()->setRoundedBorderRadius(10);
    btnAddEmp->getRenderer()->setBorders(0);
    btnAddEmp->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    rightPanel->add(btnAddEmp, "btnAddEmp");

    auto btnUpdateEmp = tgui::Button::create("CẬP NHẬT");
    btnUpdateEmp->setPosition("50% - width/2", tgui::bindTop(btnAddEmp));
    btnUpdateEmp->setSize("29.0%", 66);
    btnUpdateEmp->setTextSize(20);
    btnUpdateEmp->getRenderer()->setRoundedBorderRadius(10);
    btnUpdateEmp->getRenderer()->setBorders(0);
    btnUpdateEmp->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    rightPanel->add(btnUpdateEmp, "btnUpdateEmp");

    auto btnDeleteEmp = tgui::Button::create("CHO NGHỈ VIỆC");
    btnDeleteEmp->setPosition("95.5% - width", tgui::bindTop(btnAddEmp));
    btnDeleteEmp->setSize("29.0%", 66);
    btnDeleteEmp->setTextSize(20);
    btnDeleteEmp->getRenderer()->setRoundedBorderRadius(10);
    btnDeleteEmp->getRenderer()->setBorders(0);
    btnDeleteEmp->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    rightPanel->add(btnDeleteEmp, "btnDeleteEmp");

    // Hàm Helper tự động đổi màu và trạng thái nút
    auto setButtonState = [](tgui::Button::Ptr btn, bool isActive, tgui::Color primaryBg) {
        btn->setEnabled(isActive);
        if (isActive) {
            btn->getRenderer()->setBackgroundColor(primaryBg);
            btn->getRenderer()->setTextColor(tgui::Color::White);
        } else {
            btn->getRenderer()->setBackgroundColorDisabled(tgui::Color(235, 230, 222)); // Màu nền khi Disabled
            btn->getRenderer()->setTextColorDisabled(tgui::Color(120, 115, 110)); // Màu chữ khi Disabled
        }
    };

    setButtonState(btnAddEmp, false, tgui::Color(31, 107, 58));
    setButtonState(btnUpdateEmp, false, tgui::Color(47, 95, 138));
    setButtonState(btnDeleteEmp, false, tgui::Color(163, 45, 45));

    auto formatDateInput = [](tgui::String text, tgui::EditBox::Ptr editBox) {
        std::string s = text.toStdString();
        std::string raw = "";
        for (char c : s) {
            if (c >= '0' && c <= '9') raw += c;
        }
        
        std::string formatted = "";
        for (size_t i = 0; i < raw.length(); ++i) {
            if (i == 2 || i == 4) formatted += "/";
            formatted += raw[i];
        }
        
        if (formatted.length() > 10) formatted = formatted.substr(0, 10);
        
        if (formatted != s) {
            editBox->setText(formatted);
            editBox->setCaretPosition(formatted.length());
        }
    };

    comboEmpRole->onItemSelect([=]() {
        if (editEmpID->isReadOnly()) return; 

        string prefix = (comboEmpRole->getSelectedItemId() == "1") ? "QL" : "NV";
        
        string currentText = editEmpID->getText().toStdString();
        string digits = "";
        for (char c : currentText) {
            if (c >= '0' && c <= '9') digits += c;
        }
        
        editEmpID->setText(prefix + digits);
    });

    editEmpID->onTextChange([=](tgui::String text) {
        if (editEmpID->isReadOnly()) return;
        
        string s = text.toStdString();
        string digits = "";
        for (char c : s) {
            if (c >= '0' && c <= '9') digits += c;
        }

        if (comboEmpRole->getSelectedItemIndex() == -1) {
            if (digits != s) {
                editEmpID->setText(digits);
                editEmpID->setCaretPosition(digits.length());
            }
            return;
        }
        
        string prefix = (comboEmpRole->getSelectedItemId() == "1") ? "QL" : "NV";
        string formatted = prefix + digits;
        
        if (formatted != s) {
            editEmpID->setText(formatted);
            editEmpID->setCaretPosition(formatted.length());
        }
    });

    editEmpDOB->onTextChange([=](tgui::String text) { formatDateInput(text, editEmpDOB); });
    editEmpHireDate->onTextChange([=](tgui::String text) { formatDateInput(text, editEmpHireDate); }); 

    auto checkFormValid = [=]() {
        if (editEmpID->isReadOnly()) return; 

        bool isFull = !editEmpID->getText().empty() &&
                      !editEmpName->getText().empty() &&
                      (radioMale->isChecked() || radioFemale->isChecked()) &&
                      !editEmpDOB->getText().empty() &&
                      !editEmpPhone->getText().empty() &&
                      comboEmpRole->getSelectedItemIndex() != -1 &&
                      !editEmpSalary->getText().empty() &&
                      comboEmpShift->getSelectedItemIndex() != -1 &&
                      !editEmpHireDate->getText().empty();

        setButtonState(btnAddEmp, isFull, tgui::Color(31, 107, 58));
    };

    editEmpID->onTextChange(checkFormValid);
    editEmpName->onTextChange(checkFormValid);
    editEmpDOB->onTextChange(checkFormValid);
    editEmpPhone->onTextChange(checkFormValid);
    editEmpSalary->onTextChange(checkFormValid);
    editEmpHireDate->onTextChange(checkFormValid);
    radioMale->onCheck(checkFormValid);
    radioFemale->onCheck(checkFormValid);
    comboEmpRole->onItemSelect(checkFormValid);
    comboEmpShift->onItemSelect(checkFormValid);

    // --- Nút Làm mới biểu mẫu ---
    auto btnClearForm = tgui::Button::create("Làm mới biểu mẫu");
    btnClearForm->setPosition("50% - width/2", tgui::bindBottom(btnAddEmp) + 36);
    btnClearForm->setTextSize(20);
    btnClearForm->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    btnClearForm->getRenderer()->setTextColor(tgui::Color(160, 82, 45));
    btnClearForm->getRenderer()->setBorders(0);
    btnClearForm->getRenderer()->setFont("assets/fonts/BeVietnamPro-SemiBold.ttf");

    btnClearForm->onPress([=]() {
        editEmpID->setReadOnly(false);
        editEmpName->setReadOnly(false);
        radioMale->setEnabled(true);
        radioFemale->setEnabled(true);
        editEmpDOB->setReadOnly(false);
        editEmpHireDate->setReadOnly(false);

        tgui::Color defaultBg = editEmpPhone->getRenderer()->getBackgroundColor();
        editEmpID->getRenderer()->setBackgroundColor(defaultBg);
        editEmpName->getRenderer()->setBackgroundColor(defaultBg);
        editEmpDOB->getRenderer()->setBackgroundColor(defaultBg);
        editEmpHireDate->getRenderer()->setBackgroundColor(defaultBg);

        comboEmpRole->deselectItem();
    
        editEmpID->setText("");
        editEmpName->setText("");
        radioMale->setChecked(false);
        radioFemale->setChecked(false);
        editEmpDOB->setText("");
        editEmpPhone->setText("");
        editEmpSalary->setText("");
        comboEmpShift->deselectItem();
        editEmpHireDate->setText("");
        comboEmpStatus->setSelectedItemById("1");
    
        setButtonState(btnAddEmp, false, tgui::Color(31, 107, 58));
        setButtonState(btnUpdateEmp, false, tgui::Color(47, 95, 138));
        setButtonState(btnDeleteEmp, false, tgui::Color(163, 45, 45));
    });
    rightPanel->add(btnClearForm, "btnClearForm");

    static int currentPage = 1;
    static int itemsPerPage = 20;
    currentPage = 1;

    auto btnFirst = tgui::Button::create("<<");
    auto btnPrev = tgui::Button::create("<");
    auto lblPageInfo = tgui::Label::create("1 / 1");
    auto btnNext = tgui::Button::create(">");
    auto btnLast = tgui::Button::create(">>");

    // Hàm Lambda định hình Style chuẩn theo thiết kế toàn ứng dụng
    auto stylePageButton = [](tgui::Button::Ptr btn) {
        btn->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
        btn->getRenderer()->setTextSize(20);
        btn->getRenderer()->setRoundedBorderRadius(10);
        btn->getRenderer()->setBorders(1);

        // Trạng thái bình thường (Tone Be/Trắng đồng bộ COL_FRAME_BORDER)
        btn->getRenderer()->setBackgroundColor(tgui::Color(255, 252, 248));
        btn->getRenderer()->setTextColor(tgui::Color(90, 75, 65));
        btn->getRenderer()->setBorderColor(COL_TEXTBOX_BORDER);

        // Trạng thái Hover khi rê chuột vào
        btn->getRenderer()->setBackgroundColorHover(tgui::Color(240, 230, 220));
        btn->getRenderer()->setTextColorHover(tgui::Color(50, 40, 30));

        // Trạng thái Disabled khi ở trang đầu/trang cuối (chìm nhẹ nhàng)
        btn->getRenderer()->setBackgroundColorDisabled(tgui::Color(245, 242, 238));
        btn->getRenderer()->setTextColorDisabled(tgui::Color(185, 180, 175));
        btn->getRenderer()->setBorderColorDisabled(tgui::Color(230, 225, 220));
    };

    stylePageButton(btnFirst);
    stylePageButton(btnPrev);
    stylePageButton(btnNext);
    stylePageButton(btnLast);

    lblPageInfo->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    lblPageInfo->setTextSize(20);
    lblPageInfo->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    lblPageInfo->setVerticalAlignment(tgui::VerticalAlignment::Center);
    lblPageInfo->getRenderer()->setTextColor(tgui::Color(90, 60, 40));

    // Căn chỉnh vị trí bám sát mép phải 96% (mép phải của listEmp)
    const char* yPos = "100% - 68";

    btnLast->setPosition("96% - 44", yPos);
    btnLast->setSize(45, 45);

    btnNext->setPosition(tgui::bindLeft(btnLast) - 52, yPos);
    btnNext->setSize(45, 45);

    lblPageInfo->setPosition(tgui::bindLeft(btnNext) - 108, yPos);
    lblPageInfo->setSize(100, 45);

    btnPrev->setPosition(tgui::bindLeft(lblPageInfo) - 52, yPos);
    btnPrev->setSize(45, 45);

    btnFirst->setPosition(tgui::bindLeft(btnPrev) - 52, yPos);
    btnFirst->setSize(45, 45);

    leftPanel->add(btnFirst);
    leftPanel->add(btnPrev);
    leftPanel->add(lblPageInfo);
    leftPanel->add(btnNext);
    leftPanel->add(btnLast);

    auto refreshListView = [=, &app]() {
        listEmp->removeAllItems();
        string keyword = searchEmp->getText().toStdString();
        string shiftFilter = filterShift->getSelectedItemId().toStdString();
        string roleFilter = filterRole->getSelectedItemId().toStdString();

        vector<Person*> filteredPersons = app.getPersonManager().searchPersons(keyword, shiftFilter, roleFilter);

        int totalItems = filteredPersons.size();
        int totalPages = (totalItems == 0) ? 1 : (totalItems + itemsPerPage - 1) / itemsPerPage;

        if (currentPage < 1) currentPage = 1;
        if (currentPage > totalPages) currentPage = totalPages;

        int startIndex = (currentPage - 1) * itemsPerPage;
        int endIndex = min(startIndex + itemsPerPage, totalItems);

        for (int i = startIndex; i < endIndex; i++){
            Person* p = filteredPersons[i];

            string roleStr = ((int)p->getAccount().getRole() == 1) ? "Quản lí" : "Nhân viên";
            string statusStr = (p->getAccount().isActive()) ? "Đang làm" : "Đã nghỉ việc";

            listEmp->addItem({p->getPersonID(), p->getFullName(), roleStr, p->getShift(), statusStr});
        }

        lblPageInfo->setText(to_string(currentPage) + " / " + to_string(totalPages));
        btnFirst->setEnabled(currentPage > 1);
        btnPrev->setEnabled(currentPage > 1);
        btnNext->setEnabled(currentPage < totalPages);
        btnLast->setEnabled(currentPage < totalPages);
    };

    btnFirst->onPress([=]() {
        currentPage = 1;
        refreshListView();
    });

    btnPrev->onPress([=]() {
        currentPage--;
        refreshListView();
    });

    btnNext->onPress([=]() {
        currentPage++;
        refreshListView();
    });

    btnLast->onPress([=]() {
        currentPage = 99999; 
        refreshListView();
    });

    refreshListView();

    btnSearch->onPress(refreshListView);
    filterShift->onItemSelect(refreshListView);
    filterRole->onItemSelect(refreshListView);

    btnAddEmp->onPress([=, &app]() {
        if (!editEmpName || !radioMale || !radioFemale || !editEmpDOB || !editEmpPhone || !comboEmpRole || !editEmpSalary || !comboEmpShift || !editEmpHireDate || !btnClearForm) {
            return;
        }

        string name = editEmpName->getText().toStdString();
        string gender = radioMale->isChecked() ? "Nam" : (radioFemale->isChecked() ? "Nữ" : "");
        
        int dob = 0;
        if (!editEmpDOB->getText().empty()) {
            try { 
                dob = stoi(editEmpDOB->getText().toStdString()); 
            } catch (...) { 
                dob = 0; 
            }
        }
        
        string phone = editEmpPhone->getText().toStdString();
        int role = (comboEmpRole->getSelectedItemId() == "1") ? 1 : 0;
        
        int salary = 0;
        if (!editEmpSalary->getText().empty()) {
            try { 
                salary = stoi(editEmpSalary->getText().toStdString()); 
            } catch (...) { 
                salary = 0; 
            }
        }
        
        string shift = comboEmpShift->getSelectedItemId().toStdString();
        string hireDate = editEmpHireDate->getText().toStdString();
        
        if (name.empty() || gender.empty() || phone.empty() || shift.empty()) {
            return;
        }

        string empID = editEmpID->getText().toStdString();
        string roleStr = (role == 1) ? "Quản lí" : "Nhân viên";
        string salaryStr = to_string(salary) + " VNĐ";

        auto detailPanel = tgui::Panel::create();
        
        auto addInfoRow = [&](tgui::String title, tgui::String value, int y) {
            auto lblTitle = tgui::Label::create(title);
            lblTitle->setPosition(20, y);
            lblTitle->setTextSize(20);
            lblTitle->getRenderer()->setTextColor(tgui::Color(100, 100, 100));
            detailPanel->add(lblTitle);
            
            auto lblValue = tgui::Label::create(value);
            lblValue->setPosition("100% - width - 20", y);
            lblValue->setTextSize(20);
            lblValue->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
            detailPanel->add(lblValue);
        };

        addInfoRow("Mã NV", empID, 20);
        addInfoRow("Họ tên", name, 65);
        addInfoRow("Chức vụ", roleStr, 110);
        addInfoRow("Ca làm", shift, 155);
        addInfoRow("Lương theo giờ", salaryStr, 200);

        showConfirmDialog(
            gui, 
            "Xác nhận thêm nhân viên", 
            "Bạn có chắc chắn muốn thêm nhân viên mới này vào hệ thống không?", 
            "?", 
            tgui::Color(160, 82, 45), 
            detailPanel, 
            [=, &app]() {
                string lowerEmpID = empID;
                for (char& c : lowerEmpID) {
                    c = tolower(c);
                }
                
                string username = (role == 1 ? "admin" : "staff") + lowerEmpID;
                string rawDOB = "";
                string dobStr = editEmpDOB->getText().toStdString();

                for (char c : dobStr) {
                    if (c != '/') rawDOB += c;
                }

                string password = lowerEmpID + rawDOB;
                Account newAcc(username, password, true, (Role)role);
                string phoneStr = editEmpPhone->getText().toStdString();
                string hireDateStr = editEmpHireDate->getText().toStdString();
                
                Person newEmp(empID, name, phoneStr, salary, hireDateStr, newAcc, gender, shift, dobStr);
                app.getPersonManager().addPerson(newEmp);
                refreshListView();
                btnClearForm->onPress.emit(btnClearForm.get(), "");
            }
        );
    });

    btnUpdateEmp->onPress([=, &app]() {
        string empID = editEmpID->getText().toStdString();
        if (empID.empty()) return;

        string newPhone = editEmpPhone->getText().toStdString();
        int role = (comboEmpRole->getSelectedItemId() == "1") ? 1 : 0;
        string shift = comboEmpShift->getSelectedItemId().toStdString();
        
        int newSalary = 0;
        if (!editEmpSalary->getText().empty()) {
            try { newSalary = stoi(editEmpSalary->getText().toStdString()); } 
            catch (...) { newSalary = 0; }
        }

        Person* oldP = app.getPersonManager().findPersonByID(empID);
        if (!oldP) return;
        
        string oldPhone = oldP->getPhone();
        string oldRole = ((int)oldP->getAccount().getRole() == 1) ? "Quản lí" : "Nhân viên";
        string oldShift = oldP->getShift();
        int oldSalary = oldP->getSalary();
        string oldName = oldP->getFullName();
        string newRole = (role == 1) ? "Quản lí" : "Nhân viên";

        auto detailPanel = tgui::Panel::create();
        
        int currentY = 15;
        
        auto addChangeRow = [&](tgui::String title, tgui::String oldVal, tgui::String newVal) {
            auto lblTitle = tgui::Label::create(title);
            lblTitle->setPosition(20, currentY);
            lblTitle->setTextSize(18);
            lblTitle->getRenderer()->setTextColor(tgui::Color(100, 100, 100));
            detailPanel->add(lblTitle);
            
            if (oldVal == newVal) {
                auto lblVal = tgui::Label::create(oldVal);
                lblVal->setPosition("100% - width - 20", currentY);
                lblVal->setTextSize(18);
                lblVal->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
                detailPanel->add(lblVal);
            } else {
                auto lblNewVal = tgui::Label::create(newVal);
                lblNewVal->setPosition("100% - width - 20", currentY);
                lblNewVal->setTextSize(18);
                lblNewVal->getRenderer()->setTextColor(tgui::Color(31, 107, 58));
                lblNewVal->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
                detailPanel->add(lblNewVal);
                
                auto lblArrow = tgui::Label::create(u8"➜");
                lblArrow->setPosition(tgui::bindLeft(lblNewVal) - tgui::bindWidth(lblArrow) - 10, currentY);
                lblArrow->setTextSize(18);
                lblArrow->getRenderer()->setTextColor(tgui::Color(100, 100, 100));
                detailPanel->add(lblArrow);
                
                auto lblOldVal = tgui::Label::create(oldVal);
                lblOldVal->setPosition(tgui::bindLeft(lblArrow) - tgui::bindWidth(lblOldVal) - 10, currentY);
                lblOldVal->setTextSize(18);
                lblOldVal->getRenderer()->setTextColor(tgui::Color(160, 160, 160));
                detailPanel->add(lblOldVal);
            }
            currentY += 45;
        };

        auto lblTitle = tgui::Label::create("Nhân viên");
        lblTitle->setPosition(20, currentY);
        lblTitle->setTextSize(18);
        lblTitle->getRenderer()->setTextColor(tgui::Color(100, 100, 100));
        detailPanel->add(lblTitle);
        
        auto lblEmpInfo = tgui::Label::create(empID + " - " + oldName);
        lblEmpInfo->setPosition("100% - width - 20", currentY);
        lblEmpInfo->setTextSize(18);
        lblEmpInfo->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
        detailPanel->add(lblEmpInfo);
        currentY += 45;

        int changesCount = 0;

        if (oldPhone != newPhone) {
            addChangeRow("Số điện thoại", oldPhone, newPhone);
            changesCount++;
        }
        if (oldRole != newRole) {
            addChangeRow("Chức vụ", oldRole, newRole);
            changesCount++;
        }
        if (oldSalary != newSalary) {
            addChangeRow("Lương theo giờ", to_string(oldSalary) + " VND", to_string(newSalary) + " VND");
            changesCount++;
        }
        if (oldShift != shift) {
            addChangeRow("Ca làm", oldShift, shift);
            changesCount++;
        }

        if (changesCount > 0 && changesCount < 4) {
            auto lblNotice = tgui::Label::create("Các thông tin còn lại không thay đổi.");
            lblNotice->setPosition(20, currentY);
            lblNotice->setTextSize(18);
            lblNotice->getRenderer()->setTextColor(tgui::Color(100, 100, 100));
            detailPanel->add(lblNotice);
            currentY += 45;
        }

        detailPanel->setSize(0, currentY);

        showConfirmDialog(
            gui, 
            "Xác nhận cập nhật thông tin", 
            "Bạn có chắc chắn muốn cập nhật thông tin mới cho nhân viên này không?", 
            "i", 
            tgui::Color(0, 102, 204), 
            detailPanel, 
            [=, &app]() {
                app.getPersonManager().updatePersonInfo(empID, newPhone, newSalary, (Role)role, shift);
                btnClearForm->onPress.emit(btnClearForm.get(), "");
            }
        );
    });

    btnDeleteEmp->onPress([=, &app]() {
        string empID = editEmpID->getText().toStdString();
        string name = editEmpName->getText().toStdString();
        if (empID.empty()) return;

        auto detailPanel = tgui::Panel::create();
        
        auto addInfoRow = [&](tgui::String title, tgui::String value, int y) {
            auto lblTitle = tgui::Label::create(title);
            lblTitle->setPosition(20, y);
            lblTitle->setTextSize(20);
            lblTitle->getRenderer()->setTextColor(tgui::Color(100, 100, 100));
            detailPanel->add(lblTitle);
            
            auto lblValue = tgui::Label::create(value);
            lblValue->setPosition("100% - width - 20", y);
            lblValue->setTextSize(20);
            lblValue->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
            detailPanel->add(lblValue);
        };

        addInfoRow("Mã NV", empID, 20);
        addInfoRow("Họ tên", name, 65);
        detailPanel->setSize(0, 110);

        showConfirmDialog(
            gui, 
            "Xác nhận cho nghỉ việc", 
            "Bạn có chắc chắn muốn cho nhân viên này nghỉ việc không?", 
            "X", 
            tgui::Color(204, 0, 0), 
            detailPanel, 
            [=, &app]() {
                app.getPersonManager().disablePerson(empID);
                btnClearForm->onPress.emit(btnClearForm.get(), "");
            }
        );
    });

    listEmp->onItemSelect([=, &app](int index) {
        if (index < 0) return;

        editEmpID->setReadOnly(true);
        editEmpName->setReadOnly(true);
        radioMale->setEnabled(false);
        radioFemale->setEnabled(false);
        editEmpDOB->setReadOnly(true);
        editEmpHireDate->setReadOnly(true);

        editEmpID->getRenderer()->setBackgroundColor(tgui::Color(242, 236, 230));
        editEmpName->getRenderer()->setBackgroundColor(tgui::Color(242, 236, 230));
        editEmpDOB->getRenderer()->setBackgroundColor(tgui::Color(242, 236, 230));
        editEmpHireDate->getRenderer()->setBackgroundColor(tgui::Color(242, 236, 230));

        string selectedID = listEmp->getItemCell(index, 0).toStdString();
        Person* p = app.getPersonManager().findPersonByID(selectedID);
        
        if (p) {
            editEmpID->setText(p->getPersonID());
            editEmpName->setText(p->getFullName());
            
            if (p->getGender() == "Nam") {
                radioMale->setChecked(true);
            } else if (p->getGender() == "Nữ") {
                radioFemale->setChecked(true);
            } else {
                radioMale->setChecked(false);
                radioFemale->setChecked(false);
            }
            
            editEmpDOB->setText(p->getDob());
            editEmpPhone->setText(p->getPhone());
            
            comboEmpRole->setSelectedItemById(to_string((int)p->getAccount().getRole()));
            editEmpSalary->setText(to_string((int)p->getSalary()));
            comboEmpShift->setSelectedItemById(p->getShift());
            editEmpHireDate->setText(p->getHireDate());
            
            if (p->getAccount().isActive()) {
                comboEmpStatus->setSelectedItemById("1");
            } else {
                comboEmpStatus->setSelectedItemById("0");
            }
        }
        setButtonState(btnAddEmp, false, tgui::Color(31, 107, 58));
        setButtonState(btnUpdateEmp, true, tgui::Color(47, 95, 138));
        setButtonState(btnDeleteEmp, true, tgui::Color(163, 45, 45));
    });

    contentPanel->add(panelEmp);
}

// 3. HÀM MAIN CHÍNH (ENTRY POINT & EVENT LOOP)
// Vai trò: Khởi tạo Application, nạp dữ liệu từ file txt, tạo cửa sổ và chạy vòng lặp sự kiện
int main() {
    // Khởi tạo đối tượng Application chứa toàn bộ các Manager (Product, Person, Table, Order)
    Application app;
    
    // Nạp toàn bộ dữ liệu từ các tệp tin .txt dưới thư mục data lên bộ nhớ RAM
    if (!app.initialize()) {
        cout << "Canh bao: Mot so file du lieu chua duoc tai thanh cong!" << endl;
    }

    // Khởi tạo cửa sổ đồ họa SFML với kích thước mặc định ban đầu (sẽ được định hình lại ở màn hình Login)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Coffee Shop Management - PBL2", sf::Style::Titlebar | sf::Style::Close);
    tgui::Gui gui(window); // Gắn giao diện TGUI vào cửa sổ SFML

    // Mở màn hình Đăng Nhập làm giao diện đầu tiên khi khởi chạy ứng dụng
    loadLoginScreen(gui, window, app);

    // Vòng lặp sự kiện chính (Main Event Loop) giúp ứng dụng chạy liên tục không bị tắt ngang
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            gui.handleEvent(event); // Chuyển các sự kiện bàn phím/chuột cho TGUI xử lý

            if (event.type == sf::Event::Closed) {
                app.saveData(); // Tự động lưu dữ liệu xuống file txt trước khi tắt ứng dụng
                window.close();
            }
        }

        window.clear(sf::Color(245, 240, 230)); // Xóa màn hình với màu nền be nhạt đặc trưng quán cafe
        gui.draw(); // Vẽ toàn bộ các thành phần giao diện lên khung hình
        window.display(); // Hiển thị khung hình hoàn chỉnh lên màn hình máy tính
    }

    return 0;
}