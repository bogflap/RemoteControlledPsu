#ifndef PSUTHREAD_H
#define PSUTHREAD_H

#include <QObject>

class PsuThread : public QObject
{
    Q_OBJECT
public:
    explicit PsuThread(QObject *parent = nullptr);

signals:
};

#endif // PSUTHREAD_H
