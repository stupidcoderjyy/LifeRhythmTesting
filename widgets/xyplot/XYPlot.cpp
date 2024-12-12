//
// Created by JYY on 24-11-4.
//

#include "XYPlot.h"

#include <math.h>
#include <QDebug>

const QMargins XYPlot::MARGINS = {50,50,50,50};

void XYPlot::outputImage(const QString &path) {
    auto img = new QPixmap(width(), height());
    {
        QPainter painter(img);
        drawPlot(painter);
    }
    img->save(path);
    delete img;
}

XYPlot::XYPlot(): spacing(0.01), xBegin(), xEnd(), minY(2.22507e-308), maxY(1.79769e+308), xSlots(), ySlots() {
    xCoordHint = [](double value) {
        return QString::asprintf("%.1f", value);
    };
    yCoordHint = xCoordHint;
}

void XYPlot::resizeEvent(QResizeEvent *event) {
    const int canvasW = width() - MARGINS.left() - MARGINS.right();
    const int canvasH = height() - MARGINS.bottom() - MARGINS.top();
    QVector<QVector<QPointF>> values;
    auto min = minY, max = maxY;
    for (const auto& func : functions) {
        double x = xBegin;
        auto child = QVector<QPointF>();
        while (x - xEnd < spacing) {
            double y = func(x);
            if (y > maxY || y < minY || isnan(y) || isinf(y)) {
                x += spacing;
                continue;
            }
            min = qMin(min, y);
            max = qMax(max, y);
            child << QPointF(x, y);
            x += spacing;
        }
        values << child;
    }
    double xRange = xEnd - xBegin;
    double yRange = maxY - minY;
    points.clear();
    for (const auto& child : values) {
        QVector<QPointF> line;
        for (int i = 0 ; i < child.length() ; i ++) {
            auto p = child.at(i);
            p.setX(MARGINS.left() + canvasW * (p.x() - xBegin) / xRange);
            p.setY(MARGINS.top() + canvasH * (1 - (p.y() - minY) / yRange));
            line << p;
        }
        points << line;
    }
}

void XYPlot::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    drawPlot(painter);
}

void XYPlot::drawPlot(QPainter& painter) {
    painter.fillRect(rect(), QColor(Qt::white));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Styles::GRAY_0->color, 2));
    for (const auto& child : points) {
        painter.drawPolyline(child.data(), child.length());
    }
    //grid
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(Styles::GRAY_4->color, 1));
    const int canvasW = width() - MARGINS.left() - MARGINS.right();
    const int canvasH = height() - MARGINS.bottom() - MARGINS.top();
    int y0 = MARGINS.top(), y1 = height() - MARGINS.bottom();
    int x0 = MARGINS.left(), x1 = width() - MARGINS.right();
    double sh = static_cast<double>(canvasH) / ySlots;
    double sw = static_cast<double>(canvasW) / xSlots;
    for (int i = 0 ; i <= xSlots ; i ++) {
        int x = qRound(MARGINS.left() + i * sw);
        painter.drawLine(x, y0, x, y1);
    }
    for (int i = 0 ; i <= ySlots ; i ++) {
        int y = qRound(MARGINS.top() + i * sh);
        painter.drawLine(x0, y, x1, y);
    }
    //text
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(Styles::FONT_SMALL);
    for (int i = 0 ; i <= xSlots ; i ++) {
        int x = qRound(MARGINS.left() + i * sw);
        double val = xBegin + i * (xEnd - xBegin) / xSlots;
        QRect rect;
        if (i == 0) {
            rect = QRect(QPoint(x, y1), QPoint(x + 40, y1 + 20));
            painter.drawText(rect, xCoordHint(val), Qt::AlignTop | Qt::AlignLeft);
        } else {
            rect = QRect(QPoint(x - 20, y1), QPoint(x + 20, y1 + 20));
            painter.drawText(rect, xCoordHint(val), Qt::AlignTop | Qt::AlignHCenter);
        }
    }
    for (int i = 0 ; i <= ySlots ; i ++) {
        int y = qRound(MARGINS.top() + (ySlots - i) * sh);
        double val = minY + i * (maxY - minY) / ySlots;
        QRect rect;
        if (i == 0) {
            rect = QRect(QPoint(x0 - 50, y - 40), QPoint(x0 - 2, y));
            painter.drawText(rect, yCoordHint(val), Qt::AlignRight | Qt::AlignBottom);
        } else {
            rect = QRect(QPoint(x0 - 50, y - 20), QPoint(x0 - 2, y + 20));
            painter.drawText(rect, yCoordHint(val), Qt::AlignRight | Qt::AlignVCenter);
        }
    }
}

XYPlotBuilder::XYPlotBuilder() {
    plot = new XYPlot();
}

XYPlotBuilder & XYPlotBuilder::setRange(double xBegin, double xEnd) {
    plot->xBegin = xBegin;
    plot->xEnd = xEnd;
    return *this;
}

XYPlotBuilder & XYPlotBuilder::limitY(double minY, double maxY) {
    plot->minY = minY;
    plot->maxY = maxY;
    return *this;
}

XYPlotBuilder & XYPlotBuilder::setSlots(int xSlots, int ySlots) {
    plot->xSlots = xSlots;
    plot->ySlots = ySlots;
    return *this;
}

XYPlotBuilder & XYPlotBuilder::setXCoordHint(std::function<QString(double)> func) {
    plot->xCoordHint = std::move(func);
    return *this;
}

XYPlotBuilder & XYPlotBuilder::setyCoordHint(std::function<QString(double)> func) {
    plot->yCoordHint = std::move(func);
    return *this;
}

XYPlotBuilder & XYPlotBuilder::setSpacing(double spacing) {
    plot->spacing = spacing;
    return *this;
}

XYPlotBuilder & XYPlotBuilder::addFunction(const std::function<double(double)>& func) {
    plot->functions.append(func);
    return *this;
}

XYPlotBuilder & XYPlotBuilder::addEquation(const std::function<double(double)> &eq) {
    plot->equations.append(eq);
    return *this;
}

XYPlot * XYPlotBuilder::get() const {
    return plot;
}
