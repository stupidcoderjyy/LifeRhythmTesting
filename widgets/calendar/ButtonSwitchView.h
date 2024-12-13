//
// Created by JYY on 24-12-13.
//

#ifndef BUTTONSWITCHVIEW_H
#define BUTTONSWITCHVIEW_H
#include <Namespaces.h>
#include <Button.h>

BEGIN_NAMESPACE(lr::calendar);

class ButtonSwitchView : public Button {
public:
    explicit ButtonSwitchView(QWidget* parent = nullptr, bool iic = true);
protected:
    void handleButtonActivate(QMouseEvent *ev) override;
};

END_NAMESPACE

#endif //BUTTONSWITCHVIEW_H
