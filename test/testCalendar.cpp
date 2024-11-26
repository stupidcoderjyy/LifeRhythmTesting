#include <RcManagers.h>
#include "LifeRhythm.h"
#include "SlotsPainter.h"
#include "ListWidget.h"
#include "Calendar.h"
#include "DropDown.h"

USING_NAMESPACE(lr)
USING_NAMESPACE(lr::calender)

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([] {
        auto f = WidgetFactoryStorage::get("test:widget_mini_calendar");
        regClazz(f, Button);
        regClazz(f, ArrowButton);
        Button::mainInit();
        f = WidgetFactoryStorage::get("test:widget_calendar");
        regClazz(f, calendar::DropDownMiniCalendar);
        regClazz(f, DropDown);
    });
    lr.onPostInit([] {
        auto parent = new QWidget;
        parent->setObjectName("parent");
        parent->setStyleSheet(qss_target("#parent", bg(Styles::BLACK->rgbHex)));
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        auto* dp = WidgetFactoryStorage::get("test:widget_calendar")->applyAndCast<DropDown>();
        auto cd = new CalendarData;
        auto mc = dp->getPointer<calendar::DropDownMiniCalendar>("c");
        mc->setData(cd);
        mc->syncDataToWidget();

        layout->addWidget(dp);

        QTimer::singleShot(0, [parent] {
            parent->show();
        });
    });
    return lr.launch();
}
