#include <qthread.h>
#include <RcManagers.h>
#include "LifeRhythm.h"
#include "SlotsPainter.h"
#include "ListWidget.h"
#include "Calendar.h"
#include "DropDown.h"

USING_NAMESPACE(lr)
USING_NAMESPACE(lr::calendar)

int count = 0;

class ItemTest : public ListItemCalendar {
public:
    void paintEvent(QPaintEvent *event) override {
        if (wData) {
            QPainter painter(this);
            painter.fillRect(event->rect().adjusted(1, 1, -1, -1), Styles::CYAN_0->color);
        }
        ListItemCalendar::paintEvent(event);
    }
};

class TestData : public WidgetData {
public:
    ~TestData() override {
        count--;
        qDebug() << "delete! count:" << count;
    }
};

class TestCalendarData : public CalendarData {
protected:
    WidgetData *getData(const QDate &date) const override {
        if (date.dayOfWeek() > Qt::Friday) {
            QThread::msleep(1000);
            count++;
            qDebug() << "new!   count:" << count;
            return new TestData;
        }
        return nullptr;
    }
};

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onMainInit([] {
        Button::mainInit();
        Calendar::mainInit();
    });
    lr.onPostInit([] {
        auto parent = new QWidget;
        parent->setObjectName("parent");
        parent->setStyleSheet(qss_target("#parent", bg(Styles::GRAY_0->rgbHex)));
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);
        auto* c = new Calendar;
        c->setItemBuilder([] {
            return new ItemTest;
        });
        c->setData(new TestCalendarData);
        layout->addWidget(c);
        QTimer::singleShot(0, [parent] {
            parent->show();
        });
    });
    return lr.launch();
}
