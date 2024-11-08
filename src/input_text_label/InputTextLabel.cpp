//
// Created by JYY on 24-11-7.
//

#include "InputTextLabel.h"
#include "RcManagers.h"

InputTextLabel::InputTextLabel(QWidget *parent): TextLabel(parent), edit(new LineEdit(this)) {
    setFixedHeight(34);
    edit->close();
    connect(edit, &LineEdit::editingFinished, this, [this] {
        finishEditing();
        edit->close();
        setVisible(true);
    });
}

void InputTextLabel::initEditor() {
    edit->setText(text());
}

void InputTextLabel::finishEditing() {
    setText(edit->text());
}

void InputTextLabel::mouseDoubleClickEvent(QMouseEvent *event) {
    edit->setGeometry(0, 0, width(), height());
    edit->show();
    initEditor();
    setText("");
    edit->setFocus();
    edit->selectAll();
}