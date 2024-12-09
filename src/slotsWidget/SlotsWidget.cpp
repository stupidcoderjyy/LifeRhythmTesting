//
// Created by JYY on 24-11-4.
//

#include "SlotsWidget.h"
#include "NBT.h"
#include "WidgetFactory.h"

SlotsWidget::SlotsWidget(QWidget *parent): Widget(parent), slotWidth(50), slotHeight(50), columns(0), rows(0),
        vSlotSizePolicy(Auto), hSlotSizePolicy(Auto), factoryItem() {
}

void SlotsWidget::setData(WidgetData *d) {
    if (!prepared) {
        updateBase();
    }
    Widget::setData(d);
    syncItems(0, INT32_MAX);    //update all
}

void SlotsWidget::prepareItem(ListItem *w) {
    w->setParent(this);
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    w->initWidget();
    w->show();
    items << w;
}

void SlotsWidget::syncItem(int idx) const {
    auto item = items[idx];
    auto ld = wData->cast<ListData>();
    auto d = ld->at(idx);
    item->setData(d);
    item->dataIdx = idx;
    item->setList(ld);
    item->syncDataToWidget();
}

void SlotsWidget::syncItems(int begin, int end) const {
    int rBorder = rows * columns;
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
    if (factoryItem) {
        return factoryItem->applyAndCast<ListItem>();
    }
    return new ListItem();
}

void SlotsWidget::resizeEvent(QResizeEvent *event) {
    Widget::resizeEvent(event);
    updateBase();
}

void SlotsWidget::connectModelView() {
    dc << connect(wData, &WidgetData::sigDataChanged, this, [this] {
        if (!prepared) {
            updateBase();
        }
        auto* d = wData->cast<ListData>();
        syncItems(d->getChangeBegin(), d->getChangeEnd());
    });
}

void SlotsWidget::onPostParsing(Handlers &handlers, NBT *nbt) {
    if (nbt->contains("item", Data::COMPOUND)) {
        auto *f = WidgetFactory::fromNbt("item", nbt->get("item")->asCompound());
        f->include(WidgetFactory::factoryInParse());
        try {
            f->parse();
        } catch (Error &) {
            delete f;
            throw;
        }
        handlers << [f](QWidget *widget) {
            auto *l = static_cast<SlotsWidget*>(widget);
            l->factoryItem = f;
        };
    }
}

void SlotsWidget::updateBase() {
    prepared = true;
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
}
