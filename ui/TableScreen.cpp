#include "TableScreen.h"
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

string statusToText(TableStatus status) {
    switch (status) {
    case TableStatus::EMPTY:    return "Trống";
    case TableStatus::OCCUPIED: return "Đang sử dụng";
    case TableStatus::RESERVED: return "Đã đặt trước";
    default:                    return "Không xác định";
    }
}

TableStatus idToStatus(const string& id) {
    if (id == "OCCUPIED") return TableStatus::OCCUPIED;
    if (id == "RESERVED") return TableStatus::RESERVED;
    return TableStatus::EMPTY;
}

string trimLower(const string& s) {
    size_t l = 0, r = s.size();
    while (l < r && isspace(static_cast<unsigned char>(s[l]))) ++l;
    while (r > l && isspace(static_cast<unsigned char>(s[r - 1]))) --r;
    string out = s.substr(l, r - l);
    for (char& c : out) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return out;
}

bool parsePositiveInt(const string& s, int& outVal) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(static_cast<unsigned char>(c))) return false;
    try {
        long long n = stoll(s);
        if (n <= 0 || n > 500) return false;
        outVal = static_cast<int>(n);
        return true;
    } catch (...) {
        return false;
    }
}

void styleCardPanel(tgui::Panel::Ptr panel) {
    auto r = panel->getSharedRenderer();
    r->setBackgroundColor(tgui::Color::White);
    r->setBorders(1);
    r->setBorderColor(tgui::Color(200, 200, 200));
    r->setRoundedBorderRadius(10);
}

void styleField(tgui::EditBox::Ptr edt, const string& placeholder, const tgui::Font& fontReg) {
    edt->setSize("88%", 46);
    edt->setTextSize(20);
    edt->setDefaultText(placeholder);
    auto r = edt->getSharedRenderer();
    r->setFont(fontReg);
    r->setRoundedBorderRadius(6);
    r->setBorders(1);
    r->setBorderColor(tgui::Color(190, 180, 170));
}

} // namespace

void loadTableView(tgui::Panel::Ptr contentPanel, Application& app) {
    contentPanel->removeAllWidgets();
    auto& manager = app.getTableManager();

    const tgui::Color coffeeTitle(111, 78, 55);
    const tgui::Color labelBrown(90, 45, 15);
    tgui::Font fontReg("assets/fonts/BeVietnamPro-Regular.ttf");
    tgui::Font fontBold("assets/fonts/BeVietnamPro-Bold.ttf");

    // --- Cột trái: Danh sách bàn & thống kê ---
    auto leftPanel = tgui::Panel::create({"62%", "90%"});
    leftPanel->setPosition("2%", "5%");
    styleCardPanel(leftPanel);

    auto lblLeftTitle = tgui::Label::create("DANH SÁCH BÀN");
    lblLeftTitle->setPosition("5%", "4%");
    lblLeftTitle->setTextSize(24);
    lblLeftTitle->getSharedRenderer()->setFont(fontBold);
    lblLeftTitle->getSharedRenderer()->setTextColor(coffeeTitle);
    leftPanel->add(lblLeftTitle);

    auto edtSearch = tgui::EditBox::create();
    edtSearch->setPosition("5%", "11%");
    edtSearch->setSize("44%", 45);
    edtSearch->setTextSize(20);
    edtSearch->setDefaultText("Nhập mã bàn cần tìm...");
    edtSearch->getSharedRenderer()->setFont(fontReg);
    edtSearch->getSharedRenderer()->setRoundedBorderRadius(6);
    edtSearch->getSharedRenderer()->setBorders(1);
    edtSearch->getSharedRenderer()->setBorderColor(tgui::Color(190, 180, 170));
    leftPanel->add(edtSearch);

    auto cbFilter = tgui::ComboBox::create();
    cbFilter->setPosition("51%", "11%");
    cbFilter->setSize("29%", 45);
    cbFilter->setTextSize(20);
    cbFilter->getSharedRenderer()->setFont(fontReg);
    cbFilter->getSharedRenderer()->setRoundedBorderRadius(6);
    cbFilter->getSharedRenderer()->setBorders(1);
    cbFilter->getSharedRenderer()->setBorderColor(tgui::Color(190, 180, 170));
    cbFilter->addItem("Tất cả", "ALL");
    cbFilter->addItem("Trống", "EMPTY");
    cbFilter->addItem("Đang sử dụng", "OCCUPIED");
    cbFilter->addItem("Đã đặt trước", "RESERVED");
    cbFilter->setSelectedItemById("ALL");
    leftPanel->add(cbFilter);

    auto btnSearch = tgui::Button::create("Tìm");
    btnSearch->setPosition("82%", "11%");
    btnSearch->setSize("13%", 45);
    btnSearch->setTextSize(20);
    btnSearch->getSharedRenderer()->setFont(fontBold);
    btnSearch->getSharedRenderer()->setRoundedBorderRadius(6);
    btnSearch->getSharedRenderer()->setBackgroundColor(tgui::Color(160, 140, 120));
    btnSearch->getSharedRenderer()->setTextColor(tgui::Color::White);
    leftPanel->add(btnSearch);

    auto listView = tgui::ListView::create();
    listView->setPosition("5%", "20%");
    listView->setSize("90%", "66%");
    listView->addColumn("Mã Bàn", 180);
    listView->addColumn("Sức Chứa", 180);
    listView->addColumn("Trạng Thái", 220);
    listView->setColumnExpanded(2, true);
    listView->setTextSize(20);
    listView->setHeaderTextSize(21);
    listView->setHeaderHeight(48);
    listView->setItemHeight(42);
    listView->getSharedRenderer()->setFont(fontReg);
    listView->getSharedRenderer()->setBorders(1);
    listView->getSharedRenderer()->setBorderColor(tgui::Color(200, 200, 200));
    leftPanel->add(listView);

    auto lblStats = tgui::Label::create();
    lblStats->setPosition("5%", "89%");
    lblStats->setTextSize(20);
    lblStats->getSharedRenderer()->setFont(fontBold);
    lblStats->getSharedRenderer()->setTextColor(labelBrown);
    leftPanel->add(lblStats);

    // --- Cột phải: Thông tin chi tiết & thao tác ---
    auto rightPanel = tgui::Panel::create({"32%", "90%"});
    rightPanel->setPosition("66%", "5%");
    styleCardPanel(rightPanel);

    auto lblRightTitle = tgui::Label::create("THÔNG TIN CHI TIẾT");
    lblRightTitle->setPosition("6%", "4%");
    lblRightTitle->setTextSize(24);
    lblRightTitle->getSharedRenderer()->setFont(fontBold);
    lblRightTitle->getSharedRenderer()->setTextColor(coffeeTitle);
    rightPanel->add(lblRightTitle);

    auto makeLbl = [&](const string& text, const char* y) {
        auto lbl = tgui::Label::create(text);
        lbl->setPosition("6%", y);
        lbl->setTextSize(20);
        lbl->getSharedRenderer()->setFont(fontReg);
        lbl->getSharedRenderer()->setTextColor(labelBrown);
        return lbl;
    };

    rightPanel->add(makeLbl("Mã bàn:", "16%"));
    auto edtId = tgui::EditBox::create();
    edtId->setPosition("6%", "22%");
    styleField(edtId, "Ví dụ: B01, VIP01", fontReg);
    rightPanel->add(edtId);

    rightPanel->add(makeLbl("Sức chứa (người):", "33%"));
    auto edtCap = tgui::EditBox::create();
    edtCap->setPosition("6%", "39%");
    styleField(edtCap, "Ví dụ: 4", fontReg);
    edtCap->setInputValidator("[0-9]*");
    rightPanel->add(edtCap);

    rightPanel->add(makeLbl("Trạng thái:", "50%"));
    auto cbStatus = tgui::ComboBox::create();
    cbStatus->setPosition("6%", "56%");
    cbStatus->setSize("88%", 46);
    cbStatus->setTextSize(20);
    cbStatus->getSharedRenderer()->setFont(fontReg);
    cbStatus->getSharedRenderer()->setRoundedBorderRadius(6);
    cbStatus->getSharedRenderer()->setBorders(1);
    cbStatus->getSharedRenderer()->setBorderColor(tgui::Color(190, 180, 170));
    cbStatus->addItem("Trống", "EMPTY");
    cbStatus->addItem("Đang sử dụng", "OCCUPIED");
    cbStatus->addItem("Đã đặt trước", "RESERVED");
    cbStatus->setSelectedItemById("EMPTY");
    rightPanel->add(cbStatus);

    auto lblMsg = tgui::Label::create("");
    lblMsg->setPosition("6%", "66%");
    lblMsg->setSize("88%", "8%");
    lblMsg->setTextSize(18);
    lblMsg->getSharedRenderer()->setFont(fontBold);
    rightPanel->add(lblMsg);

    auto setMsg = [lblMsg](const string& text, bool isError) {
        lblMsg->setText(text);
        lblMsg->getSharedRenderer()->setTextColor(
            isError ? tgui::Color(220, 53, 69) : tgui::Color(40, 167, 69)
        );
    };

    auto makeActionBtn = [&](const string& text, const char* x, const char* w, tgui::Color bg, tgui::Color fg) {
        auto btn = tgui::Button::create(text);
        btn->setPosition(x, "77%");
        btn->setSize(w, "46");
        btn->setTextSize(19);
        btn->getSharedRenderer()->setFont(fontBold);
        btn->getSharedRenderer()->setRoundedBorderRadius(6);
        btn->getSharedRenderer()->setBackgroundColor(bg);
        btn->getSharedRenderer()->setTextColor(fg);
        return btn;
    };

    auto btnAdd = makeActionBtn("THÊM MỚI", "6%", "27%", tgui::Color(40, 167, 69), tgui::Color::White);
    auto btnUpdate = makeActionBtn("CẬP NHẬT", "36%", "29%", tgui::Color(255, 193, 7), tgui::Color(50, 50, 50));
    auto btnDelete = makeActionBtn("XÓA BÀN", "68%", "26%", tgui::Color(220, 53, 69), tgui::Color::White);
    rightPanel->add(btnAdd);
    rightPanel->add(btnUpdate);
    rightPanel->add(btnDelete);

    auto btnClear = tgui::Button::create("Làm mới biểu mẫu");
    btnClear->setPosition("6%", "88%");
    btnClear->setSize("88%", "42");
    btnClear->setTextSize(19);
    btnClear->getSharedRenderer()->setFont(fontReg);
    btnClear->getSharedRenderer()->setRoundedBorderRadius(6);
    btnClear->getSharedRenderer()->setBackgroundColor(tgui::Color(240, 235, 230));
    btnClear->getSharedRenderer()->setTextColor(coffeeTitle);
    rightPanel->add(btnClear);

    // --- Làm mới danh sách & thống kê ---
    auto refresh = [=, &manager]() {
        listView->removeAllItems();
        string kw = trimLower(edtSearch->getText().toStdString());
        string filterId = cbFilter->getSelectedItemId().toStdString();

        for (Table* t : manager.getAllTables()) {
            if (!t) continue;
            if (!kw.empty() && trimLower(t->getTableID()).find(kw) == string::npos) continue;
            if (filterId != "ALL" && t->getStatus() != idToStatus(filterId)) continue;

            listView->addItem(vector<tgui::String>{
                tgui::String(t->getTableID()),
                tgui::String(to_string(t->getCapacity()) + " người"),
                tgui::String(statusToText(t->getStatus()))
            });
        }

        ostringstream oss;
        oss << "Tổng: " << manager.getTotalTableCount()
            << "  |  Trống: " << manager.getAvailableTableCount()
            << "  |  Đang sử dụng: " << manager.getOccupiedTableCount()
            << "  |  Đã đặt trước: " << manager.getReservedTables().size();
        lblStats->setText(oss.str());
    };

    auto clearForm = [=]() {
        edtId->setReadOnly(false);
        edtId->getSharedRenderer()->setBackgroundColor(tgui::Color::White);
        edtId->setText("");
        edtCap->setText("");
        cbStatus->setSelectedItemById("EMPTY");
        listView->deselectItems();
    };

    refresh();

    btnSearch->onPress(refresh);
    edtSearch->onReturnKeyPress(refresh);
    cbFilter->onItemSelect(refresh);

    listView->onItemSelect([=, &manager](int row) {
        if (row < 0) return;
        string id = listView->getItemCell(row, 0).toStdString();
        Table* t = manager.findTableByID(id);
        if (!t) return;

        edtId->setText(t->getTableID());
        edtId->setReadOnly(true);
        edtId->getSharedRenderer()->setBackgroundColor(tgui::Color(242, 236, 230));
        edtCap->setText(to_string(t->getCapacity()));
        cbStatus->setSelectedItemById(
            t->getStatus() == TableStatus::OCCUPIED ? "OCCUPIED" :
            t->getStatus() == TableStatus::RESERVED ? "RESERVED" : "EMPTY"
        );
        setMsg("Đang chọn bàn " + t->getTableID() + ".", false);
    });

    btnAdd->onPress([=, &manager]() {
        string id = edtId->getText().toStdString();
        while (!id.empty() && isspace(static_cast<unsigned char>(id.front()))) id.erase(id.begin());
        while (!id.empty() && isspace(static_cast<unsigned char>(id.back()))) id.pop_back();

        int cap = 0;
        if (id.empty()) { setMsg("Vui lòng nhập mã bàn.", true); return; }
        if (!parsePositiveInt(edtCap->getText().toStdString(), cap)) {
            setMsg("Sức chứa phải là số nguyên dương (1 - 500).", true);
            return;
        }
        if (manager.findTableByID(id) != nullptr) {
            setMsg("Mã bàn '" + id + "' đã tồn tại.", true);
            return;
        }

        Table t(id, idToStatus(cbStatus->getSelectedItemId().toStdString()), cap);
        if (!manager.addTable(t)) { setMsg("Thêm bàn thất bại.", true); return; }

        refresh();
        clearForm();
        setMsg("Đã thêm bàn '" + id + "'.", false);
    });

    btnUpdate->onPress([=, &manager]() {
        string id = edtId->getText().toStdString();
        while (!id.empty() && isspace(static_cast<unsigned char>(id.front()))) id.erase(id.begin());
        while (!id.empty() && isspace(static_cast<unsigned char>(id.back()))) id.pop_back();

        int cap = 0;
        if (id.empty() || !manager.findTableByID(id)) {
            setMsg("Chọn hoặc nhập mã bàn hợp lệ để cập nhật.", true);
            return;
        }
        if (!parsePositiveInt(edtCap->getText().toStdString(), cap)) {
            setMsg("Sức chứa phải là số nguyên dương (1 - 500).", true);
            return;
        }

        bool okCap = manager.updateTableCapacity(id, cap);
        bool okStatus = manager.updateTableStatus(id, idToStatus(cbStatus->getSelectedItemId().toStdString()));
        if (!okCap || !okStatus) { setMsg("Cập nhật bàn thất bại.", true); return; }

        refresh();
        setMsg("Đã cập nhật bàn '" + id + "'.", false);
    });

    btnDelete->onPress([=, &manager]() {
        string id = edtId->getText().toStdString();
        while (!id.empty() && isspace(static_cast<unsigned char>(id.front()))) id.erase(id.begin());
        while (!id.empty() && isspace(static_cast<unsigned char>(id.back()))) id.pop_back();

        if (id.empty() || !manager.findTableByID(id)) {
            setMsg("Vui lòng chọn bàn cần xóa.", true);
            return;
        }
        if (!manager.removeTable(id, true)) {
            setMsg("Xóa bàn thất bại.", true);
            return;
        }

        refresh();
        clearForm();
        setMsg("Đã xóa bàn '" + id + "'.", false);
    });

    btnClear->onPress([=]() {
        clearForm();
        setMsg("Đã làm mới biểu mẫu.", false);
    });

    contentPanel->add(leftPanel);
    contentPanel->add(rightPanel);
}