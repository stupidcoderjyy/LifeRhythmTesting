//
// Created by JYY on 24-11-6.
//

#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <MessageHandler.h>
#include <Widget.h>
#include "SlotsWidget.h"
#include "TextLabel.h"
#include <QDate>

class CalendarItem : public ListItem {
protected:
    QDate date;
    bool isToday;
    bool isCurrentMonth;
    QRect outline;
public:
    explicit CalendarItem(QWidget* parent = nullptr);
    void syncDataToWidget() override;
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent* event) override;
private:
    void updateOutline();
};

class CalendarView : public SlotsWidget {
public:
    explicit CalendarView(QWidget* parent = nullptr);
    void setData(WidgetData* data) override;
    void loadCalendar(const QDate& date) const;
protected:
    ListItem* newItem() override;
};

class CalendarData : public ListData {
    friend class MessageCalendarData;
    friend class CalendarView;
    friend class CalendarUpdateListener;
private:
    QDate date;
    Identifier msgLoc;
public:
    explicit CalendarData(const QString& msgId);
    inline QDate getDate() const;
    inline Identifier getSenderLoc() const;
    inline Identifier getReceiverLoc() const;
    ~CalendarData() override;
protected:
    virtual void fromMessage(const NBT* nbt);
private:
    void ensureData(const QDate& date);
};

inline QDate CalendarData::getDate() const {
    return date;
}

inline Identifier CalendarData::getSenderLoc() const {
    return msgLoc;
}

inline Identifier CalendarData::getReceiverLoc() const {
    return msgLoc.withSuffix("/r");
}

class MessageCalendarData final : public Message {
private:
    CalendarData* d;
public:
    explicit MessageCalendarData(CalendarData* d);
    NBT * buildData() override;
    void onReplied(int result) override;
};

class CalendarUpdateListener : public MessageListener {
private:
    CalendarData* d;
public:
    explicit CalendarUpdateListener(CalendarData* d);
    int onReceived(const Identifier &sender, const NBT *data) override;
};



#endif //CALENDARWIDGET_H
