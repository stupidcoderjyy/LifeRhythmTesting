
#include <qboxlayout.h>

#include "MiniCalendar.h"
#include "LifeRhythm.h"

USING_NAMESPACE(lr)

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto parent = new QWidget;
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);
        auto d = new MiniCalendar(new WeekDayTitleDrawer, new CalendarContentDrawer, parent);
        d->setData(new MiniCalendarData);
        layout->addWidget(d);
        parent->show();
        d->loadDate(QDate::currentDate());
    });
    return lr.launch();
}
