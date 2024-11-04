#include <QDebug>
\
#include "LifeRhythm.h"
#include "SlotsWidget.h"

USING_NAMESPACE(lr)

#include <QHBoxLayout>
#include "ListWidget.h"
#include "TiledListWidget.h"
#include <QDebug>

USING_NAMESPACE(lr)

class TestData : public WidgetData {
public:
    int data;
    explicit TestData(int data) : data(data) {}
};

class TestItem : public ListItem {
private:
    int i;
    int d;
public:
    explicit TestItem(int idx, QWidget *parent) : ListItem(parent), i(idx), d(-1) {
    }

    void syncDataToWidget() override {
        if (wData) {
            d = wData->cast<TestData>()->data;
        } else {
            d = -1;
        }
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.fillRect(rect(), Styles::GRAY_0->color);
        painter.setFont(Styles::FONT_MAIN);
        painter.setPen(Styles::GRAY_TEXT_0->color);
        painter.drawText(rect(), QString::asprintf("i:%d\nd:%d", i, d), QTextOption(Qt::AlignCenter));
    }
};

class Slots : public SlotsWidget {
public:
    explicit Slots(QWidget *parent) : SlotsWidget(parent) {}

    ListItem* newItem() override {
        return new TestItem(items.length(), this);
    }
};

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto d = new ListData;
        for (int i = 0; i < 12; i++) {
            d->append(new TestData(i));
        }

        auto parent = new QWidget;
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        auto sw = new Slots(parent);
        sw->setColumns(5);
        sw->setRows(3);
        sw->setMinimumSize(200, 200);
        sw->setSizePolicy(QSizePolicy:: Minimum, QSizePolicy::Minimum);
        sw->setData(d);
        layout->addWidget(sw);
        parent->show();
    });
    return lr.launch();
}
