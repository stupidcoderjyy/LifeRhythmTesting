//
// Created by stupid_coder_jyy on 2024/2/8.
//

#include "Button.h"

#include <NBT.h>

#include "Styles.h"
#include "WidgetUtil.h"
#include "QMouseEvent"
#include "FocusManager.h"

auto qssNormal = bg(Styles::CLEAR->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px");
auto qssHovered = bg(Styles::GRAY_1->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px");
auto qssPressed = bg(Styles::GRAY_2->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px");
auto qssSelected = bg(Styles::GRAY_2->rgbHex) + bd("2px", "none", Styles::CLEAR->rgbHex) + brad("2px");

Button::Button(QWidget *parent): Label(parent), running(), selected(), hasStyle(true), activatedOnPress(), type(Click) {
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

void Button::setSelected(bool s) {
    if (selected == s || type == Click) {
        return;
    }
    selected = s;
    if (s) {
        emit sigSelected();
    } else {
        emit sigCancelled();
    }
    if (!hasStyle) {
        return;
    }
    if (type == Select) {
        if (s) {
            setStyleSheet(qssSelected);
        } else if (isMouseHovered(this)) {
            setStyleSheet(qssHovered);
        } else {
            setStyleSheet(qssNormal);
        }
    }
}

void Button::onPostParsing(Handlers &handlers, NBT *widgetTag) {
    Label::onPostParsing(handlers, widgetTag);
    Mode mode = Click;
    auto modeStr = widgetTag->getString("mode");
    if (modeStr == "Select") {
        mode = Select;
    } else if (modeStr == "SelectClick") {
        mode = SelectClick;
    }
    bool styleEnabled = widgetTag->getBool("styleEnabled", true);
    bool activeOnPress = widgetTag->getBool("activeOnPress", false);
    handlers << [mode, styleEnabled, activeOnPress](QWidget* w) {
        auto b = static_cast<Button*>(w);
        b->setButtonMode(mode);
        b->setButtonStyleEnabled(styleEnabled);
        b->setActivateOnPress(activeOnPress);
    };
}

void Button::enterEvent(QEvent *event) {
    if (hasStyle && (type != Select || !selected)) {
        setStyleSheet(qssHovered);
    }
}

void Button::mouseReleaseEvent(QMouseEvent *ev) {
    if (!activatedOnPress) {
        handleButtonActivate(ev);
    }
}

void Button::mousePressEvent(QMouseEvent *ev) {
    if (hasStyle) {
        setStyleSheet(qssPressed);
    }
    FocusManager::mark(this);
    if (activatedOnPress) {
        handleButtonActivate(ev);
    }
}

void Button::leaveEvent(QEvent *event) {
    if (hasStyle && (type != Select || !selected)) {
        setStyleSheet(qssNormal);
    }
}

void Button::handleButtonActivate(QMouseEvent *ev) {
    if (!isHovered(this, ev)) {
        return;
    }
    if (type == Click) {
        emit sigActivated();
    } else {
        setSelected(!selected);
    }
    if (!hasStyle) {
        return;
    }
    if (type == Click) {
        setStyleSheet(qssHovered);
    } else if (type == SelectClick) {
        setStyleSheet(qssHovered);
    }
}
