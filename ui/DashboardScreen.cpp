#include "DashboardScreen.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

extern void loadLoginScreen(tgui::Gui& gui, sf::RenderWindow& window, Application& app);
extern void loadMenuView(tgui::Panel::Ptr contentPanel, Application& app);

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
        for (size_t i = 0; i < menuButtons->size(); ++i) {
            if (i == index) {
                (*menuButtons)[i]->getRenderer()->setBackgroundColor(tgui::Color(110, 90, 70));
            } else {
                (*menuButtons)[i]->getRenderer()->setBackgroundColor(tgui::Color(160, 140, 120));
            }
        }

        contentPanel->removeAllWidgets();

        if (index == 0) {
            loadHomeView(contentPanel, app);
        } else if (index == 1) {
            loadOrderView(contentPanel, app, pGui);
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

        // Gắn sự kiện click để gọi hàm chuyển Tab tương ứng
        btn->onPress([switchTab, i]() {
            switchTab(i);
        });
    }

    // --- Nút Đăng Xuất (Nằm ở cuối cùng bên phải thanh menu) ---
    auto btnLogout = tgui::Button::create("ĐĂNG XUẤT");
    btnLogout->setTextSize(25); //Thay đổi cỡ chữ nút Đăng xuất
    btnLogout->setSize({tgui::String(to_string(buttonWidth) + "%"), "100%"});
    btnLogout->setPosition({tgui::String(to_string(menuItems.size() * buttonWidth) + "%"), "0%"});
    btnLogout->getRenderer()->setBorders(0);
    btnLogout->getRenderer()->setBackgroundColor(tgui::Color(160, 140, 120)); 
    btnLogout->getRenderer()->setTextColor(tgui::Color(255, 120, 120)); // Chữ màu đỏ nhạt cảnh báo
    menuBar->add(btnLogout);

    // Sự kiện Đăng xuất: Xóa phiên làm việc hiện tại và quay về màn hình Login (thu nhỏ)
    btnLogout->onPress([&gui, &window, &app]() {
        app.logout(); 
        loadLoginScreen(gui, window, app); 
    });

    // MẶC ĐỊNH: Khi vừa đăng nhập thành công, tự động load tab đầu tiên (TRANG CHỦ - Index 0)
    switchTab(0);
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

    // Thêm 4 thẻ vào contentPanel (Các chỉ số hiện đang là dữ liệu giả lập - Hardcode)
    // Sau này bạn sẽ gọi app.getDailyRevenue(), app.getDailyOrders()... để truyền vào
    contentPanel->add(createStatCard("Số đơn hàng trong ngày", to_string(todayOrders) + " Đơn", "3%", "5%", tgui::Color(50, 50, 50)));
    contentPanel->add(createStatCard("Doanh thu ngày", formatCurrency(todayRevenue), "27.5%", "5%", tgui::Color(30, 150, 50)));
    contentPanel->add(createStatCard("Số đơn đang xử lý", "3 Đơn", "52%", "5%", tgui::Color(200, 120, 0)));
    contentPanel->add(createStatCard("Số đơn huỷ", "1 Đơn", "76.5%", "5%", tgui::Color(200, 50, 50)));

    // -----------------------------------------------------
    // 2. KHUNG TRÁI: THỐNG KÊ TOP SẢN PHẨM BÁN CHẠY
    // -----------------------------------------------------
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

    // -----------------------------------------------------
    // 3. KHUNG PHẢI: ĐƠN HÀNG GẦN ĐÂY (Sử dụng ListView để có thanh cuộn)
    // -----------------------------------------------------
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

    // Bảng danh sách đơn hàng (TGUI ListView tự động tạo thanh cuộn nếu vượt quá kích thước)
    auto listView = tgui::ListView::create();
    listView->setSize("90%", "80%");
    listView->setPosition("5%", "15%");
    listView->setTextSize(20);
    listView->setItemHeight(40);
    listView->setHeaderHeight(40);
    
    // Thêm các cột theo bản thiết kế
    listView->addColumn("Mã Order", 120);
    listView->addColumn("Mã NV", 120);
    listView->addColumn("Nhân Viên", 275);
    listView->addColumn("Thời Gian", 205);
    listView->addColumn("Tổng Tiền", 150);
    listView->addColumn("Trạng Thái", 180);
    listView->setColumnExpanded(6, true); //Cột cuối tự động khớp
    
    //Lấy dữ liệu 20 hoá đơn gần đây để hiển thị ở bảng
    vector<Order*> recentOrders = app.getOrderManager().getRecentOrders(20);

    for (Order* order : recentOrders){
        Person* staff = app.getPersonManager().findPersonByID(order->getEmployeeID());
        string staffName = (staff != nullptr) ? staff->getFullName() : "Không xác định";

        //Hiện tại chỉ mới làm các đơn đã PAID nên tạm để là Hoàn thành
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