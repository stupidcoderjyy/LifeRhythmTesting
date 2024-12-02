//
// Created by stupid_coder_jyy on 24-11-8.
//

#include "Calendar.h"

#include <RcManagers.h>

USING_NAMESPACE(lr)

CalendarData::CalendarData(): viewType(ViewType::D3), dateStart(QDate::currentDate()) {
}

Calendar::Calendar(QWidget *parent, bool iic): Widget(parent, iic), dropDownMc(), miniCalendar(),
                                               labelRange(), labelDate() {
}

void Calendar::setData(WidgetData *d) {
    if (dynamic_cast<CalendarData*>(d) && !wData) {
        Widget::setData(d);
        initWidget();
    }
}

void Calendar::syncDataToWidget() {
    if (auto* cd = wData->cast<CalendarData>()) {
        miniCalendar->syncDataToWidget();
        labelRange->setText(QString::number(cd->viewType) + "天");
        labelRange->setSizeToText();
        QString s;
        switch (cd->viewType) {
            case calendar::Month: {
                s = cd->dateStart.toString("yyyy年MM月");
                break;
            }
            case calendar::D1: {
                s = cd->dateStart.toString("yyyy年 MM月dd日");
                break;
            }
            default: {
                auto d = cd->dateStart, d1 = d.addDays(cd->viewType - 1);
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
    }
}

void Calendar::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, &Calendar::syncDataToWidget);
}

void Calendar::initWidget() {
    if (wData && !prepared) {
        auto cd = wData->cast<CalendarData>();
        dropDownMc = getPointer<DropDown>("dropdown_mini_calendar");
        labelDate = dropDownMc->getPointer<Label>("l");
        miniCalendar = dropDownMc->getPointer<calendar::MiniCalendarDropDown>("c");
        miniCalendar->setData(cd);
        connect(miniCalendar, &MiniCalendar::sigRelease, this, [this] {
            if (miniCalendar->getViewLevel() == mini_calendar::Day) {
                miniCalendar->syncWidgetToData();
                emit dropDownMc->getMenu()->sigSelectOption();
            }
        });

        auto dp = getPointer<DropDown>("dropdown_range");
        dp->setData(cd);
        labelRange = dp->getPointer<Label>("l");

        syncDataToWidget();
        prepared = true;
    }
}

USING_NAMESPACE(lr::calendar)

MiniCalendarDropDown::MiniCalendarDropDown(QWidget *parent, bool iic): MiniCalendar(parent, iic),
                                                                       layerDay(), viewType() {
}

void MiniCalendarDropDown::syncDataToWidget() {
    if (auto md = wData->cast<CalendarData>()) {
        dateStart = md->dateStart;
        viewType = md->viewType;
        syncWidget();
    }
}

void MiniCalendarDropDown::syncWidgetToData() {
    if (auto md = wData->cast<CalendarData>()) {
        md->dateStart = dateStart;
        md->viewType = viewType;
        emit md->sigDataChanged();
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
}

void MiniCalendarDropDown::syncWidget() {
    MiniCalendar::syncWidget();
    if (!prepared) {
        return;
    }
    layerDay->set(viewType, dateTopLeft.daysTo(dateStart));
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
                p.fillRect(area.adjusted(BD, BD, 0, -BD), Styles::GRAY_0->color);
            } else if (count == se) {
                p.fillRect(area.adjusted(0, BD, -BD, -BD), Styles::GRAY_0->color);
            } else {
                p.fillRect(area.adjusted(0, BD, 0, -BD), Styles::GRAY_0->color);
            }
        }
    } else if (hovered) {
        p.fillRect(area, Styles::GRAY_4->color);
        if (count == hb) {
            p.fillRect(area.adjusted(BD, BD, 0, -BD), Styles::GRAY_0->color);
        } else if (count == he) {
            p.fillRect(area.adjusted(0, BD, -BD, -BD), Styles::GRAY_0->color);
        } else {
            p.fillRect(area.adjusted(0, BD, 0, -BD), Styles::GRAY_0->color);
        }
    }
    count++;
}

DataRange::DataRange(int type): days(type) {
}

ItemRange::ItemRange(QWidget *parent, bool iic): ListItem(parent, iic), label() {
}

void ItemRange::initWidget() {
    label = getPointer<Label>("l");
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

ListRange::ListRange(QWidget *parent, bool iic): ListWidget(parent, iic) {
}

ListItem * ListRange::newItem() {
    return WidgetFactoryStorage::get("test:calendar/item_range")->applyAndCast<ItemRange>();
}

DropDownRange::DropDownRange(QWidget *parent, bool iic): DropDown(parent, iic), label(), list(), viewType() {
}

void DropDownRange::syncWidgetToData() {
    if (auto md = wData->cast<CalendarData>()) {
        md->viewType = viewType;
        emit md->sigDataChanged();
    }
}

void DropDownRange::initWidget() {
    DropDown::initWidget();
    label = getPointer<Label>("l");
    list = getPointer<ListRange>("list");
    auto cd = new ListData;
    for (int t = 1; t <= 7; t++) {
        cd->append(new DataRange(t));
    }
    list->setRowHeight(29);
    list->setFixedHeight(29 * 7);
    list->setData(cd);
    connect(cd, &ListData::sigDataSelected, this, [this, cd](int, int cur) {
        viewType = static_cast<ViewType>(cur + 1);
        cd->selectData(-1);
        syncWidgetToData();
        emit menu->sigSelectOption();
    });
}
