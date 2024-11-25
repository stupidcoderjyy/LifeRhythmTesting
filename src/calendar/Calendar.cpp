//
// Created by stupid_coder_jyy on 24-11-8.
//

#include "Calendar.h"

USING_NAMESPACE(lr::calendar)

DropDownMiniCalendar::DropDownMiniCalendar(QWidget *parent, bool iic): MiniCalendar(parent, iic) {
}

void DropDownMiniCalendar::initWidget() {
    MiniCalendar::initWidget();
    painters[ViewLevel::Day]->addLayer(new LayerDay);
    painters[ViewLevel::Day]->setMouseTracking(true);
}

LayerDay::LayerDay(): viewType(ViewType::D2), length(2), count(), begin(-1), end(-1) {
}

void LayerDay::mouseEntered(int column, int row) {
    if (viewType != ViewType::Month) {
        begin = row * 7 + column;
        end = begin + length;
    } else {
        begin = -1;
        end = -1;
    }
    parent->update();
}

void LayerDay::beforeDrawing(QPainter &p) {
    count = 0;
}

void LayerDay::drawSlot(QPainter &p, QRect &area, int column, int row) {
    if (count >= begin && count <= end) {
        p.fillRect(area, Styles::GRAY_1->color);
    }
    count++;
}
