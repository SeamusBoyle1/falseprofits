// Copyright 2017

#ifndef FINISHNOTIFIER_H
#define FINISHNOTIFIER_H

#include <QObject>

class FinishNotifier : public QObject
{
    Q_OBJECT
public:
    FinishNotifier() {}
    virtual ~FinishNotifier() {}

Q_SIGNALS:
    void finished();

public Q_SLOTS:
    void setFinished()
    {
        if (!m_finished) {
            m_finished = true;
            Q_EMIT finished();
        }
    }

private:
    bool m_finished{ false };
};

#endif // FINISHNOTIFIER_H
