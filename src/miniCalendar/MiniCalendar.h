//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef MINICALENDAR_H
#define MINICALENDAR_H

#include <qboxlayout.h>

#include "Widget.h"
#include "TextLabel.h"
#include "SlotsPainter.h"
#include <QDateTime>
#include "ArrowButton.h"

class ContentLayer;
class TitleLayer;

enum ViewLevel {
    Year,
    Month,
    Day
};

class MiniCalendar : public Widget {
protected:
    ArrowButton* prev;
    ArrowButton* next;
    Button* title;
    ContentLayer* layerContent;
    TitleLayer* layerTitle;
    SlotsPainter* painterWeekdayTitle;
    SlotsPainter* painters[3];
    QVBoxLayout* bottom;
    ViewLevel viewLevel;
    ViewLevel maxViewLevel;
    bool running;
public:
    explicit MiniCalendar(QWidget* parent = nullptr);
    void loadDate(const QDate& date) const;
    void setMaxViewLevel(ViewLevel level);
protected:
    void syncDataToWidget() override;
    void connectModelView() override;
private:
    void init();
    void onViewLevelChanged(ViewLevel level);
    void handleArrowButton(bool add);
};

class MiniCalendarData : public WidgetData {
    friend class MiniCalendar;
private:
    ViewLevel viewLevel;
    QDate topLeft;
    QDate titleDate;
    int mark1, mark2, mark3;
    int firstVal;
private:
    MiniCalendarData();
    void setViewLevel(ViewLevel level);
    void setTopLeft(const QDate& d);
    void ensureTopLeft(const QDate& date);
};

class ContentLayer : public SlotsPainterLayer {
private:
    int firstVal;
    int mark1, mark2, mark3, mark4;
    int val, count;
    int rx, ry;
    ViewLevel viewLevel;
public:
    ContentLayer();
    void setVal(ViewLevel viewLevel, int firstVal, int mark1, int mark2, int mark3);
protected:
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
    void mousePressed(int column, int row) override;
};

class TitleLayer : public SlotsPainterLayer {
protected:
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
};

#endif //LIFERHYTHM_CALENDAR_H
