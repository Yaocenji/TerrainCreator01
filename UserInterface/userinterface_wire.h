#ifndef USERINTERFACE_WIRE_H
#define USERINTERFACE_WIRE_H

#include <QObject>

class Wire : public QObject {
    Q_OBJECT
public:
    explicit Wire(QObject *parent = nullptr);

signals:
};

#endif // USERINTERFACE_WIRE_H
