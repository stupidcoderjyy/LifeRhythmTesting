//
// Created by JYY on 24-12-12.
//

#include <qthread.h>

#include "LifeRhythm.h"
#include "AsyncGetter.h"
#include <QDebug>

USING_NAMESPACE(lr)

class TestData : public  QObject {
public:
    int val;
    explicit TestData(int v) : val(v) {
    }
    ~TestData() override {
        qDebug() << "TestData::~TestData()";
    }
};

int main(int argc, char* argv[]) {
    LifeRhythm lr(argc, argv);
    Config cfg;
    cfg.setMode(Config::Test);
    lr.setConfig(cfg);
    lr.onPostInit([] {
        auto getter = AsyncGetter::create();
        getter->setBuilder([] {
            QThread::msleep(5000);
            qDebug() << "Worker:" << QThread::currentThread();
            return new TestData(999);
        });
        getter->setHandler([](QObject* o) {
            qDebug() << "Handler:" << QThread::currentThread();
            qDebug() << static_cast<TestData*>(o)->val;
        });
        getter->start();
        qDebug() << "Main:" << QThread::currentThread();
    });
    return lr.launch();
}