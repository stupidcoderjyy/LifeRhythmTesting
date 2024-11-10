//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef LIFERHYTHM_SLOTSDRAWER_H
#define LIFERHYTHM_SLOTSDRAWER_H

#include "Widget.h"
#include "Error.h"

class SlotsPainterLayer;

class SlotsPainter : public Widget {
public:
    enum SlotSizePolicy {
        Fixed,
        Auto
    };
protected:
    int slotWidth, slotHeight;
    int columns, rows;
    bool running;
    SlotSizePolicy vSlotSizePolicy, hSlotSizePolicy;
private:
    QVector<SlotsPainterLayer*> layers;
public:
    explicit SlotsPainter(QWidget* parent = nullptr);
    void appendLayer(SlotsPainterLayer* layer);
    void insertLayer(int i, SlotsPainterLayer* layer);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    inline void setSlotSize(int width, int height);
    inline void setSlotCount(int columns, int rows);
    inline void setSlotSizePolicy(SlotSizePolicy horizontal, SlotSizePolicy vertical);
    ~SlotsPainter() override;
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    virtual void onSlotClicked(QMouseEvent* evt, int row, int column);
private:
    void updateBase();
};

inline void SlotsPainter::setSlotSize(int width, int height) {
    if (width <= 0 || height <= 0) {
        throwInFunc("invalid size");
    }
    slotWidth = width;
    slotHeight = height;
    updateBase();
}

inline void SlotsPainter::setSlotCount(int c, int r) {
    columns = c;
    rows = r;
    updateBase();
}

inline void SlotsPainter::setSlotSizePolicy(SlotSizePolicy horizontal, SlotSizePolicy vertical) {
    vSlotSizePolicy = vertical;
    hSlotSizePolicy = horizontal;
    updateBase();
}

class SlotsPainterLayer {
    friend class SlotsPainter;
protected:
    SlotsPainter* parent;
public:
    SlotsPainterLayer();
    virtual ~SlotsPainterLayer();
protected:
    virtual bool shouldDraw();
    virtual void beforeDrawing(QPainter& p);
    virtual void drawSlot(QPainter& p, QRect& area, int row, int column);
    virtual void afterDrawing(QPainter& p);
};

#endif //LIFERHYTHM_SLOTSDRAWER_H
