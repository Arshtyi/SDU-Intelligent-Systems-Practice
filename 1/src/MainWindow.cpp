#include "MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), heap1(new BinomialHeap()), heap2(new BinomialHeap())
{
    activeHeap = heap1;
    setupUi();
    updateVisualizers();
}

MainWindow::~MainWindow()
{
    delete heap1;
    delete heap2;
}

void MainWindow::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Controls Area
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    // Heap Selection
    controlsLayout->addWidget(new QLabel("Active Heap:"));
    heapSelector = new QComboBox();
    heapSelector->addItem("Heap 1");
    heapSelector->addItem("Heap 2");
    connect(heapSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onHeapSelectionChanged);
    controlsLayout->addWidget(heapSelector);

    // Insert
    insertSpinBox = new QSpinBox();
    insertSpinBox->setRange(-1000, 1000);
    QPushButton *insertBtn = new QPushButton("Insert");
    connect(insertBtn, &QPushButton::clicked, this, &MainWindow::onInsert);
    controlsLayout->addWidget(insertSpinBox);
    controlsLayout->addWidget(insertBtn);

    // Extract Min
    QPushButton *extractMinBtn = new QPushButton("Extract Min");
    connect(extractMinBtn, &QPushButton::clicked, this, &MainWindow::onExtractMin);
    controlsLayout->addWidget(extractMinBtn);

    // Merge
    QPushButton *mergeBtn = new QPushButton("Merge Other into Active");
    connect(mergeBtn, &QPushButton::clicked, this, &MainWindow::onMerge);
    controlsLayout->addWidget(mergeBtn);

    // Load File
    QPushButton *loadFileBtn = new QPushButton("Load File");
    connect(loadFileBtn, &QPushButton::clicked, this, &MainWindow::onLoadFile);
    controlsLayout->addWidget(loadFileBtn);

    mainLayout->addLayout(controlsLayout);

    // More Controls
    QHBoxLayout *controlsLayout2 = new QHBoxLayout();

    // Decrease Key
    controlsLayout2->addWidget(new QLabel("Decr Key (Old->New):"));
    decreaseKeyOldSpinBox = new QSpinBox();
    decreaseKeyOldSpinBox->setRange(-1000, 1000);
    decreaseKeyNewSpinBox = new QSpinBox();
    decreaseKeyNewSpinBox->setRange(-1000, 1000);
    QPushButton *decreaseKeyBtn = new QPushButton("Decrease Key");
    connect(decreaseKeyBtn, &QPushButton::clicked, this, &MainWindow::onDecreaseKey);
    controlsLayout2->addWidget(decreaseKeyOldSpinBox);
    controlsLayout2->addWidget(decreaseKeyNewSpinBox);
    controlsLayout2->addWidget(decreaseKeyBtn);

    // Delete
    controlsLayout2->addWidget(new QLabel("Delete Key:"));
    deleteSpinBox = new QSpinBox();
    deleteSpinBox->setRange(-1000, 1000);
    QPushButton *deleteBtn = new QPushButton("Delete");
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onDelete);
    controlsLayout2->addWidget(deleteSpinBox);
    controlsLayout2->addWidget(deleteBtn);

    mainLayout->addLayout(controlsLayout2);

    // Visualization Area
    QSplitter *splitter = new QSplitter(Qt::Vertical);

    QWidget *container1 = new QWidget();
    QVBoxLayout *layout1 = new QVBoxLayout(container1);
    layout1->addWidget(new QLabel("Heap 1"));
    visualizer1 = new HeapVisualizer();
    visualizer1->setHeap(heap1);
    layout1->addWidget(visualizer1);
    splitter->addWidget(container1);

    QWidget *container2 = new QWidget();
    QVBoxLayout *layout2 = new QVBoxLayout(container2);
    layout2->addWidget(new QLabel("Heap 2"));
    visualizer2 = new HeapVisualizer();
    visualizer2->setHeap(heap2);
    layout2->addWidget(visualizer2);
    splitter->addWidget(container2);

    mainLayout->addWidget(splitter);
}

void MainWindow::onHeapSelectionChanged(int index)
{
    if (index == 0)
        activeHeap = heap1;
    else
        activeHeap = heap2;
}

void MainWindow::onInsert()
{
    int val = insertSpinBox->value();
    activeHeap->insert(val);
    updateVisualizers();
}

void MainWindow::onExtractMin()
{
    Node *min = activeHeap->find_min();
    if (min)
    {
        int key = min->key;
        activeHeap->extract_min();
        QMessageBox::information(this, "Extract Min", "Extracted: " + QString::number(key));
    }
    else
    {
        QMessageBox::warning(this, "Extract Min", "Heap is empty!");
    }
    updateVisualizers();
}

void MainWindow::onMerge()
{
    BinomialHeap *otherHeap = (activeHeap == heap1) ? heap2 : heap1;
    activeHeap->union_heap(otherHeap);
    updateVisualizers();
}

void MainWindow::onDecreaseKey()
{
    int oldKey = decreaseKeyOldSpinBox->value();
    int newKey = decreaseKeyNewSpinBox->value();

    Node *node = activeHeap->find_node(oldKey);
    if (node)
    {
        activeHeap->decrease_key(node, newKey);
        updateVisualizers();
    }
    else
    {
        QMessageBox::warning(this, "Decrease Key", "Node with key " + QString::number(oldKey) + " not found!");
    }
}

void MainWindow::onDelete()
{
    int key = deleteSpinBox->value();
    Node *node = activeHeap->find_node(key);
    if (node)
    {
        activeHeap->delete_node(node);
        updateVisualizers();
    }
    else
    {
        QMessageBox::warning(this, "Delete", "Node with key " + QString::number(key) + " not found!");
    }
}

void MainWindow::onLoadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Dataset", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot open file!");
        return;
    }

    QTextStream in(&file);
    int count = 0;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        for (const QString &part : parts)
        {
            bool ok;
            int val = part.toInt(&ok);
            if (ok)
            {
                activeHeap->insert(val);
                count++;
            }
        }
    }
    file.close();
    QMessageBox::information(this, "Load File", "Loaded " + QString::number(count) + " elements.");
    updateVisualizers();
}

void MainWindow::updateVisualizers()
{
    visualizer1->refresh();
    visualizer2->refresh();
}
