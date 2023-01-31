/*!
 * @file mainwindow.h
 * @author Kacper Wiśniewski (kwisniewski541@gmail.com)
 * @version 1.0
 * @date 2023-01-30
 *
 * The MainWindow class displays/hides a login form and a graph with data. Also loads and saves settings.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include "datavisualizer.h"
#include "authhandler.h"
#include "databasehandler.h"

#define API_KEY "AIzaSyDbpwdghPR8KNOvCjVEuiLMZYcFcP8-mVA"   ///< Firebase web app API key.

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class - displays/hides a login form and a graph with data.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);  ///< Construct a new MainWindow object.
    ~MainWindow();                          ///< Destroy the MainWindow object.
public slots:
    void userSignedIn();            ///< Sets the interface after the user logs in.
    void userLogOut();              ///< Sets the interface after the user logs out.
private slots:
    void on_pushButtonLogin_clicked();      ///< Handles the login button press.
private:
    void readSettings();            ///< Reads the saved settings.
    void rememberMe();              ///< Saves user data to settings.
    Ui::MainWindow *ui;
    DataVisualizer *visualizer;
    AuthHandler *auth;
    DatabaseHandler *db;

};
#endif // MAINWINDOW_H
