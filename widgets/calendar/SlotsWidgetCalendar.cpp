//
// Created by JYY on 24-12-13.
//

#include <SlotsWidgetCalendar.h>
#include <Calendar.h>
#include <MemUtil.h>

USING_NAMESPACE(lr::calendar)

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
    int w = region.width();
    int h = region.height();
    p.fillRect(0, 0, w, h, Styles::BLACK->color);
    p.setPen(Styles::GRAY_4->color);
    for (int y = 0 ; y <= h ; y += slotHeight) {
        p.drawLine(0, y, w, y);
    }
    for (int x = 0 ; x <= w ; x += slotWidth) {
        p.drawLine(x, 0, x, h);
    }
}

void SlotsWidgetCalendar::updateBase() {
    SlotsWidget::updateBase();
    update();
}
