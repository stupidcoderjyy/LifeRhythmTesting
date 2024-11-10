//
// Created by JYY on 24-11-4.
//

#ifndef SLOTSWIDGET_H
#define SLOTSWIDGET_H

#include <ListWidget.h>
#include <Widget.h>
#include <Error.h>

class SlotsWidget : public Widget {
public:
    enum SlotSizePolicy {
        Fixed,
        Auto
    };
protected:
    QVector<ListItem*> items;
    int slotWidth, slotHeight;
    int columns, rows;
    bool running;
    SlotSizePolicy vSlotSizePolicy, hSlotSizePolicy;
public:
    explicit SlotsWidget(QWidget *parent = nullptr);
    inline void setSlotSize(int width, int height);
    inline void setSlotCount(int columns, int rows);
    inline void setSlotSizePolicy(SlotSizePolicy horizontal, SlotSizePolicy vertical);
protected:
    virtual void prepareItem(ListItem* w);
    void syncItems(int begin, int end) const;
    virtual void syncItem(int idx) const;
    virtual ListItem* newItem();
    void resizeEvent(QResizeEvent* event) override;
    void connectModelView() override;
private:
    void updateBase();
};

inline void SlotsWidget::setSlotSize(int width, int height) {
    if (width <= 0 || height <= 0) {
        throwInFunc("invalid size");
    }
    slotWidth = width;
    slotHeight = height;
    updateBase();
}

inline void SlotsWidget::setSlotCount(int c, int r) {
    columns = c;
    rows = r;
    updateBase();
}

inline void SlotsWidget::setSlotSizePolicy(SlotSizePolicy horizontal, SlotSizePolicy vertical) {
    vSlotSizePolicy = vertical;
    hSlotSizePolicy = horizontal;
    updateBase();
}

#endif //SLOTSWIDGET_H
