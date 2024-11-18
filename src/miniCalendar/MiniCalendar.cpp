//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "MiniCalendar.h"
#include "RcManagers.h"
#include "DateUtil.h"
#include <QWheelEvent>
#include <QLayout>
#include <QPainterPath>

MiniCalendar::MiniCalendar(QWidget *parent): Widget(parent), prev(), next(), title(), layerContent(), layerTitle(),
                                             painterWeekdayTitle(), painters(), bottom(), viewLevel(Day), running(),
                                             maxViewLevel(Day) {
    init();
}

void MiniCalendar::loadDate(const QDate &date) const {
    if (wData) {
        auto cd = wData->cast<MiniCalendarData>();
        cd->setTopLeft(QDate(date.year(), date.month(), 1));
    }
}

void MiniCalendar::setMaxViewLevel(ViewLevel level) {
    maxViewLevel = level;
    if (viewLevel > maxViewLevel) {
        wData->cast<MiniCalendarData>()->setViewLevel(maxViewLevel);
    }
}

void MiniCalendar::syncDataToWidget() {
    auto cd = wData->cast<MiniCalendarData>();
    layerContent->setVal(cd->viewLevel, cd->firstVal, cd->mark1, cd->mark2, cd->mark3);
    switch (cd->viewLevel) {
        case Day: {
            title->setButtonText(cd->titleDate.toString("yyyy年M月"));
            break;
        }
        case Month: {
            title->setButtonText(cd->titleDate.toString("yyyy年"));
            break;
        }
        case Year: {
            int y = cd->titleDate.year();
            y = y - y % 10;
            title->setButtonText(QString("%1 - %2").arg(y).arg(y + 9));
            break;
        }
    }
    onViewLevelChanged(cd->viewLevel);
}

void MiniCalendar::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this] {
        syncDataToWidget();
    });
}

#define TIMES 9
#define SLOT_SIZE_1 4 * TIMES
#define SLOT_SIZE_2 7 * TIMES
#define WIDTH 7 * SLOT_SIZE_1
#define HEIGHT 8 * SLOT_SIZE_1

void MiniCalendar::init() {
    if (running) {
        return;
    }
    setFixedSize(WIDTH, HEIGHT);
    running = true;
    WidgetFactoryStorage::get("test:widget_mini_calendar")->apply(nullptr, this);
    prev = getPointer<ArrowButton>("prev");
    connect(prev, &ArrowButton::sigSelected, this, [this](){
        handleArrowButton(false);
    });
    next = getPointer<ArrowButton>("next");
    connect(next, &ArrowButton::sigSelected, this, [this](){
        handleArrowButton(true);
    });
    title = getPointer<Button>("title");
    connect(title, &Button::sigActivated, this, [this] {
        auto cd = wData->cast<MiniCalendarData>();
        switch (cd->viewLevel) {
            case Day:
                cd->setViewLevel(Month);
                break;
            case Month:
                cd->setViewLevel(Year);
                title->setButtonStyleEnabled(false);
                title->setButtonEnabled(false);
                break;
            default:
                break;
        }
    });
    getPointer<Widget>("top")->setFixedHeight(SLOT_SIZE_1);
    layerTitle = new TitleLayer;
    layerContent = new ContentLayer;
    painterWeekdayTitle = new SlotsPainter(this);
    painterWeekdayTitle->setFixedSize(WIDTH, SLOT_SIZE_1);
    painterWeekdayTitle->setSlotCount(7, 1);
    painterWeekdayTitle->appendLayer(layerTitle);

    auto* painterDay = new SlotsPainter(this);
    painterDay->setFixedSize(WIDTH, SLOT_SIZE_1 * 6);
    painterDay->setSlotCount(7, 6);
    painterDay->appendLayer(layerContent);
    connect(painterDay, &SlotsPainter::sigScroll, this, [this](int dy){
        auto cd = wData->cast<MiniCalendarData>();
        if (dy < 0) {
            cd->setTopLeft(cd->topLeft.addDays(7));
        } else {
            cd->setTopLeft(cd->topLeft.addDays(-7));
        }
    });

    auto *painterMonth = new SlotsPainter(this);
    painterMonth->setFixedSize(WIDTH, SLOT_SIZE_2 * 4);
    painterMonth->setSlotCount(4, 4);
    painterMonth->close();
    connect(painterMonth, &SlotsPainter::sigReleaseSlot, this, [this](int c, int r) {
        if (maxViewLevel == Day) {
            auto cd = wData->cast<MiniCalendarData>();
            cd->topLeft = cd->topLeft.addMonths((r << 2) + c);
            cd->setViewLevel(Day);
        }
    });
    connect(painterMonth, &SlotsPainter::sigScroll, this, [this](int dy){
        auto cd = wData->cast<MiniCalendarData>();
        if (dy < 0) {
            cd->setTopLeft(cd->topLeft.addMonths(4));
        } else {
            cd->setTopLeft(cd->topLeft.addMonths(-4));
        }
    });

    auto *painterYear = new SlotsPainter(this);
    painterYear->setFixedSize(WIDTH, SLOT_SIZE_2 * 4);
    painterYear->setSlotCount(4, 4);
    painterYear->close();
    connect(painterYear, &SlotsPainter::sigReleaseSlot, this, [this](int c, int r) {
        auto cd = wData->cast<MiniCalendarData>();
        cd->topLeft = cd->topLeft.addYears((r << 2) + c);
        if (maxViewLevel > Year) {
            cd->setViewLevel(Month);
        }
    });
    connect(painterYear, &SlotsPainter::sigScroll, this, [this](int dy){
        auto cd = wData->cast<MiniCalendarData>();
        if (dy < 0) {
            cd->setTopLeft(cd->topLeft.addYears(4));
        } else {
            cd->setTopLeft(cd->topLeft.addYears(-4));
        }
    });

    painters[Day] = painterDay;
    painters[Month] = painterMonth;
    painters[Year] = painterYear;

    bottom = static_cast<QVBoxLayout*>(getPointer<Widget>("bottom")->layout());
    bottom->addWidget(painterWeekdayTitle);
    bottom->addWidget(painterDay);
    auto d = new MiniCalendarData;
    setData(d);
    loadDate(QDate::currentDate());
    if (viewLevel < maxViewLevel) {
        d->setViewLevel(maxViewLevel);
    }
}

void MiniCalendar::onViewLevelChanged(ViewLevel level) {
    if (viewLevel == level) {
        return;
    }
    if (viewLevel == Year) {
        title->setButtonEnabled(true);
    }
    if (viewLevel == Day) {
        bottom->removeWidget(painterWeekdayTitle);
        painterWeekdayTitle->close();
    }
    painters[viewLevel]->close();
    painters[viewLevel]->removeLayer(layerContent);
    if (level == Day) {
        bottom->replaceWidget(painters[viewLevel], painterWeekdayTitle);
        bottom->addWidget(painters[Day]);
        painterWeekdayTitle->show();
    } else {
        bottom->replaceWidget(painters[viewLevel], painters[level]);
    }
    painters[level]->appendLayer(layerContent);
    painters[level]->show();
    viewLevel = level;
}

void MiniCalendar::handleArrowButton(bool add) {
    auto cd = wData->cast<MiniCalendarData>();
    switch (viewLevel) {
        case Day: {
            auto d = cd->titleDate.addMonths(add ? 1 : -1);
            d.setDate(d.year(), d.month(), 1);
            cd->setTopLeft(d);
            break;
        }
        case Month: {
            auto d = cd->titleDate.addYears(add ? 1 : -1);
            d.setDate(d.year(), 1, 1);
            cd->setTopLeft(d);
            break;
        }
        case Year: {
            cd->setTopLeft(cd->titleDate.addYears(add ? 10 : -10));
            break;
        }
    }
}

MiniCalendarData::MiniCalendarData(): viewLevel(Day), mark1(0), mark2(0), mark3(0), firstVal() {
}

void MiniCalendarData::setViewLevel(ViewLevel level) {
    if (viewLevel == level) {
        return;
    }
    viewLevel = level;
    switch (level) {
        case Day: {
            topLeft.setDate(topLeft.year(), topLeft.month(), 1);
            break;
        }
        case Month: {
            topLeft.setDate(topLeft.year(), 1, 1);
            break;
        }
        default: {
            topLeft.setDate(topLeft.year() - topLeft.year() % 10, 1, 1);
            break;
        }
    }
    setTopLeft(topLeft);
}

void MiniCalendarData::setTopLeft(const QDate &d) {
    ensureTopLeft(d);
    mark3 = -1;
    auto today = QDate::currentDate();
    switch (viewLevel) {
        case Day: {
            if (topLeft.day() > 14) {
                auto d1 = topLeft.addMonths(1);
                d1.setDate(d1.year(), d1.month(), 1);
                mark1 = static_cast<int>(topLeft.daysTo(d1));
                mark2 = mark1 + d1.daysInMonth();
                titleDate = d1;
            } else {
                auto d1 = QDate(topLeft.year(), topLeft.month(), 1);
                mark1 = 0;
                mark2 = d1.daysInMonth() - topLeft.day() + 1;
                titleDate = d;
            }
            firstVal = topLeft.day();
            if (auto days = topLeft.daysTo(today); days >= 0 && days < 42) {
                mark3 = days;
            }
            break;
        }
        case Month: {
            if (topLeft.month() > 8) {
                auto d1 = topLeft.addYears(1);
                d1.setDate(d1.year(), 1, 1);
                mark1 = 13 - topLeft.month();
                mark2 = mark1 + 11;
                titleDate = d1;
            } else {
                mark1 = 0;
                mark2 = 13 - topLeft.month();
                titleDate = d;
            }
            firstVal = topLeft.month();
            if (today >= topLeft && today < topLeft.addMonths(16)) {
                if (today.year() == topLeft.year()) {
                    mark3 = today.month() - topLeft.month();
                } else {
                    mark3 = 12 + today.month() - topLeft.month();
                }
            }
            break;
        }
        case Year: {
            int i = topLeft.year() % 10;
            if (i > 4) {
                auto d1 = topLeft.addYears(10 - i);
                mark1 = 10 - i;
                mark2 = mark1 + 9;
                titleDate = d1;
            } else {
                mark1 = 0;
                mark2 = 10 - i;
                titleDate = d;
            }
            firstVal = topLeft.year();
            auto end = topLeft.addYears(16);
            if (today >= topLeft && today < end) {
                mark3 = today.year() - topLeft.year();
            }
            break;
        }
    }
    emit sigDataChanged();
}

void MiniCalendarData::ensureTopLeft(const QDate &date) {
    switch (viewLevel) {
        case Day: {
            topLeft = date.addDays(1 - date.dayOfWeek());
            break;
        }
        case Month: {
            topLeft = date.addMonths(-(date.month() & 0x3) + 1);
            break;
        }
        default: {
            topLeft.setDate(date.year() & 0xFFFC, 1, 1);
            break;
        }
    }
}

ContentLayer::ContentLayer(): firstVal(), mark1(-1), mark2(-1), mark3(-1), mark4(-1), val(), count(), rx(), ry(),
                              viewLevel() {
}

void ContentLayer::setVal(ViewLevel vl, int fv, int m1, int m2, int m3) {
    viewLevel = vl;
    firstVal = fv;
    mark1 = m1;
    mark2 = m2;
    mark3 = m3;
    mark4 = -1;
    if (vl == Day) {
        rx = SLOT_SIZE_1 >> 1;
        ry = 32;
    } else {
        rx = SLOT_SIZE_2 >> 1;
        ry = 50;
    }
    parent->update();
}

void ContentLayer::beforeDrawing(QPainter &p) {
    p.setFont(Styles::FONT_MAIN);
    p.setPen(Styles::GRAY_4->color);
    val = firstVal;
    count = 0;
    p.fillRect(parent->rect(), Styles::BLACK->color);
}

void ContentLayer::drawSlot(QPainter &p, QRect &area, int column, int row) {
    if (count == mark4) {
        p.fillRect(area, Styles::GRAY_1->color);
    }
    if (count == mark1) {
        p.setPen(Styles::GRAY_TEXT_0->color);
        if (viewLevel != Year && mark1 > 0) {
            val = 1;
        }
    } else if (count == mark2) {
        p.setPen(Styles::GRAY_3->color);
        if (viewLevel != Year) {
            val = 1;
        }
    } else if (count == mark3) {
        QPainterPath path;
        QPoint center(area.x() + rx,area.y() + ry);
        path.addEllipse(center, 2, 2);
        p.fillPath(path, Styles::BLUE_1->color);
    }
    if (viewLevel == Month) {
        p.drawText(area, Qt::AlignCenter, QString::number(val++) + "月");
    } else {
        p.drawText(area, Qt::AlignCenter, QString::number(val++));
    }
    count++;
}

void ContentLayer::mousePressed(int column, int row) {
    if (viewLevel != Day) {
        mark4 = row * 4 + column;
        parent->update();
    }
}

void TitleLayer::beforeDrawing(QPainter &p) {
    p.setFont(FontBuilder(Styles::FONT_TYPE_MAIN, Styles::FONT_MAIN).setBoldWeight().get());
    p.fillRect(parent->rect(), Styles::BLACK->color);
    p.setPen(Styles::GRAY_TEXT_0->color);
}

void TitleLayer::drawSlot(QPainter &p, QRect &area, int column, int row) {
    p.drawText(area, Qt::AlignCenter, DateUtil::WEEKDAYS_CN[column]);
}
