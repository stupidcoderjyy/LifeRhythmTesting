//
// Created by JYY on 24-12-13.
//

#ifndef SLOTSWIDGETCALENDAR_H
#define SLOTSWIDGETCALENDAR_H

#include <Namespaces.h>
#include <SlotsWidget.h>
#include <Common.h>

BEGIN_NAMESPACE(lr::calendar)

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
    void updateBase() override;
};

inline void SlotsWidgetCalendar::setItemBuilder(std::function<ListItemCalendar*()> ib) {
    itemBuilder = std::move(ib);
}

END_NAMESPACE

#endif //SLOTSWIDGETCALENDAR_H
