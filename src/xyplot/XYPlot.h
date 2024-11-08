//
// Created by JYY on 24-11-4.
//

#ifndef XYPLOT_H
#define XYPLOT_H

#include <Widget.h>

class XYPlot final : public Widget {
    friend class XYPlotBuilder;
private:
    const static QMargins MARGINS;
    QVector<std::function<double(double)>> functions;
    QVector<std::function<double(double)>> equations;
    QVector<QVector<QPointF>> points;
    std::function<QString(double)> xCoordHint;
    std::function<QString(double)> yCoordHint;
    double spacing;
    double xBegin, xEnd;
    double minY, maxY;
    int xSlots, ySlots;
public:
    void outputImage(const QString &path);
protected:
    XYPlot();
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    void drawPlot(QPainter& painter);
};

class XYPlotBuilder {
private:
    XYPlot* plot;
public:
    XYPlotBuilder();
    XYPlotBuilder& setRange(double xBegin, double xEnd);
    XYPlotBuilder& limitY(double minY, double maxY);
    XYPlotBuilder& setSlots(int xSlots, int ySlots);
    XYPlotBuilder& setXCoordHint(std::function<QString(double)> func);
    XYPlotBuilder& setyCoordHint(std::function<QString(double)> func);
    XYPlotBuilder& setSpacing(double spacing);
    XYPlotBuilder& addFunction(const std::function<double(double)>& func);
    XYPlotBuilder& addEquation(const std::function<double(double)>& eq);
    XYPlot* get() const;
};

#endif //XYPLOT_H
