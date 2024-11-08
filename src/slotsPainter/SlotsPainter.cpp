//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "SlotsPainter.h"
#include "MemUtil.h"
#include <QMouseEvent>

SlotsPainter::SlotsPainter(QWidget *parent): Widget(parent), layers(), slotWidth(), running(false),
        slotHeight(), columns(), rows(), hSlotSizePolicy(Auto), vSlotSizePolicy(Auto) {
}

void SlotsPainter::appendLayer(SlotsPainterLayer *layer) {
    layer->parent = this;
    layers << layer;
    if (running) {
        update();
    }
}

void SlotsPainter::insertLayer(int i, SlotsPainterLayer *layer) {
    layer->parent = this;
    layers.insert(i, layer);
    if (running) {
        update();
    }
}

SlotsPainter::~SlotsPainter() {
    DELETE_LIST(layers)
}

void SlotsPainter::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    QRect area;
    for (auto* l : layers) {
        if (!l->shouldDraw()) {
            continue;
        }
        area.setRect(0, 0, slotWidth, slotHeight);
        l->beforeDrawing(p);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                l->drawSlot(p, area, r, c);
                area.translate(slotWidth, 0);
            }
            area.translate(-area.x(), slotHeight);
        }
        l->afterDrawing(p);
    }
}

void SlotsPainter::resizeEvent(QResizeEvent *event) {
    Widget::resizeEvent(event);
    running = true;
    updateBase();
}

void SlotsPainter::mousePressEvent(QMouseEvent *event) {
    auto p = event->pos();
    onSlotClicked(event, p.y() / slotHeight, p.x() / slotWidth);
}

void SlotsPainter::onSlotClicked(QMouseEvent* evt, int row, int column) {
}

void SlotsPainter::updateBase() {
    if (!running) {
        return;
    }
    if (vSlotSizePolicy == Auto) {
        slotHeight = height() / rows;
    }
    if (hSlotSizePolicy == Auto) {
        slotWidth = width() / columns;
    }
}

SlotsPainterLayer::SlotsPainterLayer(): parent() {
}

SlotsPainterLayer::~SlotsPainterLayer() = default;

bool SlotsPainterLayer::shouldDraw() {
    return true;
}

void SlotsPainterLayer::beforeDrawing(QPainter &p) {
}

void SlotsPainterLayer::drawSlot(QPainter &p, QRect &area, int row, int column) {
}

void SlotsPainterLayer::afterDrawing(QPainter &p) {
}
