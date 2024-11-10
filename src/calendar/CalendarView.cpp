//
// Created by JYY on 24-11-6.
//

#include "CalendarView.h"
#include "DateUtil.h"
#include "MemUtil.h"
#include <LifeRhythm.h>
#include "RcManagers.h"

CalendarItem::CalendarItem(QWidget *parent): ListItem(parent), isToday(), isCurrentMonth() {
}

void CalendarItem::syncDataToWidget() {
    ListItem::syncDataToWidget();
    if (auto ld = listData->cast<CalendarData>()) {
        date = ld->getDate().addDays(dataIdx);
        auto today = QDate::currentDate();
        isToday = date == today;
        isCurrentMonth = date.month() == today.month();
        updateOutline();
        update();
    }
}

void CalendarItem::resizeEvent(QResizeEvent *event) {
    updateOutline();
}

void CalendarItem::updateOutline() {
    outline = rect();
    if (dataIdx >= 35) {
        outline.setHeight(height() - 1);
    }
    if (dataIdx % 7 == 6) {
        outline.setWidth(width() - 1);
    }
}

void CalendarItem::paintEvent(QPaintEvent *event) {
    if (!date.isValid()) {
        return;
    }
    QPainter painter(this);
    QPen pen;
    painter.fillRect(rect(), Styles::BLACK->color);
    painter.setPen(Styles::GRAY_1->color);
    painter.drawRect(outline);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (isCurrentMonth) {
        if (isToday) {
            painter.setPen(Styles::BLUE_1->color);
            painter.setBrush(Styles::BLUE_1->color);
            painter.drawEllipse({15, 15}, 12, 12);
            painter.setFont(FontBuilder(Styles::FONT_TYPE_MAIN).setBoldWeight().setMediumSize().get());
        } else {
            painter.setFont(Styles::FONT_MAIN);
        }
        pen.setColor(Styles::GRAY_TEXT_0->color);
    } else {
        painter.setFont(Styles::FONT_MAIN);
        pen.setColor(Styles::GRAY_4->color);
    }
    painter.setPen(pen);
    painter.drawText({0, 0, 30, 30}, QString::number(date.day()), QTextOption(Qt::AlignCenter));
    painter.setRenderHint(QPainter::Antialiasing, false);
}

CalendarView::CalendarView(QWidget *parent): SlotsWidget(parent) {
    setSlotCount(7, 6);
    setMinimumSize(700, 600);
}

void CalendarView::setData(WidgetData *data) {
    if (dynamic_cast<CalendarData *>(data)) {
        StandardWidget::setData(data);
    }
}

void CalendarView::loadCalendar(const QDate &date) const {
    if (wData) {
        auto d = QDate(date.year(), date.month(), 1);
        wData->cast<CalendarData>()->ensureData(d.addDays(1 - d.dayOfWeek()));
    }
}

ListItem * CalendarView::newItem() {
    return new CalendarItem();
}

void MessageCalendarData::onReplied(int result) {
    Message::onReplied(result);
}

CalendarData::CalendarData(const QString& msgId): msgLoc("lr:calendar/" + msgId) {
    lr::LifeRhythm::get()->getMessageHandler().registerListener(getReceiverLoc(), new CalendarUpdateListener(this));
}

void CalendarData::fromMessage(const NBT *nbt) {
    for (int i = 0; i < 42; i++) {
        append(nullptr);
    }
}

void CalendarData::ensureData(const QDate &d) {
    if (date == d) {
        return;
    }
    date = d;
    auto& handler = lr::LifeRhythm::get()->getMessageHandler();
    handler.sendDirectMessage(new MessageCalendarData(this));
}

CalendarData::~CalendarData() {
    lr::LifeRhythm::get()->getMessageHandler().unregisterListeners(getReceiverLoc());
}

MessageCalendarData::MessageCalendarData(CalendarData* d): Message(d->getSenderLoc()), d(d) {
}

NBT* MessageCalendarData::buildData() {
    auto nbt = new NBT();
    nbt->putInt("d")->setVal(DATE_2_INT(d->date));
    return nbt;
}

CalendarUpdateListener::CalendarUpdateListener(CalendarData *d): d(d) {
}

int CalendarUpdateListener::onReceived(const Identifier &sender, const NBT *data) {
    DELETE_LIST(d->data)
    d->clear();
    d->fromMessage(data);
    d->markAll();
    return 16;
}
