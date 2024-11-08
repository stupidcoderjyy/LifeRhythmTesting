//
// Created by JYY on 24-11-4.
//

#include "SlotsWidget.h"

SlotsWidget::SlotsWidget(QWidget *parent): Widget(parent), slotWidth(50), slotHeight(50), columns(0), rows(0), running(),
        vSlotSizePolicy(Auto), hSlotSizePolicy(Auto){
}

void SlotsWidget::prepareItem(ListItem *w) {
    w->setList(wData->cast<ListData>());
    w->setParent(this);
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    w->show();
    items << w;
}

void SlotsWidget::syncItem(int idx) const {
    auto item = items[idx];
    auto d = wData->cast<ListData>()->at(idx);
    item->setData(d);
    item->dataIdx = idx;
    item->syncDataToWidget();
}

void SlotsWidget::syncItems(int begin, int end) const {
    int rBorder = qMin(rows * columns, wData->cast<ListData>()->length());
    if (end < 0 || begin >= rBorder) {
        return;
    }
    begin = qMax(0, begin);
    end = qMin(rBorder - 1, end);
    while (begin <= end) {
        syncItem(begin++);
    }
}

ListItem * SlotsWidget::newItem() {
    return new ListItem();
}

void SlotsWidget::resizeEvent(QResizeEvent *event) {
    Widget::resizeEvent(event);
    running = true;
    updateBase();
}

void SlotsWidget::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this] {
        auto* d = wData->cast<ListData>();
        syncItems(d->getChangeBegin(), d->getChangeEnd());
    });
}

void SlotsWidget::updateBase() {
    if (!running) {
        return;
    }
    int oldSlotHeight = slotHeight, oldSlotWidth = slotWidth;
    if (vSlotSizePolicy == Auto) {
        slotHeight = height() / rows;
    }
    if (hSlotSizePolicy == Auto) {
        slotWidth = width() / columns;
    }
    if (oldSlotHeight == slotHeight && oldSlotWidth == slotWidth) {
        return;
    }
    int totalItems = rows * columns;
    items.reserve(totalItems);
    int oldItems = items.length();
    for (int i = oldItems ; i < totalItems ; i ++) {
        prepareItem(newItem());
    }
    int i = 0, x = 0, y = 0;
    for (int r = 0 ; r < rows ; r++, x = 0, y += slotHeight) {
        for (int c = 0; c < columns ; c++) {
            auto item = items[i++];
            item->setGeometry(x, y, slotWidth, slotHeight);
            item->setVisible(true);
            x += slotWidth;
        }
    }
    int rb = i, rn = items.length() - i;
    while (i < items.length()) {
        delete items[i++];
    }
    items.remove(rb, rn);
    syncItems(0, totalItems);
}
