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
    // The button
    QPushButton *button = (QPushButton *) sender();

    const QString &newData = button->toolTip();
    const QString &oldData = ui->lineEdit->text();

    // If one is letter and another is letter or digit, add " "
    // For example, "3" + "pi" -> "3 pi"
    if (
        ui->lineEdit->text() != ""
        &&
        (
            (
                (*(oldData.end()-1)).isLetter() && newData[0].isLetterOrNumber()
            )
            ||
            (
                newData[0].isLetter() && (*(oldData.end()-1)).isLetterOrNumber()
            )

        )
    )
    {
        ui->lineEdit->insert(" ");
    }

    // Do insert
    ui->lineEdit->insert(newData);
}

void MainWindow::on_input_f()
{
}

void MainWindow::on_input_sct()
{
}

void MainWindow::on_input_scth()
{
}

void MainWindow::on_pushButton_57_clicked()
{
    ui->lineEdit->backspace();
}

void MainWindow::on_pushButton_58_clicked()
{
    ui->lineEdit->clear();
}
