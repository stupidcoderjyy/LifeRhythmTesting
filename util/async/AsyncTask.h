//
// Created by JYY on 24-12-12.
//

#ifndef ASYNCTASK_H
#define ASYNCTASK_H

#include <QObject>
#include <QRunnable>

class AsyncTask : public QObject, public QRunnable {
    Q_OBJECT
private:
    std::function<void()> handler;
public:
    static AsyncTask* create();
    void start();
    void setTask(std::function<void()> handler);
    void setFinished(const QObject* receiver, const std::function<void()> &handler);
    void run() override;
signals:
    void sigTaskFinished();
private:
    explicit AsyncTask();
};



#endif //ASYNCTASK_H
