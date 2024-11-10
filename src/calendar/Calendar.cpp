//
// Created by stupid_coder_jyy on 24-11-8.
//

#include "Calendar.h"
#include "CalendarView.h"
#include <qlayout.h>
#include <RcManagers.h>

#include "DateUtil.h"

Calendar::Calendar(QWidget *parent): Widget(parent), spWeekDayTitle() {
}

void Calendar::onFinishedParsing(Handlers &handlers, NBT *widgetTag) {
    handlers << [](QWidget *t) {
        auto c = static_cast<Calendar*>(t);
        c->spWeekDayTitle = c->getPointer<SlotsPainter>("painter");
        c->spWeekDayTitle->appendLayer(new WeekDaysTitleLayer);
    };
}

void WeekDaysTitleLayer::beforeDrawing(QPainter &p) {
    p.setFont(Styles::FONT_SMALL);
    p.setPen(Styles::GRAY_TEXT_0->color);
}

void WeekDaysTitleLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
    p.fillRect(area, Styles::BLACK->color);
    p.drawText(area, "周" + DateUtil::WEEKDAYS_CN[column], Qt::AlignLeft | Qt::AlignVCenter);
}

CalendarBuilder::CalendarBuilder(QWidget* parent) {
    c = WidgetFactoryStorage::get("lr:widget_calendar")->applyAndCast<Calendar>(parent, new Calendar);
}

CalendarBuilder & CalendarBuilder::setView(CalendarView *view) {
    c->layout()->addWidget(view);
    return *this;
}

Calendar * CalendarBuilder::get() {
    auto res = c;
    c = nullptr;
    return res;
}

CalendarBuilder::~CalendarBuilder() {
    delete c;
}
