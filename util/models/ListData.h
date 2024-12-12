//
// Created by JYY on 24-12-12.
//

#ifndef LISTDATA_H
#define LISTDATA_H

#include "WidgetData.h"

class ListData : public WidgetData {
    Q_OBJECT
    friend class ListItem;
protected:
    int selectedIdx;
    int changeBegin, changeEnd;
    bool editing;
    QVector<WidgetData*> data;
public:
    ListData();
    void beginEdit();
    void endEdit();
    void markChange(int min, int max);
    void markAll();
    void selectData(int idx);
    virtual void append(WidgetData* data);
    virtual void insert(int idx, WidgetData* data);
    virtual WidgetData* remove(int idx);
    virtual void clear();
    inline int length() const;
    inline WidgetData* at(int idx) const noexcept;
    inline int getChangeBegin() const;
    inline int getChangeEnd() const;
    inline QVector<WidgetData*>& getData();
    signals:
        void sigDataSelected(int pre, int cur);
};

inline int ListData::length() const {
    return data.length();
}

inline WidgetData *ListData::at(int idx) const noexcept {
    if (idx < 0 || idx >= data.length()) {
        return nullptr;
    }
    return data.at(idx);
}

inline int ListData::getChangeBegin() const {
    return changeBegin;
}

inline int ListData::getChangeEnd() const {
    return changeEnd;
}

inline QVector<WidgetData *> &ListData::getData() {
    return data;
}

#endif //LISTDATA_H
