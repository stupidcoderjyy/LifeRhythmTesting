//
// Created by JYY on 24-11-7.
//

#ifndef INPUTTEXTLABEL_H
#define INPUTTEXTLABEL_H

#include "TextLabel.h"
#include "LineEdit.h"

class InputTextLabel : public TextLabel {
private:
    LineEdit* edit;
    bool editing;
public:
    explicit InputTextLabel(QWidget *parent = Q_NULLPTR);
protected:
    virtual void initEditor();
    virtual void finishEditing();
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void stopEditing();
};



#endif //INPUTTEXTLABEL_H
