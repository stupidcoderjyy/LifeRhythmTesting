//
// Created by stupid_coder_jyy on 24-11-8.
//

#ifndef CALENDAR_H
#define CALENDAR_H
#include <Namespaces.h>
#include <WidgetData.h>
#include "MiniCalendar.h"

BEGIN_NAMESPACE(lr)

BEGIN_NAMESPACE(calender)

enum ViewType {
    Month, D1, D2, D3, D4, D5, D6, D7
};

END_NAMESPACE

class CalendarData : public WidgetData {
    using ViewType = calender::ViewType;
public:
    ViewType viewType;
    QDate dateStart;
public:
    CalendarData();
};

BEGIN_NAMESPACE(calendar)

class LayerDay;

class DropDownMiniCalendar : public MiniCalendar {
    using ViewType = calender::ViewType;
private:
    LayerDay* layerDay;
    QDate dateStart;
    ViewType viewType;
public:
    explicit DropDownMiniCalendar(QWidget* parent = nullptr, bool iic = true);
    void syncDataToWidget() override;
    void syncWidgetToData() override;
protected:
    void initWidget() override;
    void syncWidget() override;
};

class LayerDay : public SlotsPainterLayer {
    using ViewType = calender::ViewType;
private:
    int len;
    int count;
    int hb, he;
    int sb, se;
public:
    LayerDay();
    void set(int len, int sb);
private:
    void mouseEntered(int column, int row) override;
    void mouseLeaved() override;
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
};

END_NAMESPACE

END_NAMESPACE

#endif //CALENDAR_H
