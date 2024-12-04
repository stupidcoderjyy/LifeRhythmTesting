#include <RcManagers.h>
#include "LifeRhythm.h"
#include "SlotsPainter.h"
#include "ListWidget.h"
#include "Calendar.h"
#include "DropDown.h"

USING_NAMESPACE(lr)
USING_NAMESPACE(lr::calendar)

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([] {
        Button::mainInit();
        auto f = WidgetFactoryStorage::get("test:widget_mini_calendar");
        regClazz(f, Widget);
        regClazz(f, Button);
        regClazz(f, ArrowButton);
        f = WidgetFactoryStorage::get("test:calendar/widget_calendar");
        regClazz(f, Widget);
        regClazz(f, calendar::MiniCalendarDropDown);
        regClazz(f, DropDown);
        regClazz(f, Calendar);
        regClazz(f, ArrowButton);
        regClazz(f, Button);
        regClazz(f, ButtonSwitchView);
        f = WidgetFactoryStorage::get("test:calendar/dropdown_range");
        regClazz(f, Widget);
        regClazz(f, calendar::DropDownRange);
        regClazz(f, Label);
        regClazz(f, ArrowButton);
        regClazz(f, ListWidget);
        regClazz(f, calendar::ItemRange);
        f = WidgetFactoryStorage::get("test:calendar/dropdown_mini_calendar");
        regClazz(f, Widget);
        regClazz(f, calendar::MiniCalendarDropDown);
        regClazz(f, DropDown);
        regClazz(f, Label);
    });
    lr.onPostInit([] {
        auto parent = new QWidget;
        parent->setObjectName("parent");
        parent->setStyleSheet(qss_target("#parent", bg(Styles::GRAY_0->rgbHex)));
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        auto* dp = WidgetFactoryStorage::get("test:calendar/widget_calendar")->applyAndCast<Calendar>();
        dp->setData(new CalendarData);

        layout->addWidget(dp);

        QTimer::singleShot(0, [parent] {
            parent->show();
        });
    });
    return lr.launch();
}
