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
    inline void setColumns(int count);
    inline void setRows(int count);
    inline void setSlotSizePolicy(SlotSizePolicy vertical, SlotSizePolicy horizontal);
protected:
    void prepareItem(ListItem* w);
    void onDataChanged(int begin, int end) const;
    void syncItem(int idx) const;
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

inline void SlotsWidget::setColumns(int count) {
    columns = count;
    updateBase();
}

inline void SlotsWidget::setRows(int count) {
    rows = count;
    updateBase();
}

inline void SlotsWidget::setSlotSizePolicy(SlotSizePolicy vertical, SlotSizePolicy horizontal) {
    vSlotSizePolicy = vertical;
    hSlotSizePolicy = horizontal;
    updateBase();
}

#endif //SLOTSWIDGET_H
