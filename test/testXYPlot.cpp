#include <cmath>
#include <qsizepolicy.h>
#include <QWidget>
#include <LifeRhythm.h>
#include <valarray>

#include "XYPlot.h"

USING_NAMESPACE(lr)

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto builder = XYPlotBuilder().setRange(0, 4 * M_PI).setSlots(10,10);
        builder.addFunction([](auto x) {
            return std::sin(x * x);
        }).addFunction([](auto x) {
            return std::sin(x);
        });

        auto plot = builder.get();
        plot->setFixedSize(1000, 500);
        plot->show();
        plot->outputImage(R"(C:\Users\JYY\Desktop\test.png)");
    });
    return lr.launch();
}
