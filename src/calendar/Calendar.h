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

class DropDownMiniCalendar : public MiniCalendar {

};

END_NAMESPACE

class CalendarData : public WidgetData {
    using ViewType = calender::ViewType;
public:
    ViewType viewType;
    QDate startDate;
};

END_NAMESPACE

#endif //CALENDAR_H
