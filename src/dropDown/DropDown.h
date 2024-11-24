//
// Created by JYY on 24-11-24.
//

#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "FocusContainer.h"
#include "LineEdit.h"
#include "Menu.h"

class DropDownMenu : public Menu {
    Q_OBJECT
public:
    explicit DropDownMenu(QWidget *parent = nullptr);
signals:
    void sigSelectOption();
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
};

class DropDown : public FocusContainer {
protected:
    DropDownMenu *menu;
    bool pressLock;
    bool menuOpen;
public:
    explicit DropDown(QWidget *parent = nullptr, bool initInConstructor = true);
    void onFinishedParsing(Handlers &handlers, NBT *widgetTag) override;
    ~DropDown() override;
    inline DropDownMenu *getMenu() const;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void initWidget() override;
};

inline DropDownMenu * DropDown::getMenu() const {
    return menu;
}

#endif //DROPDOWN_H
