#include <QDebug>
#include <RcManagers.h>
#include "LifeRhythm.h"
#include "SlotsPainter.h"
#include <QHBoxLayout>
#include "ListWidget.h"
#include "WidgetFactory.h"
#include "Calendar.h"
#include "InputTextLabel.h"
#include "CalendarView.h"

USING_NAMESPACE(lr)

class TestFeedBack : public Message {
public:
    explicit TestFeedBack(Identifier loc): Message(std::move(loc)) {
    }
};

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([] {
        auto f = WidgetFactoryStorage::get("lr:widget_calendar");
        regClazz(f, SlotsPainter);
        regClazz(f, InputTextLabel);
        regClazz(f, Calendar);
    });
    lr.onPostInit([] {
        auto parent = new QWidget;
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        CalendarBuilder builder(parent);
        auto v = new CalendarView;
        builder.setView(v);

        layout->addWidget(builder.get());
        parent->show();

        auto d = new CalendarData("test");
        LifeRhythm::get()->getMessageHandler().registerListener(d->getSenderLoc(), [d](auto, auto) {
            LifeRhythm::get()->getMessageHandler().sendMessage(new TestFeedBack(d->getReceiverLoc()));
            return MessageListener::SUCCESS;
        });
        v->setData(d);
        v->loadCalendar(QDate::currentDate());
    });
    return lr.launch();
}
