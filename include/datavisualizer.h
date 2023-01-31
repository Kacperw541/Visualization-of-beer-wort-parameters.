/*!
 * @file datavisualizer.h
 * @author Kacper Wiśniewski (kwisniewski541@gmail.com)
 * @version 1.0
 * @date 2023-01-31
 *
 * The DataVisualizer class displays a graph and recent parameter values based on the received data.
 */

#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include <QMainWindow>

#define BATTERY_VERY_LOW 3.3f   ///< Voltage level for very low battery status.
#define BATTERY_LOW      3.7f   ///< Voltage level for low battery status.
#define BATTERY_MEDIUM   3.85f  ///< Voltage level for medium battery status.
#define BATTERY_HIGH     4.0f   ///< Voltage level for high battery status.

namespace Ui {
class DataVisualizer;
}

/*!
 * \brief The DataVisualizer class - Displays a graph and recent parameter values
 */
class DataVisualizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataVisualizer(QWidget *parent = nullptr);     ///< Construct a new DataVisualizer object.
    ~DataVisualizer();                                      ///< Destroy the DataVisualizer object.
public slots:
    void takeData(QMap<QString, QVector <double>> &data);   ///< Receives data ready to be displayed.
private slots:
    void on_graphComboBox_currentIndexChanged(int index);   ///< Changes the displayed graph depending on the parameter in combobox.
signals:
    void refreshData();     ///< The signal sent when data refresh request is made.
    void userLogOut();      ///< The signal sent when user log out request is made.
private:
    /*!
     * \brief The dataType enum - Type of data displayed on the graph
     */
    enum dataType
    {
        PLATO,          ///< Deegres Plato [°P]
        TEMPERATURE,    ///< Temperature [°C]
        VOLTAGE         ///< Battery Voltage [V]
    }onGraph;

    void printData(dataType dataType);  ///< Prints the selected data type on the graph.
    QVector <double> time;              ///< Time container.
    QVector <double> plato;             ///< Container with measured plato degrees.
    QVector <double> temperature;       ///< Container with measured temperature.
    QVector <double> batteryVoltage;    ///< Container with measured battery voltage.

    Ui::DataVisualizer *ui;
};

#endif // DATAVISUALIZER_H
