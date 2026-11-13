#include "ProductScreen.h"
#include <iostream>

using namespace std;

void loadMenuView(tgui::Panel::Ptr contentPanel, Application& app) {
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
    lblListTitle->getRenderer()->setTextColor(tgui::Color(111, 78, 55));
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

    // Khung chứa Ảnh minh họa
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
    // Đổi sang float để tinh chỉnh tọa độ chi tiết hơn. Kéo toàn bộ khối lên mốc 38%
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

    auto btnAdd = tgui::Button::create("THÊM MỚI");
    btnAdd->setSize("28%", 45);
    btnAdd->setPosition("5%", "88%");
    btnAdd->setTextSize(20);
    btnAdd->getRenderer()->setBackgroundColor(tgui::Color(40, 167, 69)); 
    btnAdd->getRenderer()->setTextColor(tgui::Color::White);
    rightPanel->add(btnAdd);

    auto btnUpdate = tgui::Button::create("CẬP NHẬT");
    btnUpdate->setSize("28%", 45);
    btnUpdate->setPosition("36%", "88%");
    btnUpdate->setTextSize(20);
    btnUpdate->getRenderer()->setBackgroundColor(tgui::Color(255, 193, 7)); 
    btnUpdate->getRenderer()->setTextColor(tgui::Color(50, 50, 50));
    rightPanel->add(btnUpdate);

    auto btnDelete = tgui::Button::create("XÓA MÓN");
    btnDelete->setSize("28%", 45);
    btnDelete->setPosition("67%", "88%");
    btnDelete->setTextSize(20);
    btnDelete->getRenderer()->setBackgroundColor(tgui::Color(220, 53, 69));
    btnDelete->getRenderer()->setTextColor(tgui::Color::White);
    rightPanel->add(btnDelete);

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
                    productImg->getRenderer()->setTexture("");
                }
            }
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
        if (p != nullptr){
            try {
                double newPrice = stod(editPrice->getText().toStdString());

                if (!p->changePrice(newPrice)){
                    cout << "[LỖI] Giá tiền không được là số âm!" << endl;
                    return;
                }

                bool isSelling = (comboStatus->getSelectedItemId() == "1");
                if (isSelling){
                    p->markAvailable();
                } else {
                    p->markUnavailable();
                }

                refreshListView();
                
                cout << "[THÀNH CÔNG] Đã cập nhật món: " << selectedID << endl;
            } catch (...) {
                cout << "[LỖI] Đơn giá nhập vào không hợp lệ (chỉ được nhập số)!" << endl;
            }
        }
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

            bool isAvailable = (comboStatus->getSelectedItemId() == "1");
            Product newProduct(id, name, price, cat, isAvailable);

            app.getProductManager().addProduct(newProduct);

            refreshListView();

            editID->setText("");
            editName->setText("");
            editCategory->setText("");
            editPrice->setText("");

            cout << "[THÀNH CÔNG] Đã thêm món mới: " << name << endl;
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
        } else {
            cout << "[LỖI] Không thể xóa! Có thể mã món không tồn tại." << endl;
        }
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
    };

    btnSearch->onPress(performSearch);
    searchBox->onReturnKeyPress(performSearch);
    filterCombo->onItemSelect(performSearch);
}