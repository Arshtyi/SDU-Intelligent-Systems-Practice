#include "HeapVisualizer.h"
#include <QBrush>
#include <QPen>

HeapVisualizer::HeapVisualizer(QWidget *parent) : QGraphicsView(parent), currentHeap(nullptr)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
}

void HeapVisualizer::setHeap(BinomialHeap *heap)
{
    currentHeap = heap;
    refresh();
}

void HeapVisualizer::refresh()
{
    scene->clear();
    if (!currentHeap)
        return;
    drawHeap();
}

int HeapVisualizer::calculateTreeWidth(Node *node)
{
    if (!node)
        return 0;
    if (!node->child)
        return 50; // Base width for a node

    int width = 0;
    Node *child = node->child;
    while (child)
    {
        width += calculateTreeWidth(child);
        child = child->sibling;
    }
    return width;
}

void HeapVisualizer::drawTree(Node *node, int x, int y, int availableWidth)
{
    if (!node)
        return;

    // Draw the node
    int nodeSize = 40;
    QGraphicsEllipseItem *ellipse = scene->addEllipse(x - nodeSize / 2, y, nodeSize, nodeSize);
    ellipse->setBrush(QBrush(Qt::white));
    ellipse->setPen(QPen(Qt::black));

    QGraphicsTextItem *text = scene->addText(QString::number(node->key));
    text->setPos(x - text->boundingRect().width() / 2, y + nodeSize / 2 - text->boundingRect().height() / 2);

    // Draw children
    if (node->child)
    {
        int totalChildrenWidth = calculateTreeWidth(node);
        int startX = x - totalChildrenWidth / 2;

        Node *child = node->child;
        while (child)
        {
            int childWidth = calculateTreeWidth(child);
            int childX = startX + childWidth / 2;
            int childY = y + 80;

            // Draw edge
            scene->addLine(x, y + nodeSize, childX, childY, QPen(Qt::black));

            drawTree(child, childX, childY, childWidth);

            startX += childWidth;
            child = child->sibling;
        }
    }
}

void HeapVisualizer::drawHeap()
{
    if (!currentHeap)
        return;

    int startX = 0;
    int startY = 50;

    for (Node *root : currentHeap->getRoots())
    {
        int treeWidth = calculateTreeWidth(root);
        drawTree(root, startX + treeWidth / 2, startY, treeWidth);
        startX += treeWidth + 50; // Gap between trees
    }
}
