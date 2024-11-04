//
// Created by JYY on 24-11-4.
//

#ifndef XYPLOT_H
#define XYPLOT_H

#include <Widget.h>

class XYPlot : public Widget {
    friend class XYPlotBuilder;
private:
    const static QMargins MARGINS;
    const static double SPACING;
    QVector<std::function<double(double)>> functions;
    QVector<QVector<QPointF>> points;
    double xBegin, xEnd;
    double minY, maxY;
    int xSlots, ySlots;
public:
    void outputImage(const QString &path);
protected:
    XYPlot();
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    virtual QString xCoordHint(double value);
    virtual QString yCoordHint(double value);
private:
    void drawPlot(QPainter& painter);
};

class XYPlotBuilder {
private:
    XYPlot* plot;
public:
    XYPlotBuilder();
    XYPlotBuilder& setRange(double xBegin, double xEnd);
    XYPlotBuilder& setSlots(int xSlots, int ySlots);
    XYPlotBuilder& addFunction(const std::function<double(double)>& func);
    XYPlot* get() const;
};

#endif //XYPLOT_H
