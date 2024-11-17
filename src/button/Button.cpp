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
auto qssSelectedDisabled = bg(Styles::CLEAR->rgbHex) + bd("2px", "solid", Styles::GRAY_1->rgbHex) + brad("2px");

Button::Button(QWidget *parent): Label(parent), running(), selected(), hasStyle(true), activatedOnPress(), hasImg(),
                                 enabled(true), type(Click) {
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void Button::setButtonText(const QString &text) {
    hasImg = false;
    QFontMetrics metrics(font());
    setFixedHeight(metrics.height() + 10);
    setFixedWidth(metrics.horizontalAdvance(text) + 10);
    setText(text);
}

void Button::setButtonImg(const QPixmap &p1, const QPixmap& p2) {
    hasImg = true;
    imgNormal = p1;
    imgDisabled = p2.isNull() ? p1 : p2;
    auto p = isEnabled() ? imgNormal : imgDisabled;
    setFixedSize(p.size());
    setPixmap(p);
}

void Button::setButtonEnabled(bool e) {
    if (enabled == e) {
        return;
    }
    enabled = e;
    if (!hasStyle) {
        return;
    }
    if (enabled) {
        if (hasImg) {
            setPixmap(imgNormal);
            setFixedSize(imgNormal.size());
            if (type == Select && selected) {
                setStyleSheet(qssSelected);
            }
        } else {
            auto p = palette();
            p.setColor(QPalette::WindowText, Styles::GRAY_TEXT_0->color);
            p.setColor(QPalette::Text, Styles::GRAY_TEXT_0->color);
            setPalette(p);
        }
    } else {
        if (type == Select && selected) {
            setStyleSheet(qssSelectedDisabled);
        } else {
            setStyleSheet(qssNormal);
        }
        if (hasImg) {
            setPixmap(imgDisabled);
            setFixedSize(imgDisabled.size());
        } else {
            auto p = palette();
            p.setColor(QPalette::WindowText, Styles::GRAY_4->color);
            p.setColor(QPalette::Text, Styles::GRAY_4->color);
            setPalette(p);
        }
    }
}

void Button::setButtonStyleEnabled(bool e) {
    if (enabled != e) {
        enabled = e;
        if (!e) {
            setStyleSheet(qssNormal);
        }
    }
}

void Button::setSelected(bool s) {
    if (!enabled || selected == s || type == Click) {
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
    if (!enabled) {
        return;
    }
    if (hasStyle && (type != Select || !selected)) {
        setStyleSheet(qssHovered);
    }
}

void Button::mouseReleaseEvent(QMouseEvent *ev) {
    if (!enabled) {
        return;
    }
    if (!activatedOnPress) {
        handleButtonActivate(ev);
    }
}

void Button::mousePressEvent(QMouseEvent *ev) {
    if (!enabled) {
        return;
    }
    if (hasStyle) {
        setStyleSheet(qssPressed);
    }
    FocusManager::mark(this);
    if (activatedOnPress) {
        handleButtonActivate(ev);
    }
}

void Button::leaveEvent(QEvent *event) {
    if (!enabled) {
        return;
    }
    if (hasStyle && (type != Select || !selected)) {
        setStyleSheet(qssNormal);
    }
}

void Button::handleButtonActivate(QMouseEvent *ev) {
    if (!isHovered(this, ev)) {
        return;
    }
    if (hasStyle && (type == Click || type == SelectClick)) {
        setStyleSheet(qssHovered);
    }
    if (type == Click) {
        emit sigActivated();
    } else {
        setSelected(!selected);
    }
}
