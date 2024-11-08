#include <QDebug>
\
#include "LifeRhythm.h"
#include "SlotsPainter.h"

USING_NAMESPACE(lr)

#include <QHBoxLayout>
#include "ListWidget.h"
#include "TiledListWidget.h"
#include <QDebug>

class TestLayer : public SlotsPainterLayer {
protected:
    void beforeDrawing(QPainter &p) override {
        p.setFont(Styles::FONT_LARGE);
        p.setPen(Styles::GRAY_TEXT_0->color);
    }

    void drawSlot(QPainter &p, QRect &area, int row, int column) override {
        p.fillRect(area, Styles::BLACK->color);
        p.drawText(area, QString::number(row) + "," + QString::number(column), QTextOption(Qt::AlignCenter));
    }
};

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
        sw->setColumns(5);
        sw->setRows(3);
        sw->setMinimumSize(200, 200);
        sw->setSizePolicy(QSizePolicy:: Minimum, QSizePolicy::Minimum);
        sw->appendLayer(new TestLayer);
        layout->addWidget(sw);
        parent->show();
    });
    return lr.launch();
}
