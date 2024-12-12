//
// Created by stupid_coder_jyy on 24-11-8.
//

#include "Calendar.h"
#include "AsyncTask.h"
#include <RcManagers.h>

USING_NAMESPACE(lr)

CalendarData::CalendarData(): viewType(ViewType::D3), prevType(), dateStart(QDate::currentDate()) {
}

void CalendarData::set(ViewType vt, QDate ds) {
    if (vt == ViewType::Month && viewType != ViewType::Month) {
        prevType = viewType;
    }
    viewType = vt;
    dateStart = ds;
    emit sigDataChanged();
}

WidgetData * CalendarData::getData(const QDate &date) const {
    return nullptr;
}

void CalendarData::setTopLeft(calendar::ListDataCalendar *ldc) const {
    if (viewType == calendar::Month) {
        QDate d = dateStart;
        d.setDate(d.year(), d.month(), 1);
        ldc->topLeft = d.addDays(1 - d.dayOfWeek());
    } else {
        ldc->topLeft = dateStart;
    }
}

void CalendarData::gatherData(calendar::ListDataCalendar* ldc) const {
    int len = viewType == calendar::Month ? 42 : viewType;
    for (int i = 0; i < len; i++) {
        ldc->append(getData(ldc->topLeft.addDays(i)));
    }
}

void Calendar::mainInit() {
    auto f = WidgetFactoryStorage::get("test:widget_mini_calendar");
    regClazz(f, Widget);
    regClazz(f, Button);
    regClazz(f, ArrowButton);
    f = WidgetFactoryStorage::get("test:calendar/widget_calendar");
    regClazz(f, Widget);
    regClazz(f, calendar::MiniCalendarDropDown);
    regClazz(f, DropDown);
    regClazz(f, Calendar);
    regClazz(f, ArrowButton);
    regClazz(f, Button);
    regClazz(f, calendar::ButtonSwitchView);
    regClazz(f, calendar::SlotsWidgetCalendar);
    f = WidgetFactoryStorage::get("test:calendar/dropdown_range");
    regClazz(f, Widget);
    regClazz(f, calendar::DropDownRange);
    regClazz(f, Label);
    regClazz(f, ArrowButton);
    regClazz(f, ListWidget);
    regClazz(f, calendar::ItemRange);
    f = WidgetFactoryStorage::get("test:calendar/dropdown_mini_calendar");
    regClazz(f, Widget);
    regClazz(f, calendar::MiniCalendarDropDown);
    regClazz(f, DropDown);
    regClazz(f, Label);
}

Calendar::Calendar(QWidget *parent, bool iic): Widget(parent, iic), dropdownMiniCalendar(), dropdownRange(),
        btnPrev(), btnNext(), btnWeek(), btnMonth(), miniCalendar(), labelRange(), labelDate(), slotsContent() {
}

void Calendar::setData(WidgetData *d) {
    if (auto cd = dynamic_cast<CalendarData*>(d)) {
        Widget::setData(d);
        miniCalendar->setData(cd);
        dropdownRange->setData(cd);
    }
}

void Calendar::setItemBuilder(const Identifier &factoryLoc) {
    itemBuilder = [factoryLoc] {
        return WidgetFactoryStorage::get(factoryLoc)->applyAndCast<ListItemCalendar>();
    };
}

void Calendar::setItemBuilder(const std::function<ListItemCalendar*()>& ib) {
    itemBuilder = ib;
}

void Calendar::syncDataToWidget() {
    if (auto* cd = wData->cast<CalendarData>()) {
        miniCalendar->syncDataToWidget();
        labelRange->setText(QString::number(cd->getViewType()) + "天");
        labelRange->setSizeToText();
        QString s;
        switch (cd->getViewType()) {
            case calendar::Month: {
                s = cd->getDateStart().toString("yyyy年MM月");
                break;
            }
            case calendar::D1: {
                s = cd->getDateStart().toString("yyyy年 MM月dd日");
                break;
            }
            default: {
                auto d = cd->getDateStart(), d1 = d.addDays(cd->getViewType() - 1);
                if (d.year() == d1.year()) {
                    s = d.toString("yyyy年 MM月dd日");
                    s += " ~ ";
                    s += d1.toString("MM月dd日");
                } else {
                    s = d.toString("yyyy年MM月dd日");
                    s += " ~ ";
                    s += d1.toString("yyyy年MM月dd日");
                }
                break;
            }
        }
        labelDate->setText(s);
        labelDate->setSizeToText();
        bool isMonth = cd->getViewType() == calendar::Month;
        dropdownRange->setVisible(!isMonth);
        btnWeek->setSelected(!isMonth);
        btnMonth->setSelected(isMonth);
        if (isMonth) {
            slotsContent->setSlotCount(7, 6);
        } else {
            slotsContent->setSlotCount(cd->getViewType(), 1);
        }
        slotsContent->cd = cd;
        refreshContent(cd);
    }
}

void Calendar::connectModelView() {
    auto* cd = wData->cast<CalendarData>();
    dc << connect(cd, &WidgetData::sigDataChanged, this, &Calendar::syncDataToWidget);
    dc << connect(miniCalendar, &MiniCalendar::sigRelease, this, [this, cd] {
        switch (miniCalendar->getViewLevel()) {
            case mini_calendar::Day: {
                miniCalendar->syncWidgetToData();
                emit dropdownMiniCalendar->sigCloseMenu();
                break;
            }
            case mini_calendar::Month: {
                if (cd->getViewType() == calendar::Month) {
                    miniCalendar->syncWidgetToData();
                    emit dropdownMiniCalendar->sigCloseMenu();
                }
                break;
            }
            default:
                break;
        }
    });
    dc << connect(dropdownMiniCalendar, &DropDown::sigMenuOpening, this, [this, cd] {
        if (cd->getViewType() == calendar::Month) {
            miniCalendar->setViewLevel(mini_calendar::Month);
        } else {
            miniCalendar->setViewLevel(mini_calendar::Day);
        }
        miniCalendar->loadDate(cd->getDateStart());
    });
    dc << connect(btnPrev, &Button::sigSelected, this, [cd] {
        if (cd->getViewType() == calendar::Month) {
            cd->set(cd->getViewType(), cd->getDateStart().addMonths(-1));
        } else {
            cd->set(cd->getViewType(), cd->getDateStart().addDays(-cd->getViewType()));
        }
    });
    dc << connect(btnNext, &Button::sigSelected, this, [cd] {
        if (cd->getViewType() == calendar::Month) {
            cd->set(cd->getViewType(), cd->getDateStart().addMonths(1));
        } else {
            cd->set(cd->getViewType(), cd->getDateStart().addDays(cd->getViewType()));
        }
    });
    dc << connect(btnWeek, &Button::sigSelected, this, [cd](bool s) {
        if (s) {
            cd->set(cd->getPrevType(), cd->getDateStart());
        }
    });
    dc << connect(btnMonth, &Button::sigSelected, this, [cd](bool s) {
        if (s) {
            cd->set(calendar::Month, cd->getDateStart());
        }
    });
}

void Calendar::refreshContent(CalendarData* cd) const {
    auto ldc = slotsContent->widgetData()->cast<calendar::ListDataCalendar>();
    DELETE_LIST(ldc->getData())
    ldc->clear();
    cd->setTopLeft(ldc);
    slotsContent->syncItems();
    auto getter = AsyncTask::create();
    getter->setTask([cd, ldc] {
        cd->gatherData(ldc);
    });
    getter->setFinished(this, [this] {
        slotsContent->widgetData()->cast<ListData>()->markAll();
        setOperationEnabled(true);
    });
    getter->start();
    setOperationEnabled(false);
}

void Calendar::setOperationEnabled(bool e) const {
    btnPrev->setWidgetEnabled(e);
    btnNext->setWidgetEnabled(e);
    btnWeek->setWidgetEnabled(e);
    btnMonth->setWidgetEnabled(e);
    dropdownRange->setWidgetEnabled(e);
    dropdownMiniCalendar->setWidgetEnabled(e);
}

void Calendar::initWidget() {
    if (!prepared) {
        WidgetFactoryStorage::get("test:calendar/widget_calendar")->apply(nullptr, this);
        dropdownMiniCalendar = getPointer<DropDown>("dropdown_mini_calendar");
        labelDate = dropdownMiniCalendar->getPointer<Label>("l");
        miniCalendar = dropdownMiniCalendar->getPointer<calendar::MiniCalendarDropDown>("c");
        dropdownRange = getPointer<calendar::DropDownRange>("dropdown_range");
        labelRange = dropdownRange->getPointer<Label>("l");
        btnPrev = getPointer<Button>("btnPrev");
        btnNext = getPointer<Button>("btnNext");
        btnWeek = getPointer<Button>("btnWeek");
        btnMonth = getPointer<Button>("btnMonth");
        btnWeek->setSelected(true);
        slotsContent = getPointer<calendar::SlotsWidgetCalendar>("slots");
        if (itemBuilder != nullptr) {
            slotsContent->setItemBuilder(itemBuilder);
        }
        slotsContent->setData(new calendar::ListDataCalendar);
        prepared = true;
    }
}

ListItemCalendar::ListItemCalendar(QWidget *parent, bool iic): ListItem(parent, iic), sw(), itemIdx(), iconNum() {
}

void ListItemCalendar::syncDataToWidget() {
    ListItem::syncDataToWidget();
    if (!sw->cd) {
        return;
    }
    auto topLeft = listData->cast<calendar::ListDataCalendar>()->topLeft;
    auto d = topLeft.addDays(itemIdx);
    if (sw->cd->getViewType() == calendar::Month) {
        int m = sw->cd->getDateStart().month();
        if (d.month() == m) {
            iconColor = Styles::GRAY_TEXT_1->color;
        } else {
            iconColor = Styles::GRAY_4->color;
        }
        if (d == QDate::currentDate()) {
            iconColor = Styles::BLUE_1->color;
            iconFont = FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_MAIN).setBoldWeight().get();
        } else {
            iconFont = Styles::FONT_MAIN;
        }
    } else {
        int dd = d.daysTo(QDate::currentDate());
        if (dd < 0) {
            iconFont = Styles::FONT_MAIN;
            iconColor = Styles::GRAY_TEXT_1->color;
        } else if (dd > 0) {
            iconFont = Styles::FONT_MAIN;
            iconColor = Styles::GRAY_4->color;
        } else {
            iconFont = FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_MAIN).setBoldWeight().get();
            iconColor = Styles::BLUE_1->color;
        }
    }
    iconNum = d.day();
    update();
}

void ListItemCalendar::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setFont(iconFont);
    p.setPen(iconColor);
    auto str = QString::number(iconNum);
    auto strSize = Label::baseTextSize(str, p.font());
    p.drawText(QRect(0, 0, strSize.width(), strSize.height()), str, {Qt::AlignCenter});
}

USING_NAMESPACE(lr::calendar)

MiniCalendarDropDown::MiniCalendarDropDown(QWidget *parent, bool iic): MiniCalendar(parent, iic),
                                                                       layerDay(), layerMonth(), viewType(), isViewTypeMonth() {
}

void MiniCalendarDropDown::syncDataToWidget() {
    if (auto md = wData->cast<CalendarData>()) {
        dateStart = md->getDateStart();
        viewType = md->getViewType();
        isViewTypeMonth = viewType == Month;
        if (viewType == Month) {
            setMaxViewLevel(ViewLevel::Month);
        } else {
            setMaxViewLevel(ViewLevel::Day);
        }
    }
}

void MiniCalendarDropDown::syncWidgetToData() {
    if (auto md = wData->cast<CalendarData>()) {
        md->set(viewType, dateStart);
    }
}

void MiniCalendarDropDown::initWidget() {
    MiniCalendar::initWidget();
    auto p = painters[ViewLevel::Day];
    layerDay = new LayerDay;
    p->addLayer(layerDay);
    p->setMouseTracking(true);
    connect(this, &MiniCalendar::sigPress, [this](const QDate& d) {
        if (viewLevel == ViewLevel::Day) {
            dateStart = d;
            syncWidget();
        }
    });
    p = painters[ViewLevel::Month];
    layerMonth = new LayerMonth;
    p->addLayer(layerMonth);
    p->setMouseTracking(true);
    connect(this, &MiniCalendar::sigPress, [this](const QDate &d) {
        if (viewLevel == ViewLevel::Month && isViewTypeMonth) {
            dateStart = d;
            syncWidget();
        }
    });
}

void MiniCalendarDropDown::syncWidget() {
    MiniCalendar::syncWidget();
    if (!prepared) {
        return;
    }
    layerDay->set(viewType, dateTopLeft.daysTo(dateStart));
    if (isViewTypeMonth) {
        int years = dateStart.year() - dateTopLeft.year();
        int months = dateStart.month() - dateTopLeft.month();
        layerMonth->set(true, years * 12 + months);
    } else {
        layerMonth->set(false, -1);
    }
}

LayerDay::LayerDay(): len(0), count(), hb(-1), he(-1), sb(-1), se(-1) {
}

void LayerDay::set(int i0, int i1) {
    len = i0;
    sb = i1;
    se = sb + len - 1;
    parent->update();
}

void LayerDay::mouseEntered(int column, int row) {
    if (len > 0) {
        hb = row * 7 + column;
        he = hb + len - 1;
    } else {
        hb = -1;
        he = -1;
    }
    parent->update();
}

void LayerDay::mouseLeaved() {
    hb = -1;
    he = -1;
    parent->update();
}

void LayerDay::beforeDrawing(QPainter &p) {
    count = 0;
}

#define BD 2

void LayerDay::drawSlot(QPainter &p, QRect &area, int column, int row) {
    bool hovered = count >= hb && count <= he;
    bool selected = count >= sb && count <= se;
    if (selected) {
        p.fillRect(area, Styles::BLUE_0->color);
        if (hovered) {
            if (count == sb) {
                if (len == 1) {
                    p.fillRect(area.adjusted(BD, BD, -BD, -BD), Styles::GRAY_0->color);
                } else {
                    p.fillRect(area.adjusted(BD, BD, 0, -BD), Styles::GRAY_0->color);
                }
            } else if (count == se) {
                p.fillRect(area.adjusted(0, BD, -BD, -BD), Styles::GRAY_0->color);
            } else {
                p.fillRect(area.adjusted(0, BD, 0, -BD), Styles::GRAY_0->color);
            }
        }
    } else if (hovered) {
        p.fillRect(area, Styles::GRAY_4->color);
        if (count == hb) {
            if (len == 1) {
                p.fillRect(area.adjusted(BD, BD, -BD, -BD), Styles::GRAY_0->color);
            } else {
                p.fillRect(area.adjusted(BD, BD, 0, -BD), Styles::GRAY_0->color);
            }
        } else if (count == he) {
            p.fillRect(area.adjusted(0, BD, -BD, -BD), Styles::GRAY_0->color);
        } else {
            p.fillRect(area.adjusted(0, BD, 0, -BD), Styles::GRAY_0->color);
        }
    }
    count++;
}

LayerMonth::LayerMonth(): hovered(-1), selected(-1), count(), isViewTypeMonth() {
}

void LayerMonth::set(bool i, int s) {
    isViewTypeMonth = i;
    selected = s;
    parent->update();
}

void LayerMonth::mouseEntered(int column, int row) {
    if (isViewTypeMonth) {
        hovered = (row << 2) + column;
        parent->update();
    }
 }

void LayerMonth::mouseLeaved() {
    if (isViewTypeMonth) {
        hovered = -1;
        parent->update();
    }
}

bool LayerMonth::shouldDraw() {
    return isViewTypeMonth;
}

void LayerMonth::beforeDrawing(QPainter &p) {
    count = 0;
}

void LayerMonth::drawSlot(QPainter &p, QRect &area, int column, int row) {
    if (!isViewTypeMonth) {
        return;
    }
    if (count == selected) {
        p.fillRect(area, Styles::BLUE_0->color);
        if (count == hovered) {
            p.fillRect(area.adjusted(BD, BD, -BD, -BD), Styles::GRAY_0->color);
        }
    } else if (count == hovered) {
        p.fillRect(area, Styles::GRAY_4->color);
        p.fillRect(area.adjusted(BD, BD, -BD, -BD), Styles::GRAY_0->color);
    }
    count++;
}

DataRange::DataRange(int type): days(type) {
}

ItemRange::ItemRange(QWidget *parent, bool iic): ListItem(parent, iic), label() {
}

void ItemRange::initWidget() {
    if (!prepared) {
        label = getPointer<Label>("l");
        prepared = true;
    }
}

void ItemRange::syncDataToWidget() {
    if (auto md = wData->cast<DataRange>()) {
        label->setText(QString::number(md->days) + "天");
    }
}

void ItemRange::enterEvent(QEvent *event) {
    setState(1);
}

void ItemRange::leaveEvent(QEvent *event) {
    setState(0);
}

DropDownRange::DropDownRange(QWidget *parent, bool iic): DropDown(parent, iic), label(), list(), viewType(), optionsData(new ListData) {
}

void DropDownRange::syncWidgetToData() {
    if (auto cd = wData->cast<CalendarData>()) {
        cd->set(viewType, cd->getDateStart());
    }
}

void DropDownRange::initWidget() {
    DropDown::initWidget();
    label = getPointer<Label>("l");
    list = getPointer<ListWidget>("list");
    for (int t = 1; t <= 7; t++) {
        optionsData->append(new DataRange(t));
    }
    list->setRowHeight(29);
    list->setFixedHeight(29 * 7);
    list->setData(optionsData);
    connect(optionsData, &ListData::sigDataSelected, this, [this](int, int cur) {
        viewType = static_cast<ViewType>(cur + 1);
        optionsData->selectData(-1);
        syncWidgetToData();
        emit sigCloseMenu();
    });
}

DropDownRange::~DropDownRange() {
    delete optionsData;
}

ButtonSwitchView::ButtonSwitchView(QWidget *parent, bool iic): Button(parent, iic) {
}

void ButtonSwitchView::handleButtonActivate(QMouseEvent *ev) {
    if (!selected) {
        Button::handleButtonActivate(ev);
    }
}

SlotsWidgetCalendar::SlotsWidgetCalendar(QWidget *parent, bool iic):
        SlotsWidget(parent, iic), itemBuilder([] { return new ListItemCalendar; }), cd() {
}

SlotsWidgetCalendar::~SlotsWidgetCalendar() {
    if (auto ld = wData->cast<ListData>()) {
        DELETE_LIST(ld->getData())
        ld->clear();
    }
    delete wData;
}

ListItem *SlotsWidgetCalendar::newItem() {
    auto lic = itemBuilder();
    lic->sw = this;
    lic->itemIdx = items.length();
    return lic;
}

void SlotsWidgetCalendar::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.fillRect(rect(), Styles::BLACK->color);
    p.setPen(Styles::GRAY_4->color);
    p.drawRect(0, 0, width() - 1, height() - 1);
    for (int i = 1, y = slotHeight ; i < rows ; i++, y += slotHeight) {
        p.drawLine(0, y, width(), y);
    }
    for (int i = 1, x = slotWidth ; i < columns ; i++, x += slotWidth) {
        p.drawLine(x, 0, x, height());
    }
}
