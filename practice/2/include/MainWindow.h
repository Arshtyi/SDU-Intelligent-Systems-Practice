#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QWheelEvent>
#include "BusSystem.h"

class ZoomableGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    ZoomableGraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
        setRenderHint(QPainter::Antialiasing);
    }

protected:
    void wheelEvent(QWheelEvent *event) override
    {
        if (event->angleDelta().y() > 0)
        {
            scale(1.1, 1.1);
        }
        else
        {
            scale(0.9, 0.9);
        }
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchClicked();

private:
    void setupUi();
    void drawMap();
    void drawPath(const std::vector<PathStep> &steps);

    BusSystem busSystem;
    QGraphicsScene *scene;
    ZoomableGraphicsView *view;

    QComboBox *startCombo;
    QComboBox *endCombo;
    QComboBox *criteriaCombo;
    QLabel *resultLabel;
    QPushButton *searchButton;
};
