//
// Created by JYY on 24-11-24.
//

#include "DropDown.h"
#include <FocusManager.h>
#include <NBT.h>
#include <WidgetUtil.h>
#include <WidgetFactory.h>
#include <Error.h>

DropDownMenu::DropDownMenu(QWidget *parent): Menu(parent) {
    setWindowModality(Qt::NonModal);
    setAttribute(Qt::WA_DeleteOnClose, false);
}

void DropDownMenu::mouseReleaseEvent(QMouseEvent *event) {
}

void DropDownMenu::focusOutEvent(QFocusEvent *event) {
    if (isMouseHovered(this)) {
        setFocus();
    } else {
        emit sigSelectOption();
    }
}

DropDown::DropDown(QWidget *parent, bool i): FocusContainer(parent, i),
                                             menu(), pressLock(), menuOpen() {
}

void DropDown::onFinishedParsing(Handlers &handlers, NBT *widgetTag) {
    auto* menuTag = widgetTag->get("menu")->asCompound();
    if (menuTag) {
        auto* wf = WidgetFactory::fromNbt("DropDownMenu", menuTag);
        try {
            wf->parse();
        } catch (Error&) {
            delete wf;
            throw;
        }
        handlers << [wf](QWidget *w) {
            wf->apply(nullptr, static_cast<DropDown*>(w)->menu);
            delete wf;
        };
    }
}

DropDown::~DropDown() {
    delete menu;
}

void DropDown::mousePressEvent(QMouseEvent *event) {
    FocusContainer::mousePressEvent(event);
    if (!menuOpen && !pressLock) {
        FocusManager::mark(this);
        if (pressLock) {
            pressLock = false;
            return;
        }
        FocusManager::push(this);
        menu->activateWindow();
        auto p = getGlobalPos(this);
        moveWidget(menu, p.x(), p.y() + height() + 1);
        menu->show();
        menuOpen = true;
    }
}

void DropDown::mouseReleaseEvent(QMouseEvent *event) {
    pressLock = false;
}

void DropDown::focusOutEvent(QFocusEvent *event) {
    if (!FocusManager::checkPeek(this)) {
        FocusContainer::focusOutEvent(event);
        pressLock = false;
    }
}

void DropDown::initWidget() {
    FocusContainer::initWidget();
    menu = new DropDownMenu();
    connect(menu, &DropDownMenu::sigSelectOption, this, [this] {
        syncWidgetToData();
        emit menu->sigAboutToClose();
        menu->close();
    });
    connect(menu, &DropDownMenu::sigAboutToClose, this, [this] {
        pressLock = isMouseHovered(this);
        menuOpen = false;
        FocusManager::popIfPeekMatch(this);
    });
}