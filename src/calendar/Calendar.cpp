//
// Created by stupid_coder_jyy on 24-11-8.
//

#include "Calendar.h"

USING_NAMESPACE(lr)

CalendarData::CalendarData(): viewType(ViewType::D3), dateStart(QDate::currentDate()) {
}

USING_NAMESPACE(lr::calendar)

DropDownMiniCalendar::DropDownMiniCalendar(QWidget *parent, bool iic): MiniCalendar(parent, iic),
        layerDay(), viewType() {
}

void DropDownMiniCalendar::syncDataToWidget() {
    if (auto md = wData->cast<CalendarData>()) {
        dateStart = md->dateStart;
        viewType = md->viewType;
        syncWidget();
    }
}

void DropDownMiniCalendar::syncWidgetToData() {
    if (auto md = wData->cast<CalendarData>()) {
        md->dateStart = dateStart;
        md->viewType = viewType;
        emit md->sigDataChanged();
    }
}

void DropDownMiniCalendar::initWidget() {
    MiniCalendar::initWidget();
    auto p = painters[ViewLevel::Day];
    layerDay = new LayerDay;
    p->addLayer(layerDay);
    p->setMouseTracking(true);
    syncDataToWidget();
}

void DropDownMiniCalendar::syncWidget() {
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
