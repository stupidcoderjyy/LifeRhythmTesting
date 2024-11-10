//
// Created by stupid_coder_jyy on 24-11-8.
//

#ifndef LIFERHYTHMTESTING_CALENDAR_H
#define LIFERHYTHMTESTING_CALENDAR_H

#include "Widget.h"
#include "SlotsPainter.h"

class Calendar : public Widget {
private:
    SlotsPainter* spWeekDayTitle;
public:
    explicit Calendar(QWidget* parent = nullptr);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
};

class WeekDaysTitleLayer : public SlotsPainterLayer {
protected:
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};

class CalendarView;

class CalendarBuilder {
private:
    Calendar* c;
public:
    explicit CalendarBuilder(QWidget* parent = nullptr);
    CalendarBuilder& setView(CalendarView* view);
    Calendar* get();
    ~CalendarBuilder();
};


#endif //LIFERHYTHMTESTING_CALENDAR_H
