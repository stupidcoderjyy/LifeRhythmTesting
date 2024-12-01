//
// Created by stupid_coder_jyy on 24-11-8.
//

#ifndef CALENDAR_H
#define CALENDAR_H
#include <ListData.h>
#include <ListWidget.h>
#include <Namespaces.h>
#include <WidgetData.h>

#include "DropDown.h"
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

class Calendar : public Widget {
public:
    explicit Calendar(QWidget* parent = nullptr, bool iic = true);
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

class DataRange : public WidgetData {
    using ViewType = calender::ViewType;
    friend class ItemRange;
private:
    int days;
public:
    explicit DataRange(int days);
};

class ItemRange : public ListItem {
private:
    Label* label;
public:
    explicit ItemRange(QWidget* parent = nullptr, bool iic = true);
    void initWidget() override;
    void syncDataToWidget() override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

class ListRange : public ListWidget {
    Q_OBJECT
public:
    explicit ListRange(QWidget* parent = nullptr, bool iic = true);
    ListItem *newItem() override;
};

class DropDownRange : public DropDown {
    using ViewType = calender::ViewType;
private:
    Label* label;
    ListRange* list;
    ViewType viewType;
public:
    explicit DropDownRange(QWidget* parent = nullptr, bool iic = true);
    void syncWidgetToData() override;
    void initWidget() override;
};

END_NAMESPACE

END_NAMESPACE

#endif //CALENDAR_H
