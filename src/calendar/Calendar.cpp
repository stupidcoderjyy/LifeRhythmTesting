//
// Created by stupid_coder_jyy on 24-11-8.
//

#include "Calendar.h"
#include "DateUtil.h"

void WeekDaysTitleLayer::beforeDrawing(QPainter &p) {
    p.setFont(Styles::FONT_MAIN);
    p.setPen(Styles::GRAY_TEXT_0->color);
}

void WeekDaysTitleLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
    p.fillRect(area, Styles::BLACK->color);
    p.drawText(area, "周" + DateUtil::WEEKDAYS_CN[column], Qt::AlignLeft | Qt::AlignVCenter);
}
