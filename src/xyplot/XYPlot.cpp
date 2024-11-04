//
// Created by JYY on 24-11-4.
//

#include "XYPlot.h"
#include <QDebug>

const QMargins XYPlot::MARGINS = {50,50,50,50};

const double XYPlot::SPACING = 0.01;

void XYPlot::outputImage(const QString &path) {
    auto img = new QPixmap(width(), height());
    {
        QPainter painter(img);
        drawPlot(painter);
    }
    img->save(path);
    delete img;
}

XYPlot::XYPlot(): xBegin(), xEnd(), minY(), maxY(), xSlots(), ySlots() {
}

void XYPlot::resizeEvent(QResizeEvent *event) {
    const int canvasW = width() - MARGINS.left() - MARGINS.right();
    const int canvasH = height() - MARGINS.bottom() - MARGINS.top();
    minY = 1.79769e+308;
    maxY = 2.22507e-308;
    QVector<QVector<QPointF>> values;
    for (const auto& func : functions) {
        double x = xBegin;
        auto child = QVector<QPointF>();
        while (x - xEnd < SPACING) {
            double y = func(x);
            minY = qMin(minY, y);
            maxY = qMax(maxY, y);
            child << QPointF(x, y);
            x += SPACING;
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
        if (!(i & 1)) {
            continue;
        }
        int x = qRound(MARGINS.left() + i * sw);
        double val = xBegin + i * (xEnd - xBegin) / xSlots;
        auto rect = QRect(QPoint(x - 20, y1 + 2), QPoint(x + 20, y1 + 20));
        painter.drawText(rect, xCoordHint(val), Qt::AlignTop | Qt::AlignHCenter);
    }
    for (int i = 0 ; i <= ySlots ; i ++) {
        if (!(i & 1)) {
            continue;
        }
        int y = qRound(MARGINS.top() + i * sh);
        double val = minY + (ySlots - i) * (maxY - minY) / ySlots;
        auto rect = QRect(QPoint(x0 - 50, y - 20), QPoint(x0 - 2, y + 20));
        painter.drawText(rect, yCoordHint(val), Qt::AlignRight | Qt::AlignVCenter);
    }
}

QString XYPlot::xCoordHint(double value) {
    return QString::asprintf("%.1f", value);
}

QString XYPlot::yCoordHint(double value) {
    return QString::asprintf("%.1f", value);
}

XYPlotBuilder::XYPlotBuilder() {
    plot = new XYPlot();
}

XYPlotBuilder & XYPlotBuilder::setRange(double xBegin, double xEnd) {
    plot->xBegin = xBegin;
    plot->xEnd = xEnd;
    return *this;
}

XYPlotBuilder & XYPlotBuilder::setSlots(int xSlots, int ySlots) {
    plot->xSlots = xSlots;
    plot->ySlots = ySlots;
    return *this;
}

XYPlotBuilder & XYPlotBuilder::addFunction(const std::function<double(double)>& func) {
    plot->functions.append(func);
    return *this;
}

XYPlot * XYPlotBuilder::get() const {
    return plot;
}