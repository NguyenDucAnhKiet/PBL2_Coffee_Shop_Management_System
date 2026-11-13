#include "PersonScreen.h"
#include <iostream>

using namespace std;

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
    //lblLeftTitle->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
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
    //lblRightTitle->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
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
                
                auto lblArrow = tgui::Label::create(tgui::String(u8"→"));
                lblArrow->getRenderer()->setFont("assets/fonts/Roboto-Regular.ttf");
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