//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "MiniCalendar.h"
#include "RcManagers.h"
#include "DateUtil.h"
#include <QWheelEvent>
#include <QLayout>

MiniCalendarData::MiniCalendarData(): viewLevel(Day), mark1(0), mark2(0) {
}

ContentLayer::ContentLayer(): firstVal(), mark1(-1), mark2(-1), val(), count(), viewLevel() {
}

void ContentLayer::setVal(MiniCalendarData::ViewLevel vl, int fv, int m1, int m2) {
    viewLevel = vl;
    firstVal = fv;
    mark1 = m1;
    mark2 = m2;
}

void ContentLayer::beforeDrawing(QPainter &p) {
    p.setFont(Styles::FONT_LARGE);
    p.setPen(Styles::GRAY_4->color);
    val = firstVal;
    count = 0;
}

void ContentLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
    if (count == mark1) {
        p.setPen(Styles::GRAY_TEXT_0->color);
        if (viewLevel != MiniCalendarData::Year && mark1 > 0) {
            val = 1;
        }
    } else if (count == mark2) {
        p.setPen(Styles::GRAY_3->color);
        if (viewLevel != MiniCalendarData::Year) {
            val = 1;
        }
    }
    if (viewLevel == MiniCalendarData::Month) {
        p.drawText(area, Qt::AlignCenter, QString::number(val++) + "月");
    } else {
        p.drawText(area, Qt::AlignCenter, QString::number(val++));
    }
    count++;
}

void TitleLayer::beforeDrawing(QPainter &p) {
    p.setFont(FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_MAIN).setBoldWeight().get());
    p.fillRect(parent->rect(), Styles::GRAY_0->color);
    p.setPen(Styles::GRAY_TEXT_0->color);
}

void TitleLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
    p.drawText(area, Qt::AlignCenter, DateUtil::WEEKDAYS_CN[column]);
}
