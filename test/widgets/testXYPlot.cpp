#include <cmath>
#include <qsizepolicy.h>
#include <QDebug>
#include <LifeRhythm.h>
#include <valarray>

#include "XYPlot.h"

USING_NAMESPACE(lr)

auto func1 = [](double beta, double lambda, double k) {
    double sinBeta2 = pow(sin(beta), 2);
    double tanBeta = tan(beta);
    double tanLambda = tan(lambda);
    double top = 1 + tanBeta * tanLambda;
    double bottom = sinBeta2 - (k + 1 / 2 - sinBeta2) * tanBeta * tanLambda;
    return pow(top / bottom, 0.5);
};

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto builder = XYPlotBuilder()
                .setRange(20 / 180.0 * M_PI, 80 / 180.0 * M_PI)
                .setSlots(10,20)
                .limitY(1, 4)
                .setSpacing(0.001)
                .setXCoordHint([](auto val) {
                    return QString::number(round(val / M_PI * 180.0));
                });
        double k = 1.4;
        for (int l = 0; l <= 35; l += 5) {
            double lambda = l / 180.0 * M_PI;
            builder.addFunction([lambda, k](auto beta) {
                return func1(beta, lambda, k);
            });
        }

        auto plot = builder.get();
        plot->setFixedSize(500, 500);
        plot->show();
        plot->outputImage(R"(C:\Users\JYY\Desktop\test.png)");
    });
    return lr.launch();
}