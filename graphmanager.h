#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <QObject>

/// 节点图的计算处理的抽象层，应该具有自递归性
class GraphManager : public QObject {
    Q_OBJECT
public:
    explicit GraphManager(QObject *parent = nullptr);

signals:
};

#endif // GRAPHMANAGER_H
