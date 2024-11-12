//
// Created by stupid_coder_jyy on 2024/3/12.
//

#ifndef LIFERHYTHM_TEXTBUTTON_H
#define LIFERHYTHM_TEXTBUTTON_H

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
    Mode type;
public:
    explicit Button(QWidget* parent);
    void setButtonText(const QString& text);
    void setButtonImg(const QPixmap& pixmap);
    inline void setButtonMode(Mode type);
    inline void setButtonStyleEnabled(bool enabled);
    inline void setActivateOnPress(bool aop);
    virtual void setSelected(bool selected);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
signals:
    void sigActivated();
    void sigSelected();
    void sigCancelled();
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

inline void Button::setButtonStyleEnabled(bool enabled) {
    hasStyle = enabled;
}

inline void Button::setActivateOnPress(bool aop) {
    activatedOnPress = aop;
}

#endif //LIFERHYTHM_TEXTBUTTON_H
