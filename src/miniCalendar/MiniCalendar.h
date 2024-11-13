//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef MINICALENDAR_H
#define MINICALENDAR_H

#include "Widget.h"
#include "TextLabel.h"
#include "ImgButton.h"
#include "SlotsPainter.h"
#include <QDateTime>

class ContentLayer;
class TitleLayer;
class ArrowButton;
class Button;

class MiniCalendar : public Widget {
private:
    ArrowButton* prev;
    ArrowButton* next;
    Button* title;
    ContentLayer* layerContent;
    TitleLayer* layerTitle;
    SlotsPainter* painterWeekdayTitle;
    SlotsPainter* painterContent;
    bool running;
public:
    MiniCalendar(QWidget* parent = nullptr);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void connectModelView() override;
private:
    void init();
};

class MiniCalendarData : public WidgetData {
    friend class MiniCalendar;
public:
    enum ViewLevel {
        Day,
        Month,
        Year
    };
private:
    ViewLevel viewLevel;
    QDate topLeftDate;
    QDate titleDate;
    int mark1, mark2;
    int firstVal;
public:
    MiniCalendarData();
    void setViewLevel(ViewLevel level);
    void setTopLeft(const QDate& d);
private:
    void updateData();
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
