//
// Created by stupid_coder_jyy on 24-11-8.
//

#ifndef CALENDAR_H
#define CALENDAR_H
#include <ListData.h>
#include <ListWidget.h>
#include <Namespaces.h>
#include <RcManagers.h>
#include <WidgetData.h>

#include "DropDown.h"
#include "MiniCalendar.h"
#include "SlotsWidget.h"

BEGIN_NAMESPACE(lr)
BEGIN_NAMESPACE(calendar)

enum ViewType {
    Month, D1, D2, D3, D4, D5, D6, D7
};

class MiniCalendarDropDown;
class SlotsWidgetCalendar;
class ListDataCalendar;

END_NAMESPACE

class ListItemCalendar;

class CalendarData : public WidgetData {
    friend class Calendar;
    using ViewType = calendar::ViewType;
private:
    ViewType viewType;
    ViewType prevType;
    QDate dateStart;
public:
    CalendarData();
    void set(ViewType viewType, QDate dateStart);
    inline ViewType getViewType() const;
    inline ViewType getPrevType() const;
    inline QDate getDateStart() const;
protected:
    virtual WidgetData* getData(const QDate& date) const;
private:
    void setTopLeft(calendar::ListDataCalendar* ldc) const;
    void gatherData(calendar::ListDataCalendar* ldc) const;
};

inline CalendarData::ViewType CalendarData::getViewType() const {
    return viewType;
}

inline CalendarData::ViewType CalendarData::getPrevType() const {
    return prevType;
}

inline QDate CalendarData::getDateStart() const {
    return dateStart;
}

class Calendar : public Widget {
private:
    DropDown* dropdownMiniCalendar;
    DropDown* dropdownRange;
    Button* btnPrev;
    Button* btnNext;
    Button* btnWeek;
    Button* btnMonth;
    calendar::MiniCalendarDropDown* miniCalendar;
    Label* labelRange;
    Label* labelDate;
    calendar::SlotsWidgetCalendar* slotsContent;
    std::function<ListItemCalendar*()> itemBuilder;
public:
    static void mainInit();
    explicit Calendar(QWidget* parent = nullptr, bool iic = true);
    void setData(WidgetData *d) override;
    void setItemBuilder(const Identifier& factoryLoc);
    void setItemBuilder(const std::function<ListItemCalendar*()>& itemBuilder);
    void syncDataToWidget() override;
    void connectModelView() override;
    void initWidget() override;
private:
    void refreshContent(CalendarData* cd) const;
    void setOperationEnabled(bool e) const;
};

class ListItemCalendar : public ListItem {
    friend class calendar::SlotsWidgetCalendar;
private:
    calendar::SlotsWidgetCalendar* sw;
    int itemIdx;
    QColor iconColor;
    QFont iconFont;
    int iconNum;
public:
    explicit ListItemCalendar(QWidget* parent = nullptr, bool iic = true);
protected:
    void syncDataToWidget() override;
    void paintEvent(QPaintEvent *event) override;
};

BEGIN_NAMESPACE(calendar)

class LayerDay;
class LayerMonth;

class MiniCalendarDropDown : public MiniCalendar {
private:
    LayerDay* layerDay;
    LayerMonth* layerMonth;
    QDate dateStart;
    ViewType viewType;
    bool isViewTypeMonth;
public:
    explicit MiniCalendarDropDown(QWidget* parent = nullptr, bool iic = true);
    void syncDataToWidget() override;
    void syncWidgetToData() override;
protected:
    void initWidget() override;
    void syncWidget() override;
};

class LayerDay : public SlotsPainterLayer {
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

class LayerMonth : public SlotsPainterLayer {
private:
    int hovered;
    int selected;
    int count;
    bool isViewTypeMonth;
public:
    LayerMonth();
    void set(bool isViewTypeMonth, int s);
    void mouseEntered(int column, int row) override;
    void mouseLeaved() override;
    bool shouldDraw() override;
    void beforeDrawing(QPainter &p) override;
    void drawSlot(QPainter &p, QRect &area, int column, int row) override;
};

class DataRange : public WidgetData {
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

class DropDownRange : public DropDown {
private:
    Label* label;
    ListWidget* list;
    ViewType viewType;
    ListData* optionsData;
public:
    explicit DropDownRange(QWidget* parent = nullptr, bool iic = true);
    void syncWidgetToData() override;
    void initWidget() override;
    ~DropDownRange() override;
};

class ButtonSwitchView : public Button {
public:
    explicit ButtonSwitchView(QWidget* parent = nullptr, bool iic = true);
protected:
    void handleButtonActivate(QMouseEvent *ev) override;
};

class SlotsWidgetCalendar : public SlotsWidget {
    friend class lr::ListItemCalendar;
    friend class lr::Calendar;
private:
    std::function<ListItemCalendar*()> itemBuilder;
    CalendarData *cd;
public:
    explicit SlotsWidgetCalendar(QWidget *parent = nullptr, bool iic = true);
    ~SlotsWidgetCalendar() override;
    inline void setItemBuilder(std::function<ListItemCalendar*()> itemBuilder);
protected:
    ListItem *newItem() override;
    void paintEvent(QPaintEvent *event) override;
};

inline void SlotsWidgetCalendar::setItemBuilder(std::function<ListItemCalendar*()> ib) {
    itemBuilder = std::move(ib);
}

class ListDataCalendar : public ListData {
public:
    QDate topLeft;
    ViewType viewType;
};

END_NAMESPACE

END_NAMESPACE

#endif //CALENDAR_H
