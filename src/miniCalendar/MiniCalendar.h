//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef MINICALENDAR_H
#define MINICALENDAR_H

#include <qboxlayout.h>

#include "Widget.h"
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
    SlotsPainter* painterWeekdayTitle;
    SlotsPainter* painters[3];
private:
    TitleLayer *layerTitle;
    QVBoxLayout *bottom;
    ViewLevel viewLevel;
    ViewLevel maxViewLevel;
    QDate dateTopLeft;
    QDate dateTitle;
public:
    explicit MiniCalendar(QWidget* parent = nullptr, bool initInConstructor = true);
    void loadDate(const QDate& date);
    void setMaxViewLevel(ViewLevel level);
protected:
    void syncDataToWidget() override;
    void syncWidgetToData() override;
    void connectModelView() override;
    void initWidget() override;
private:
    void setViewLevel(ViewLevel levelNew);
    void updateTitle() const;
    void handleArrowButton(bool add);
    void handleButtonTitle();
    void handlePainterScroll(bool add);
    void ensureTopLeft();
};

class MiniCalendarData : public WidgetData {
    friend class MiniCalendar;
private:
    ViewLevel viewLevel;
    QDate topLeft;
    QDate dateTitle;
    int mark1, mark2, mark3;
    int firstVal;
private:
    MiniCalendarData();
    void set(ViewLevel level, const QDate& topLeft);
};

class ContentLayer : public SlotsPainterLayer {
private:
    int firstVal;
    int mark1, mark2, mark3, mark4;
    int val, count;
    int rx, ry;
    ViewLevel viewLevel;
    ViewLevel maxViewLevel;
public:
    ContentLayer();
    void setVal(ViewLevel maxViewLevel, ViewLevel viewLevel, int firstVal, int mark1, int mark2, int mark3);
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
