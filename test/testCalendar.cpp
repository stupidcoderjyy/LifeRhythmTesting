#include <QDebug>
\
#include "LifeRhythm.h"
#include "SlotsPainter.h"

USING_NAMESPACE(lr)

#include <QHBoxLayout>
#include "ListWidget.h"
#include "TiledListWidget.h"
#include <QDebug>
#include "Calendar.h"

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto parent = new QWidget;
        auto layout = new QVBoxLayout(parent);
        parent->setLayout(layout);

        auto sw = new SlotsPainter(parent);
        sw->setColumns(7);
        sw->setRows(1);
        sw->setMinimumWidth(200);
        sw->setFixedHeight(40);
        sw->setSizePolicy(QSizePolicy:: Minimum, QSizePolicy::Minimum);
        sw->appendLayer(new WeekDaysTitleLayer);
        layout->addWidget(sw);
        parent->show();
    });
    return lr.launch();
}
