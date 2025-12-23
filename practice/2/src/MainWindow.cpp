#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QMessageBox>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    busSystem.loadData("in.txt");
    setupUi();
    drawMap();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    startCombo = new QComboBox();
    endCombo = new QComboBox();

    // Populate combos
    std::vector<std::string> names;
    for (const auto &stop : busSystem.getStops())
    {
        names.push_back(stop.name);
    }
    std::sort(names.begin(), names.end());
    for (const auto &name : names)
    {
        startCombo->addItem(QString::fromStdString(name));
        endCombo->addItem(QString::fromStdString(name));
    }

    criteriaCombo = new QComboBox();
    criteriaCombo->addItem("Min Cost");
    criteriaCombo->addItem("Min Time (No Wait)");
    criteriaCombo->addItem("Min Time (With Wait)");

    searchButton = new QPushButton("Search");
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);

    controlLayout->addWidget(new QLabel("Start:"));
    controlLayout->addWidget(startCombo);
    controlLayout->addWidget(new QLabel("End:"));
    controlLayout->addWidget(endCombo);
    controlLayout->addWidget(new QLabel("Criteria:"));
    controlLayout->addWidget(criteriaCombo);
    controlLayout->addWidget(searchButton);

    mainLayout->addLayout(controlLayout);

    resultLabel = new QLabel("Ready");
    resultLabel->setWordWrap(true);
    mainLayout->addWidget(resultLabel);

    scene = new QGraphicsScene(this);
    view = new ZoomableGraphicsView(this);
    view->setScene(scene);
    mainLayout->addWidget(view);

    resize(1000, 800);
}

void MainWindow::drawMap()
{
    scene->clear();

    // Draw edges first
    QPen edgePen(Qt::lightGray);
    edgePen.setWidth(1);

    for (const auto &route : busSystem.getRoutes())
    {
        const auto &stops = route.stopIds;
        for (size_t i = 0; i < stops.size() - 1; ++i)
        {
            const auto &s1 = busSystem.getStops()[stops[i]];
            const auto &s2 = busSystem.getStops()[stops[i + 1]];
            scene->addLine(s1.x, s1.y, s2.x, s2.y, edgePen);
        }
    }

    // Draw stops
    QBrush stopBrush(Qt::blue);
    QPen stopPen(Qt::black);

    for (const auto &stop : busSystem.getStops())
    {
        double r = 5;
        QGraphicsEllipseItem *item = scene->addEllipse(stop.x - r, stop.y - r, 2 * r, 2 * r, stopPen, stopBrush);
        item->setToolTip(QString::fromStdString(stop.name));

        QGraphicsTextItem *text = scene->addText(QString::fromStdString(stop.name));
        text->setPos(stop.x, stop.y);
        text->setDefaultTextColor(Qt::black);
    }

    // Fit in view
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::onSearchClicked()
{
    std::string start = startCombo->currentText().toStdString();
    std::string end = endCombo->currentText().toStdString();

    BusSystem::Criteria criteria;
    switch (criteriaCombo->currentIndex())
    {
    case 0:
        criteria = BusSystem::MinCost;
        break;
    case 1:
        criteria = BusSystem::MinTimeNoWait;
        break;
    case 2:
        criteria = BusSystem::MinTimeWithWait;
        break;
    default:
        criteria = BusSystem::MinCost;
    }

    auto result = busSystem.findPath(start, end, criteria);

    if (!result.found)
    {
        resultLabel->setText("No path found.");
        drawMap();
        return;
    }

    QString resText = QString("Total Value: %1. Path: ").arg(result.totalValue);
    for (size_t i = 0; i < result.steps.size(); ++i)
    {
        resText += QString::fromStdString(result.steps[i].stopName);
        if (i < result.steps.size() - 1)
        {
            int nextRoute = result.steps[i + 1].routeId;
            resText += QString(" --(R%1)--> ").arg(nextRoute);
        }
    }

    resultLabel->setText(resText);

    drawPath(result.steps);
}

void MainWindow::drawPath(const std::vector<PathStep> &steps)
{
    drawMap(); // Reset to base state

    if (steps.empty())
        return;

    QPen pathPen(Qt::red);
    pathPen.setWidth(3);

    for (size_t i = 0; i < steps.size() - 1; ++i)
    {
        std::string s1Name = steps[i].stopName;
        std::string s2Name = steps[i + 1].stopName;

        int id1 = busSystem.getStopNameMap().at(s1Name);
        int id2 = busSystem.getStopNameMap().at(s2Name);

        const auto &s1 = busSystem.getStops()[id1];
        const auto &s2 = busSystem.getStops()[id2];

        scene->addLine(s1.x, s1.y, s2.x, s2.y, pathPen);
    }
}
