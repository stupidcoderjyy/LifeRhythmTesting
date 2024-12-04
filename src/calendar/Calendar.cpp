//
// Created by stupid_coder_jyy on 24-11-8.
//

#include "Calendar.h"

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

Calendar::Calendar(QWidget *parent, bool iic): Widget(parent, iic), dropdownMiniCalendar(), dropdownRange(),
        btnPrev(), btnNext(), btnWeek(), btnMonth(), miniCalendar(), labelRange(), labelDate() {
}

void Calendar::setData(WidgetData *d) {
    auto cd = dynamic_cast<CalendarData*>(d);
    if (!cd) {
        throw Error("[Calendar::setData] requires CalendarData");
    }
    if (prepared) {
        miniCalendar->setData(cd);
        dropdownRange->setData(cd);
    }
    Widget::setData(d);
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

void Calendar::initWidget() {
    if (!prepared) {
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
        prepared = true;
    }
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
