//
// Created by stupid_coder_jyy on 24-11-8.
//

#ifndef CALENDAR_H
#define CALENDAR_H
#include <Namespaces.h>
#include <WidgetData.h>
#include <QDate>

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
    QDate startDate;
};

BEGIN_NAMESPACE(calendar)

class DropDownMiniCalendar : public MiniCalendar {
public:
    explicit DropDownMiniCalendar(QWidget* parent = nullptr, bool iic = true);
protected:
    void initWidget() override;
};

class LayerDay : public SlotsPainterLayer {
    using ViewType = calender::ViewType;
private:
    ViewType viewType;
    int length;
    int count;
    int begin, end;
public:
    LayerDay();
protected:
    void mouseEntered(int column, int row) override;
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
};

END_NAMESPACE

END_NAMESPACE

#endif //CALENDAR_H
