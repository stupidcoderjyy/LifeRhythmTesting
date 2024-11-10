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
        Select
    };
private:
    bool running;
    bool selected;
    Mode type;
public:
    explicit Button(QWidget* parent);
    void setButtonText(const QString& text);
    void setButtonImg(const QPixmap& pixmap);
    void setButtonMode(Mode type);
    void setSelected(bool selected);
signals:
    void sigActivated();
    void sigSelected();
    void sigCancelled();
protected:
    void enterEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void leaveEvent(QEvent *event) override;
};

#endif //LIFERHYTHM_TEXTBUTTON_H
