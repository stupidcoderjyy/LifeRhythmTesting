//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "MiniCalendar.h"
#include "RcManagers.h"
#include "DateUtil.h"
#include <QWheelEvent>
#include <QLayout>
#include "ArrowButton.h"
#include "Button.h"

MiniCalendar::MiniCalendar(QWidget *parent): Widget(parent), prev(), next(), title(), layerContent(),
layerTitle(), painterWeekdayTitle(), painterContent() {
}

void MiniCalendar::resizeEvent(QResizeEvent *event) {
    init();
}

void MiniCalendar::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this](){
        auto mcd = wData->cast<MiniCalendarData>();
        layerContent->setVal(mcd->viewLevel, mcd->firstVal, mcd->mark1, mcd->mark2);
    });
}

void MiniCalendar::init() {
    if (running) {
        return;
    }
    running = true;
    WidgetFactoryStorage::get("test:widget_mini_calendar")->apply(nullptr, this);
    prev = getPointer<ArrowButton>("prev");
    next = getPointer<ArrowButton>("next");
    title = getPointer<Button>("button");
    layerTitle = new TitleLayer;
    layerContent = new ContentLayer;
    painterWeekdayTitle = new SlotsPainter(this);
    painterWeekdayTitle->appendLayer(layerTitle);
    painterContent = new SlotsPainter(this);
    painterContent->appendLayer(layerContent);
    auto d = new MiniCalendarData;
    d->setTopLeft(QDate::currentDate());
    setData(d);
    auto* bottom = getPointer<Widget>("bottom");
    bottom->layout()->addWidget(painterWeekdayTitle);
    bottom->layout()->addWidget(painterContent);
}

MiniCalendarData::MiniCalendarData(): viewLevel(Day), mark1(0), mark2(0), firstVal() {
}

void MiniCalendarData::setViewLevel(ViewLevel level) {
    if (viewLevel == level) {
        return;
    }
    setTopLeft(topLeftDate);
}

void MiniCalendarData::setTopLeft(const QDate &d) {
    topLeftDate = d;
    switch (viewLevel) {
        case Day: {
            if (d.day() > 14) {
                auto d1 = d.addMonths(1);
                d1.setDate(d1.year(), d1.month(), 1);
                mark1 = static_cast<int>(d.daysTo(d1));
                mark2 = mark1 + d1.daysInMonth();
                titleDate = d1;
            } else {
                auto d1 = QDate(d.year(), d.month(), 1);
                mark1 = 0;
                mark2 = d1.daysInMonth() - d.day() + 1;
                titleDate = d;
            }
            firstVal = topLeftDate.day();
            break;
        }
        case Month: {
            if (d.month() > 8) {
                auto d1 = d.addYears(1);
                d1.setDate(d1.year(), 1, 1);
                mark1 = 13 - d.month();
                mark2 = mark1 + 11;
                titleDate = d1;
            } else {
                mark1 = 0;
                mark2 = 13 - d.month();
                titleDate = d;
            }
            break;
        }
        case Year: {
            int i = 10 - d.year() % 10;
            if (i < 5) {
                auto d1 = d.addYears(10 - i);
                mark1 = i;
                mark2 = mark1 + 9;
                titleDate = d1;
            } else {
                mark1 = 0;
                mark2 = i;
                titleDate = d;
            }
            break;
        }
    }
    emit sigDataChanged();
}

void MiniCalendarData::updateData() {

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
