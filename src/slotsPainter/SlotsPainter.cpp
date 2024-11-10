//
// Created by stupid_coder_jyy on 2024/4/5.
//

#include "SlotsPainter.h"

#include <NBT.h>

#include "MemUtil.h"
#include <QMouseEvent>

SlotsPainter::SlotsPainter(QWidget *parent): Widget(parent), slotWidth(), running(false),
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

void SlotsPainter::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    if (widgetTag->contains("slots_count", Data::ARR)) {
        int slotCounts[2];
        widgetTag->get("slots_count")->asArray()->fillInt(slotCounts, 2);
        handlers << [slotCounts](QWidget* t) {
            auto painter = static_cast<SlotsPainter*>(t);
            painter->setSlotCount(slotCounts[0], slotCounts[1]);
        };
    }
    if (widgetTag->contains("slots_size", Data::ARR)) {
        int slotsSize[2];
        widgetTag->get("slots_size")->asArray()->fillInt(slotsSize, 2);
        handlers << [slotsSize](QWidget* t) {
            auto painter = static_cast<SlotsPainter*>(t);
            painter->setSlotSize(slotsSize[0], slotsSize[1]);
        };
    }
    if (widgetTag->contains("slots_size_policy", Data::ARR)) {
        QStringList list;
        widgetTag->get("slots_size_policy")->asArray()->fillString(list, 2);
        SlotSizePolicy policy[2];
        for (int i = 0; i < 2; i++) {
            if (list[i] == "Auto") {
                policy[i] = Auto;
            } else if (list[i] == "Fixed") {
                policy[i] = Fixed;
            }
        }
        handlers << [policy](QWidget* t) {
            auto painter = static_cast<SlotsPainter*>(t);
            painter->setSlotSizePolicy(policy[0], policy[1]);
        };
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
