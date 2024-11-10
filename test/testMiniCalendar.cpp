
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
    });
    return lr.launch();
}
