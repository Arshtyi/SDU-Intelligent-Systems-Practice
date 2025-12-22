#ifndef HEAPVISUALIZER_H
#define HEAPVISUALIZER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include "BinomialHeap.h"

class HeapVisualizer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HeapVisualizer(QWidget *parent = nullptr);
    void setHeap(BinomialHeap *heap);
    void refresh();

private:
    QGraphicsScene *scene;
    BinomialHeap *currentHeap;

    void drawHeap();
    int calculateTreeWidth(Node *node);
    void drawTree(Node *node, int x, int y, int availableWidth);
    int getTreeDepth(Node *node);
};

#endif // HEAPVISUALIZER_H
