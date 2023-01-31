/**
 * @file mainwindow.cpp
 * @author Kacper Wiśniewski (kwisniewski541@gmail.com)
 * @version 1.0
 * @date 2023-01-30
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datavisualizer.h"

/*!
 * \brief MainWindow::MainWindow - Construct a new MainWindow object.
 * \param parent - pointer to the parent object.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , visualizer(new DataVisualizer)
    , auth(new AuthHandler)
    , db(new DatabaseHandler)

{
    ui->setupUi(this);

    connect(auth, &AuthHandler::userSignedIn, this, &MainWindow::userSignedIn);
    connect(auth, &AuthHandler::userSignedIn, db, &DatabaseHandler::userSignedIn);
    connect(db, &DatabaseHandler::dataReady, visualizer, &DataVisualizer::takeData);
    connect(visualizer, &DataVisualizer::refreshData, db, &DatabaseHandler::refreshData);
    connect(visualizer, &DataVisualizer::userLogOut, this, &MainWindow::userLogOut);

    connect(auth, &AuthHandler::userLoginFailed,
            [=] () {QMessageBox::information(this, "ERROR", "Incorrect e-mail or password!");});
    connect(db, &DatabaseHandler::readingDataError,
            [=] () {QMessageBox::information(this, "ERROR", "Loading data failed, please try again.");});
    connect(db, &DatabaseHandler::dataEmpty,
            [=] () {QMessageBox::information(this, "Info", "The database is empty.");});

    readSettings();
}

/*!
 * \brief MainWindow::~MainWindow - destroy the MainWindow object.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::userSignedIn - Sets the interface after the user logs in.
 */
void MainWindow::userSignedIn()
{
    hide();
    visualizer->show();
    if (ui->checkRememeberMe->isChecked())
        rememberMe();
}

/*!
 * \brief MainWindow::userLogOut - Sets the interface after the user logs out.
 */
void MainWindow::userLogOut()
{
    QSettings settings("User", "App");

    settings.setValue("rememberMe", false);

    ui->lineEditEmail->setText("");
    ui->lineEditPassword->setText("");
    visualizer->hide();
    show();
}

/*!
 * \brief MainWindow::on_pushButtonLogin_clicked - Handles the login button press.
 */
void MainWindow::on_pushButtonLogin_clicked()
{
    QString email = ui->lineEditEmail->text();
    QString password = ui->lineEditPassword->text();
    auth->setAPIKey(API_KEY);
    auth->signUserIn(email, password);
}

/*!
 * \brief MainWindow::readSettings - Reads the saved settings.
 */
void MainWindow::readSettings()
{
    QSettings settings("User", "App");

    if (settings.value("rememberMe").toBool())
    {
        settings.beginGroup("userData");
        auth->setAPIKey(API_KEY);
        auth->signUserIn(settings.value("email").toString(), settings.value("password").toString());
        settings.endGroup();
    }
}

/*!
 * \brief MainWindow::rememberMe - Saves user data to settings.
 */
void MainWindow::rememberMe()
{
    QSettings settings("User", "App");

    settings.setValue("rememberMe", true);
    settings.beginGroup("userData");
    settings.setValue("email", ui->lineEditEmail->text());
    settings.setValue("password", ui->lineEditPassword->text());
    settings.endGroup();
}

