#include "OrderScreen.h"
#include <iostream>

using namespace std;

void loadOrderView(tgui::Panel::Ptr contentPanel, Application& app, tgui::Gui* gui) {
    contentPanel->removeAllWidgets();

    // =========================================================================
    // 1. KHU VỰC BÊN TRÁI: THỰC ĐƠN / TÌM KIẾM MÓN (60% chiều rộng)
    // =========================================================================
    auto leftPanel = tgui::Panel::create();
    leftPanel->setPosition("24", "24");
    leftPanel->setSize("60% - 36", "100% - 48");
    leftPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    leftPanel->getRenderer()->setBorderColor(tgui::Color(227, 217, 204));
    leftPanel->getRenderer()->setRoundedBorderRadius(10);
    contentPanel->add(leftPanel, "leftPanelOrder");

    auto lblMenuTitle = tgui::Label::create("THỰC ĐƠN");
    lblMenuTitle->setPosition("3%", "2.5%");
    lblMenuTitle->setTextSize(26);
    lblMenuTitle->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    lblMenuTitle->getRenderer()->setTextColor(tgui::Color(120, 60, 30));
    leftPanel->add(lblMenuTitle);

    // =========================================================================
    // 2. KHU VỰC BÊN PHẢI: CHI TIẾT ĐƠN HÀNG / GIỎ HÀNG (40% chiều rộng)
    // =========================================================================
    auto rightPanel = tgui::Panel::create();
    rightPanel->setPosition("60% + 12", "24");
    rightPanel->setSize("40% - 36", "100% - 48");
    rightPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    rightPanel->getRenderer()->setBorderColor(tgui::Color(227, 217, 204));
    rightPanel->getRenderer()->setRoundedBorderRadius(10);
    contentPanel->add(rightPanel, "rightPanelOrder");

    auto lblOrderTitle = tgui::Label::create("ĐƠN HÀNG");
    lblOrderTitle->setPosition("4.5%", "2.5%");
    lblOrderTitle->setTextSize(26);
    lblOrderTitle->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    lblOrderTitle->getRenderer()->setTextColor(tgui::Color(120, 60, 30));
    rightPanel->add(lblOrderTitle);

    // =========================================================================
    // THANH TÌM KIẾM & LỌC (Kế thừa khoảng cách chuẩn)
    // =========================================================================
    auto btnSearchMenu = tgui::Button::create("Tìm");
    btnSearchMenu->setPosition("97% - width", tgui::bindBottom(lblMenuTitle) + 16);
    btnSearchMenu->setSize("8%", 60);
    btnSearchMenu->setTextSize(20);
    btnSearchMenu->getRenderer()->setBackgroundColor(tgui::Color(160, 138, 120)); // Nâu cafe
    btnSearchMenu->getRenderer()->setTextColor(tgui::Color::White);
    btnSearchMenu->getRenderer()->setRoundedBorderRadius(8);
    btnSearchMenu->getRenderer()->setBorders(0);
    btnSearchMenu->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    leftPanel->add(btnSearchMenu, "btnSearchMenu");

    auto filterCategory = tgui::ComboBox::create();
    filterCategory->setPosition(tgui::bindLeft(btnSearchMenu) - "width" - "2%", tgui::bindTop(btnSearchMenu));
    filterCategory->setSize("20%", 60);
    filterCategory->setTextSize(20);
    filterCategory->getRenderer()->setRoundedBorderRadius(8);
    filterCategory->getRenderer()->setBorderColor(tgui::Color(207, 194, 179));
    filterCategory->addItem("Tất cả loại", "All");
    filterCategory->addItem("Cà phê", "Cà phê");
    filterCategory->addItem("Trà trái cây", "Trà trái cây");
    filterCategory->addItem("Bánh", "Bánh");
    filterCategory->setSelectedItemById("All");
    leftPanel->add(filterCategory, "filterMenuCategory");

    auto searchBox = tgui::EditBox::create();
    searchBox->setPosition("3%", tgui::bindTop(btnSearchMenu));
    searchBox->setSize(tgui::bindLeft(filterCategory) - "3%" - "2%", 60);
    searchBox->setTextSize(20);
    searchBox->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    searchBox->setDefaultText("Nhập tên món cần tìm...");
    searchBox->getRenderer()->setRoundedBorderRadius(8);
    searchBox->getRenderer()->setBorderColor(tgui::Color(207, 194, 179));
    leftPanel->add(searchBox, "searchMenuBox");

    // =========================================================================
    // KHU VỰC CUỘN DANH SÁCH MÓN (SCROLLABLE PANEL)
    // =========================================================================
    auto scrollPanel = tgui::ScrollablePanel::create();
    scrollPanel->setPosition("0%", tgui::bindBottom(searchBox) + 24);
    scrollPanel->setSize("100%", "100% - y - 10");
    scrollPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    scrollPanel->getRenderer()->setScrollbarWidth(8);
    leftPanel->add(scrollPanel, "scrollMenuPanel");

    // =========================================================================
    // HÀM TẠO THẺ MÓN ĂN (CHUẨN TỶ LỆ THEO BẢN THIẾT KẾ)
    // =========================================================================
    auto createProductCard = [scrollPanel](string id, string name, string priceStr, int index, bool isInCart, int quantity = 0) {
        // Tính toán vị trí: 2 cột (Cột 0 và Cột 1)
        int col = index % 2;
        int row = index / 2;
        
        auto card = tgui::Panel::create();
        card->setSize("46%", 96); // Chiều cao 96px để nội dung có độ thở chuẩn
        
        // Căn lề: Cột 1 ở 3%, Cột 2 ở 51%. Cách nhau dòng 16px.
        card->setPosition(tgui::String(to_string(3 + col * 48) + "%"), row * 112); 
        card->getRenderer()->setRoundedBorderRadius(10);
        card->getRenderer()->setBorders(1.5f);

        // Đổi màu nền & viền nếu món đang nằm trong giỏ hàng (State: In Cart)
        if (isInCart) {
            card->getRenderer()->setBackgroundColor(tgui::Color(245, 252, 248)); // Xanh nhạt
            card->getRenderer()->setBorderColor(tgui::Color(40, 130, 70)); // Viền xanh lá
        } else {
            card->getRenderer()->setBackgroundColor(tgui::Color::White);
            card->getRenderer()->setBorderColor(tgui::Color(210, 205, 200)); // Viền xám nhạt
        }

        // 1. Ô chứa hình ảnh (Image Placeholder)
        auto imgBox = tgui::Panel::create({"66", "66"});
        imgBox->setPosition("12", "15");
        imgBox->getRenderer()->setRoundedBorderRadius(8);
        imgBox->getRenderer()->setBorders(1);
        imgBox->getRenderer()->setBorderColor(tgui::Color(220, 215, 210));
        imgBox->getRenderer()->setBackgroundColor(tgui::Color(245, 242, 238));
        
        auto lblNoImg = tgui::Label::create("No Image");
        lblNoImg->setTextSize(10);
        lblNoImg->getRenderer()->setTextColor(tgui::Color(150, 140, 130));
        lblNoImg->setPosition("50% - width/2", "65%");
        imgBox->add(lblNoImg);
        card->add(imgBox);

        // 2. Tên món
        auto lblName = tgui::Label::create(name);
        lblName->setPosition("92", "20"); // Cách imgBox 14px (12 + 66 + 14 = 92)
        lblName->setTextSize(18);
        lblName->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
        lblName->getRenderer()->setTextColor(tgui::Color(70, 55, 45));
        card->add(lblName);

        // 3. Giá tiền
        auto lblPrice = tgui::Label::create(priceStr + " đ");
        lblPrice->setPosition("92", "52");
        lblPrice->setTextSize(18);
        lblPrice->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
        lblPrice->getRenderer()->setTextColor(tgui::Color(100, 80, 70));
        card->add(lblPrice);

        // 4. Cụm nút Thêm/Trừ (+ / -)
        auto btnAdd = tgui::Button::create("+");
        btnAdd->setSize("36", "36");
        btnAdd->setPosition("100% - 48", "30"); // Canh lề phải cách 12px (48 - 36 = 12)
        btnAdd->setTextSize(22);
        btnAdd->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
        btnAdd->getRenderer()->setRoundedBorderRadius(6);
        btnAdd->getRenderer()->setBackgroundColor(tgui::Color(31, 107, 58)); // Xanh lá đậm
        btnAdd->getRenderer()->setTextColor(tgui::Color::White);
        btnAdd->getRenderer()->setBorders(0);
        card->add(btnAdd);

        // Nếu món đã có trong giỏ, hiển thị thêm nút trừ (-) và số lượng
        if (isInCart) {
            auto lblQty = tgui::Label::create(to_string(quantity));
            lblQty->setSize("24", "36");
            lblQty->setPosition(tgui::bindLeft(btnAdd) - 24, "30");
            lblQty->setTextSize(18);
            lblQty->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
            lblQty->setVerticalAlignment(tgui::VerticalAlignment::Center);
            lblQty->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
            lblQty->getRenderer()->setTextColor(tgui::Color(70, 55, 45));
            card->add(lblQty);

            auto btnMinus = tgui::Button::create("-");
            btnMinus->setSize("36", "36");
            btnMinus->setPosition(tgui::bindLeft(lblQty) - 36, "30");
            btnMinus->setTextSize(22);
            btnMinus->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
            btnMinus->getRenderer()->setRoundedBorderRadius(6);
            btnMinus->getRenderer()->setBorders(1);
            btnMinus->getRenderer()->setBorderColor(tgui::Color(200, 195, 190));
            btnMinus->getRenderer()->setBackgroundColor(tgui::Color::White);
            btnMinus->getRenderer()->setTextColor(tgui::Color(70, 55, 45));
            card->add(btnMinus);
        }

        scrollPanel->add(card);
    };

    // --- HIỂN THỊ MẪU MỘT VÀI MÓN ĐỂ KIỂM TRA UI ---
    // (Sau này bạn sẽ thay bằng vòng lặp lấy từ app.getProductManager())
    createProductCard("SP01", "Cà Phê Đen", "20,000", 0, false);
    createProductCard("SP02", "Cà Phê Sữa", "20,000", 1, true, 2); // Đang nằm trong giỏ 2 phần
    createProductCard("SP03", "Espresso", "35,000", 2, false);
    createProductCard("SP04", "Americano", "39,000", 3, false);
    createProductCard("SP05", "Cappuccino", "40,000", 4, false);
    createProductCard("SP06", "Cà Phê Muối", "27,000", 5, false);
    createProductCard("SP07", "Bạc Xỉu", "27,000", 6, true, 1);    // Đang nằm trong giỏ 1 phần

    // =========================================================================
    // 3. HEADER GIỎ HÀNG (TRẠNG THÁI, BÀN, MÃ ĐƠN)
    // =========================================================================
    auto comboServingOrders = tgui::ComboBox::create();
    comboServingOrders->setPosition(tgui::bindRight(lblOrderTitle) + 16, tgui::bindTop(lblOrderTitle) - 8);
    comboServingOrders->setSize("45%", 36); 
    comboServingOrders->setTextSize(16);
    comboServingOrders->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    comboServingOrders->getRenderer()->setRoundedBorderRadius(8);
    comboServingOrders->getRenderer()->setBorderColor(tgui::Color(207, 194, 179));
    
    // Thêm các mục mẫu (Sau này sẽ load từ DataManager)
    comboServingOrders->addItem("Bàn 03 - HD026", "HD026");
    comboServingOrders->addItem("Bàn 05 - HD025", "HD025");
    comboServingOrders->addItem("+ Tạo đơn mới", "NEW");
    comboServingOrders->setSelectedItemById("NEW"); // Mặc định hiển thị Đơn Mới
    rightPanel->add(comboServingOrders, "comboServingOrders");

    // Nhãn Bàn và Mã đơn
    auto lblTable = tgui::Label::create("Bàn");
    lblTable->setPosition("5%", tgui::bindBottom(lblOrderTitle) + 20);
    lblTable->setTextSize(22);
    lblTable->getRenderer()->setTextColor(tgui::Color(120, 100, 90));
    rightPanel->add(lblTable);

    // ComboBox chọn Bàn
    auto comboTable = tgui::ComboBox::create();
    comboTable->setPosition("5%", tgui::bindBottom(lblTable) + 8);
    comboTable->setSize("44%", 66);
    comboTable->setTextSize(20);
    comboTable->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    comboTable->getRenderer()->setRoundedBorderRadius(8);
    comboTable->getRenderer()->setBorderColor(tgui::Color(207, 194, 179));
    comboTable->addItem("Chọn bàn", "");
    comboTable->setSelectedItemById("");
    rightPanel->add(comboTable, "comboTableOrder");

    // EditBox Mã đơn (Chỉ đọc - màu xám)
    auto editOrderID = tgui::EditBox::create();
    editOrderID->setPosition("95% - width", tgui::bindTop(comboTable));
    editOrderID->setSize("44%", 66);
    editOrderID->setTextSize(20);
    editOrderID->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    editOrderID->getRenderer()->setRoundedBorderRadius(8);
    editOrderID->getRenderer()->setBorderColor(tgui::Color(207, 194, 179));
    editOrderID->getRenderer()->setBackgroundColor(tgui::Color(245, 242, 238)); 
    editOrderID->setDefaultText("Hệ thống tự tạo");
    editOrderID->setReadOnly(true);
    rightPanel->add(editOrderID, "editOrderID");

    auto lblOrderID = tgui::Label::create("Mã đơn");
    lblOrderID->setPosition(tgui::bindLeft(editOrderID), tgui::bindTop(lblTable));
    lblOrderID->setTextSize(22);
    lblOrderID->getRenderer()->setTextColor(tgui::Color(120, 100, 90));
    rightPanel->add(lblOrderID);

    // =========================================================================
    // 4. DANH SÁCH MÓN TRONG GIỎ HÀNG (LISTVIEW)
    // =========================================================================
    // =========================================================================
    // DANH SÁCH MÓN ĐÃ CHỌN (CUSTOM SCROLLABLE PANEL)
    // =========================================================================
    auto lblCartTitle = tgui::Label::create("MÓN ĐÃ CHỌN");
    lblCartTitle->setPosition("5%", tgui::bindBottom(comboTable) + 24);
    lblCartTitle->setTextSize(22);
    lblCartTitle->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    lblCartTitle->getRenderer()->setTextColor(tgui::Color(100, 90, 80));
    rightPanel->add(lblCartTitle, "lblCartTitle");

    auto btnClearCart = tgui::Label::create("Xóa hết");
    btnClearCart->setPosition("95% - width", tgui::bindTop(lblCartTitle));
    btnClearCart->setTextSize(20);
    btnClearCart->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    btnClearCart->getRenderer()->setTextColor(tgui::Color(163, 45, 45)); 
    rightPanel->add(btnClearCart, "btnClearCart");

    auto cartScrollPanel = tgui::ScrollablePanel::create();
    cartScrollPanel->setPosition("5%", tgui::bindBottom(lblCartTitle) + 12);
    // Tính khoảng hở để nhường chỗ cho Ghi chú và Nút thanh toán ở dưới đáy
    cartScrollPanel->setSize("90%", "100% - y - 145"); 
    cartScrollPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    cartScrollPanel->getRenderer()->setBorderColor(tgui::Color(227, 217, 204));
    cartScrollPanel->getRenderer()->setBorders(1);
    cartScrollPanel->getRenderer()->setRoundedBorderRadius(8);
    cartScrollPanel->getRenderer()->setScrollbarWidth(6);
    cartScrollPanel->getRenderer()->setPadding({1, 6, 1, 6});
    rightPanel->add(cartScrollPanel, "cartScrollPanel");

    // --- HÀM TẠO 1 DÒNG MÓN ĂN TRONG GIỎ ---
    auto createCartItem = [cartScrollPanel](int index, string name, string unitPrice, int qty, string totalPrice) {
        auto itemRow = tgui::Panel::create({"100%", 65});
        itemRow->setPosition(0, index * 65); 
        itemRow->getRenderer()->setBorders(0);
        
        if (index % 2 != 0) {
            itemRow->getRenderer()->setBackgroundColor(tgui::Color(250, 246, 240)); 
        } else {
            itemRow->getRenderer()->setBackgroundColor(tgui::Color::White);
        }

        auto lblName = tgui::Label::create(name);
        lblName->setPosition("4%", 12);
        lblName->setTextSize(15);
        lblName->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
        lblName->getRenderer()->setTextColor(tgui::Color(40, 30, 20));
        itemRow->add(lblName);

        auto lblUnitPrice = tgui::Label::create(unitPrice + " đ");
        lblUnitPrice->setPosition("4%", 36);
        lblUnitPrice->setTextSize(13);
        lblUnitPrice->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
        lblUnitPrice->getRenderer()->setTextColor(tgui::Color(120, 110, 100));
        itemRow->add(lblUnitPrice);

        auto btnRemove = tgui::Button::create("x");
        btnRemove->setPosition("96% - width", 22);
        btnRemove->setSize(20, 20);
        btnRemove->setTextSize(14);
        btnRemove->getRenderer()->setBorders(0);
        btnRemove->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        btnRemove->getRenderer()->setTextColor(tgui::Color(100, 90, 80));
        btnRemove->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
        itemRow->add(btnRemove);

        auto lblTotal = tgui::Label::create(totalPrice);
        lblTotal->setPosition(tgui::bindLeft(btnRemove) - "width" - 12, 22);
        lblTotal->setTextSize(15);
        lblTotal->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
        lblTotal->getRenderer()->setTextColor(tgui::Color(40, 30, 20));
        itemRow->add(lblTotal);

        auto btnPlus = tgui::Button::create("+");
        btnPlus->setPosition(tgui::bindLeft(lblTotal) - 40 - 16, 16);
        btnPlus->setSize(30, 30);
        btnPlus->setTextSize(18);
        btnPlus->getRenderer()->setRoundedBorderRadius(6);
        btnPlus->getRenderer()->setBorders(0);
        btnPlus->getRenderer()->setBackgroundColor(tgui::Color(31, 107, 58)); 
        btnPlus->getRenderer()->setTextColor(tgui::Color::White);
        itemRow->add(btnPlus);

        auto lblQty = tgui::Label::create(to_string(qty));
        lblQty->setPosition(tgui::bindLeft(btnPlus) - 26, 20);
        lblQty->setSize(26, 30);
        lblQty->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        lblQty->setTextSize(15);
        lblQty->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
        lblQty->getRenderer()->setTextColor(tgui::Color(40, 30, 20));
        itemRow->add(lblQty);

        auto btnMinus = tgui::Button::create("-");
        btnMinus->setPosition(tgui::bindLeft(lblQty) - 30, 16);
        btnMinus->setSize(30, 30);
        btnMinus->setTextSize(18);
        btnMinus->getRenderer()->setRoundedBorderRadius(6);
        btnMinus->getRenderer()->setBorders(1);
        btnMinus->getRenderer()->setBorderColor(tgui::Color(210, 205, 195));
        btnMinus->getRenderer()->setBackgroundColor(tgui::Color::White);
        btnMinus->getRenderer()->setTextColor(tgui::Color(31, 107, 58));
        itemRow->add(btnMinus);

        cartScrollPanel->add(itemRow);
    };

    // Tạo mẫu vài dòng để test UI
    createCartItem(0, "Cà Phê Muối", "27,000", 1, "27,000");
    createCartItem(1, "Trà Chanh", "29,000", 2, "58,000");
    createCartItem(2, "Americano", "39,000", 1, "39,000");

    auto editNote = tgui::EditBox::create();
    editNote->setPosition("5%", tgui::bindBottom(cartScrollPanel) + 12);
    editNote->setSize("90%", 42);
    editNote->setTextSize(16);
    editNote->setDefaultText("Ghi chú cho đơn (tùy chọn)");
    editNote->getRenderer()->setFont("assets/fonts/BeVietnamPro-Regular.ttf");
    editNote->getRenderer()->setRoundedBorderRadius(8);
    editNote->getRenderer()->setBorderColor(tgui::Color(207, 194, 179));
    editNote->setEnabled(false);
    rightPanel->add(editNote, "editNote");

    // =========================================================================
    // 6. FOOTER: TỔNG TIỀN VÀ 3 NÚT TẠO / THANH TOÁN / HỦY
    // =========================================================================
    auto lblTotalText = tgui::Label::create("Tổng cộng (0 món, 0 phần)");
    lblTotalText->setPosition("5%", "100% - 90");
    lblTotalText->setTextSize(16);
    lblTotalText->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    lblTotalText->getRenderer()->setTextColor(tgui::Color(100, 80, 70));
    rightPanel->add(lblTotalText, "lblTotalText");

    auto lblTotalPrice = tgui::Label::create("0 đ");
    lblTotalPrice->setPosition("45%", "100% - 96");
    lblTotalPrice->setSize("50%", 30);
    lblTotalPrice->setHorizontalAlignment(tgui::HorizontalAlignment::Right);
    lblTotalPrice->setTextSize(26);
    lblTotalPrice->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
    lblTotalPrice->getRenderer()->setTextColor(tgui::Color(163, 45, 45)); // Đỏ đô
    rightPanel->add(lblTotalPrice, "lblTotalPrice");

    // Nút trạng thái tĩnh (Thay đổi màu tùy logic về sau)
    auto styleActionButton = [](tgui::Button::Ptr btn, tgui::Color activeColor) {
        btn->getRenderer()->setFont("assets/fonts/BeVietnamPro-Bold.ttf");
        btn->getRenderer()->setRoundedBorderRadius(8);
        btn->getRenderer()->setBorders(0);
        
        btn->getRenderer()->setBackgroundColor(activeColor);
        btn->getRenderer()->setTextColor(tgui::Color::White);
        
        btn->getRenderer()->setBackgroundColorDisabled(tgui::Color(210, 205, 195)); // Nền mờ
        btn->getRenderer()->setTextColorDisabled(tgui::Color(160, 155, 150));      // Chữ mờ
    };

    auto btnCreate = tgui::Button::create("TẠO ĐƠN");
    btnCreate->setPosition("5%", "100% - 55");
    btnCreate->setSize("28%", 45);
    btnCreate->setTextSize(18);
    styleActionButton(btnCreate, tgui::Color(40, 130, 70)); // Xanh lá
    btnCreate->setEnabled(false); // Chưa chọn món thì khóa
    rightPanel->add(btnCreate, "btnCreateOrder");

    auto btnPay = tgui::Button::create("THANH TOÁN");
    btnPay->setPosition("36%", "100% - 55");
    btnPay->setSize("38%", 45);
    btnPay->setTextSize(18);
    styleActionButton(btnPay, tgui::Color(160, 82, 45)); // Nâu
    btnPay->setEnabled(false); 
    rightPanel->add(btnPay, "btnPayOrder");

    auto btnCancel = tgui::Button::create("HỦY ĐƠN");
    btnCancel->setPosition("77%", "100% - 55");
    btnCancel->setSize("18%", 45);
    btnCancel->setTextSize(18);
    styleActionButton(btnCancel, tgui::Color(204, 51, 51)); // Đỏ
    btnCancel->setEnabled(false);
    rightPanel->add(btnCancel, "btnCancelOrder");
}