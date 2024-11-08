//
// Created by stupid_coder_jyy on 24-11-8.
//

#ifndef LIFERHYTHMTESTING_CALENDAR_H
#define LIFERHYTHMTESTING_CALENDAR_H

#include "Widget.h"
#include "SlotsPainter.h"

class Calendar : public Widget {

};

class WeekDaysTitleLayer : public SlotsPainterLayer {
protected:
    void beforeDrawing(QPainter &p) override;

    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};


#endif //LIFERHYTHMTESTING_CALENDAR_H
