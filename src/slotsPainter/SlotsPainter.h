//
// Created by stupid_coder_jyy on 2024/4/5.
//

#ifndef LIFERHYTHM_SLOTSDRAWER_H
#define LIFERHYTHM_SLOTSDRAWER_H

#include "Widget.h"
#include "Error.h"

class SlotsPainterLayer;

class SlotsPainter : public Widget {
    Q_OBJECT
public:
    enum SlotSizePolicy {
        Fixed,
        Auto
    };
protected:
    int slotWidth, slotHeight;
    int columns, rows;
    SlotSizePolicy vSlotSizePolicy, hSlotSizePolicy;
private:
    QVector<SlotsPainterLayer*> layers;
public:
    explicit SlotsPainter(QWidget* parent = nullptr, bool initInConstructor = true);
    void appendLayer(SlotsPainterLayer* layer);
    void insertLayer(int i, SlotsPainterLayer* layer);
    void removeLayer(SlotsPainterLayer* layer);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    inline void setSlotSize(int width, int height);
    inline void setSlotCount(int columns, int rows);
    inline void setSlotSizePolicy(SlotSizePolicy horizontal, SlotSizePolicy vertical);
    ~SlotsPainter() override;
signals:
    void sigPressSlot(int column, int row);
    void sigReleaseSlot(int column, int row);
    void sigScroll(int dy);
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
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
    virtual void drawSlot(QPainter& p, QRect& area, int column, int row);
    virtual void afterDrawing(QPainter& p);
    virtual void mousePressed(int column, int row);
    virtual void mouseReleased(int column, int row);
};

#endif //LIFERHYTHM_SLOTSDRAWER_H
