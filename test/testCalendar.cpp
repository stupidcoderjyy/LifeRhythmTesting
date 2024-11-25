#include <RcManagers.h>
#include "LifeRhythm.h"
#include "SlotsPainter.h"
#include "ListWidget.h"
#include "Calendar.h"

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
    });
    lr.onPostInit([] {
        auto parent = new QWidget;
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        auto* c = new calendar::DropDownMiniCalendar(parent);

        layout->addWidget(c);

        parent->show();
    });
    return lr.launch();
}
