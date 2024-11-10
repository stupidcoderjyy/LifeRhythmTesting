//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "Button.h"

#include "Styles.h"
#include "WidgetUtil.h"
#include "QMouseEvent"
#include "FocusManager.h"

#define qssNormal bg(Styles::CLEAR->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px")
#define qssHovered bg(Styles::GRAY_1->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px")
#define qssPressed bg(Styles::GRAY_2->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px")
#define qssSelected bg(Styles::GRAY_2->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px")

Button::Button(QWidget *parent) : Label(parent), running(), type(Click) {
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void Button::setButtonText(const QString &text) {
    QFontMetrics metrics(font());
    setFixedHeight(metrics.height() + 10);
    setFixedWidth(metrics.horizontalAdvance(text) + 10);
    setText(text);
}

void Button::setButtonImg(const QPixmap &p) {
    setFixedSize(p.size());
    setPixmap(p);
}

void Button::setButtonMode(Mode t) {
    type = t;
}

void Button::setSelected(bool s) {
    if (type == Select && selected != s) {
        if (s) {
            setStyleSheet(qssSelected);
            emit sigSelected();
        } else {
            setStyleSheet(qssNormal);
            emit sigCancelled();
        }
        selected = s;
    }
}

void Button::enterEvent(QEvent *event) {
    if (type == Click || !selected) {
        setStyleSheet(qssHovered);
    }
}

void Button::mouseReleaseEvent(QMouseEvent *ev) {
    if (isHovered(this, ev)) {
        if (type == Click) {
            emit sigActivated();
            setStyleSheet(qssHovered);
        } else {
            setSelected(!selected);
        }
    }
}

void Button::mousePressEvent(QMouseEvent *ev) {
    setStyleSheet(qssPressed);
    FocusManager::mark(this);
}

void Button::leaveEvent(QEvent *event) {
    if (type == Click || !selected) {
        setStyleSheet(qssNormal);
    }
}
