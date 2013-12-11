#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Ready", 0);

    mml = new QtMmlWidget(this);

    calc.init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    try
    {
        calc.parse(arg1.toStdString());
        ui->statusBar->showMessage(QString(std::to_string(calc.finishByData()).c_str()));
    }
    catch (const OPParser::opparser_error &e)
    {
        ui->statusBar->showMessage(QString(e.what()));
        calc.init();
    }
}

void MainWindow::on_input()
{
    QPushButton *button = (QPushButton *) sender();

    ui->lineEdit->insert(button->text());
    ui->lineEdit->focusWidget();
}

void MainWindow::on_pushButton_57_clicked()
{
    ui->lineEdit->backspace();
    ui->lineEdit->focusWidget();
}

void MainWindow::on_pushButton_58_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit->focusWidget();
}
