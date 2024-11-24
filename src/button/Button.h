//
// Created by stupid_coder_jyy on 2024/3/12.
//

#ifndef BUTTON_H
#define BUTTON_H

#include "Label.h"

class Button : public Label {
    Q_OBJECT
public:
    enum Mode {
        Click,
        Select,
        SelectClick
    };
protected:
    bool running;
    bool selected;
    bool hasStyle;
    bool activatedOnPress;
    bool hasImg;
    bool enabled;
    Mode type;
    QPixmap imgNormal, imgDisabled;
private:
    static QString qssNormal;
    static QString qssHovered;
    static QString qssPressed;
    static QString qssSelected;
    static QString qssSelectedDisabled;
public:
    explicit Button(QWidget* parent = nullptr, bool initInConstructor = true);
    void setButtonText(const QString& text);
    void setButtonImg(const QPixmap& p1, const QPixmap& p2 = {});
    void setButtonEnabled(bool e = true);
    void setButtonStyleEnabled(bool enabled);
    virtual void setSelected(bool selected);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
    inline void setActivateOnPress(bool aop);
    inline void setButtonMode(Mode type);
    static void mainInit();
signals:
    void sigActivated();
    void sigSelected(bool selected);
protected:
    void enterEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *event) override;
private:
    void handleButtonActivate(QMouseEvent* ev);
};

inline void Button::setButtonMode(Mode t) {
    type = t;
}

inline void Button::setActivateOnPress(bool aop) {
    activatedOnPress = aop;
}

#endif //BUTTON_H
