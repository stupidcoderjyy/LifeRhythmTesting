//
// Created by stupid_coder_jyy on 2024/3/12.
//

#ifndef JBTEXTBUTTON_H
#define JBTEXTBUTTON_H

#include "TextLabel.h"
#include "FocusContainer.h"

class JbTextButton : public FocusContainer {
    Q_OBJECT
public:
    enum State {
        Highlighted,
        Normal,
        Disabled
    };
private:
    TextLabel* content;
    FontBuilder fontBuilder;
    State type;
    bool running;
public:
    explicit JbTextButton(QWidget* parent = nullptr);
    void setButtonType(State t);
    void setButtonText(const QString& text);
    void setFont(const FontBuilder& fb);
    void onPostParsing(Handlers &handlers, NBT *widgetTag) override;
signals:
    void sigActivated();
private:
    QString getNormalQss() override;
    QString getFocusedQss() override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void updateBase();
};

#endif //JBTEXTBUTTON_H
