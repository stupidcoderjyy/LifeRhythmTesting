//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef LIFERHYTHM_CALENDAR_H
#define LIFERHYTHM_CALENDAR_H

#include "Widget.h"
#include "TextLabel.h"
#include "ImgButton.h"
#include "SlotsPainter.h"
#include <QDateTime>

class ContentLayer;
class TitleLayer;

class MiniCalendar : public Widget {
private:
    ContentLayer* layerContent;
    TitleLayer* layerTitle;

};

class MiniCalendarData : public WidgetData {
public:
    enum ViewLevel {
        Day,
        Month,
        Year
    };
private:
    ViewLevel viewLevel;
    QDate topLeftDate;
    int mark1, mark2;
public:
    MiniCalendarData();
};

class ContentLayer : public SlotsPainterLayer {
private:
    int firstVal;
    int mark1, mark2;
    int val, count;
    MiniCalendarData::ViewLevel viewLevel;
public:
    ContentLayer();
    void setVal(MiniCalendarData::ViewLevel viewLevel, int firstVal, int mark1, int mark2);
protected:
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};

class TitleLayer : public SlotsPainterLayer {
protected:
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int row, int column) override;
};

#endif //LIFERHYTHM_CALENDAR_H
