/**
 * @file datavisualizer.cpp
 * @author Kacper Wiśniewski (kwisniewski541@gmail.com)
 * @version 1.0
 * @date 2023-01-31
 */

#include "datavisualizer.h"
#include "ui_datavisualizer.h"

/*!
 * \brief DataVisualizer::DataVisualizer - Construct a new DataVisualizer object.
 * \param parent - pointer to the parent object.
 */
DataVisualizer::DataVisualizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataVisualizer)
{
    ui->setupUi(this);
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssDot);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    connect(ui->actionRefresh, &QAction::triggered, this, &DataVisualizer::refreshData);
    connect(ui->actionLog_out, &QAction::triggered, this, &DataVisualizer::userLogOut);

    onGraph = PLATO;
}

/*!
 * \brief DataVisualizer::~DataVisualizer - Destroy the DataVisualizer object.
 */
DataVisualizer::~DataVisualizer()
{
    delete ui;
}

/*!
 * \brief DataVisualizer::takeData - Receives data ready to be displayed.
 * \param data - QMap with ready-to-use data.
 */
void DataVisualizer::takeData(QMap<QString, QVector<double> > &data)
{
    time = data.take("time");
    plato = data.take("plato");
    temperature = data.take("temperature");
    batteryVoltage = data.take("voltage");

    ui->temperatureValue->setText(QString::number(temperature.constLast(), 'f', 2) + " °C");
    ui->platoValue->setText(QString::number(plato.constLast(), 'f', 2) + " °P");

    /* Depending on the voltage level, display the battery status. */
    double lastBatteryVoltage = batteryVoltage.constLast();
    if (lastBatteryVoltage <= BATTERY_VERY_LOW)
        ui->batteryStatus->setText("CRITICAL! " + QString::number(lastBatteryVoltage, 'f', 2) + " V");
    else if (lastBatteryVoltage <= BATTERY_LOW)
        ui->batteryStatus->setText("LOW " + QString::number(lastBatteryVoltage, 'f', 2) + " V");
    else if (lastBatteryVoltage <= BATTERY_MEDIUM)
        ui->batteryStatus->setText("MEDIUM " + QString::number(lastBatteryVoltage, 'f', 2) + " V");
    else
        ui->batteryStatus->setText("HIGH (" + QString::number(lastBatteryVoltage, 'f', 2) + " V)");

    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    ui->customPlot->xAxis->setTicker(dateTimeTicker);
    dateTimeTicker->setDateTimeFormat("d. MMM\nyyyy");

    printData(onGraph);
}

/*!
 * \brief DataVisualizer::printData - Prints the selected data type on the graph.
 * \param dataType - Type of data to be printed.
 */
void DataVisualizer::printData(dataType dataType)
{
    switch (dataType)
    {
    case PLATO:
        ui->customPlot->graph(0)->setData(time, plato);
        ui->customPlot->yAxis->setLabel("Plato [°P]");
        break;
    case TEMPERATURE:
        ui->customPlot->graph(0)->setData(time, temperature);
        ui->customPlot->yAxis->setLabel("Temperature [°C]");
        break;
    case VOLTAGE:
        ui->customPlot->graph(0)->setData(time, batteryVoltage);
        ui->customPlot->yAxis->setLabel("Battery Voltage [V]");
        break;
    default:
        break;
    }
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
    ui->customPlot->update();
}

/*!
 * \brief DataVisualizer::on_graphComboBox_currentIndexChanged - Changes the displayed graph depending on the parameter in combobox.
 * \param index - ComboBox index, selected data type.
 */
void DataVisualizer::on_graphComboBox_currentIndexChanged(int index)
{
    onGraph = (dataType)index;
    printData(onGraph);
}
