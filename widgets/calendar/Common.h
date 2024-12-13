//
// Created by JYY on 24-12-13.
//

#ifndef CALENDARINCLUDES_H
#define CALENDARINCLUDES_H

#include <Namespaces.h>
#include <QDate>

BEGIN_NAMESPACE(lr)

class ListItemCalendar;
class Calendar;
class CalendarData;

BEGIN_NAMESPACE(calendar)

enum ViewType {
    Month, D1, D2, D3, D4, D5, D6, D7
};

class LayerDay;
class LayerMonth;
class MiniCalendarDropDown;
class SlotsWidgetCalendar;

class ListDataCalendar : public ListData {
public:
    QDate topLeft;
    ViewType viewType;
};

END_NAMESPACE

END_NAMESPACE

#endif //CALENDARINCLUDES_H
