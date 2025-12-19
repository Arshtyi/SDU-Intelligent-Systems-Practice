#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include "BinomialHeap.h"
#include "HeapVisualizer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onInsert();
    void onExtractMin();
    void onMerge();
    void onDecreaseKey();
    void onDelete();
    void onLoadFile();
    void onHeapSelectionChanged(int index);

private:
    BinomialHeap *heap1;
    BinomialHeap *heap2;
    BinomialHeap *activeHeap;

    HeapVisualizer *visualizer1;
    HeapVisualizer *visualizer2;

    QComboBox *heapSelector;
    QSpinBox *insertSpinBox;
    QSpinBox *decreaseKeyOldSpinBox;
    QSpinBox *decreaseKeyNewSpinBox;
    QSpinBox *deleteSpinBox;

    void setupUi();
    void updateVisualizers();
};

#endif // MAINWINDOW_H
