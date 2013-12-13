#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->showMessage("Ready", 0);

    // Init MathML content
    ui->frame->setContent(_MML(_MN(42)));

    ui->frame->setBaseFontPointSize(16);

    calc.init();
    mml.init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    // Nothing
    if (arg1.size() == 0) {
        return;
    }
    try
    {
        // Do calculation
        calc.parse(arg1.toStdString());
        OPParser::CalcData resultC = calc.finishByData();
        mml.parse(arg1.toStdString());
        OPParser::MMLData resultM = mml.finishByData();

        // Print
        ui->statusBar->showMessage(QString(std::to_string(resultC).c_str()));
        ui->statusBar->showMessage(QString(resultM.c_str()));
        ui->frame->setContent(QString(resultM.c_str()));
        ui->frame->setBaseFontPointSize(16);

        // Scale the size to fit in
        while (
            (
                ui->frame->getSize().width() > ui->frame->width()
                ||
                ui->frame->getSize().height() > ui->frame->height()
            )
            &&
            ui->frame->baseFontPointSize() > 8
        )
        {
            ui->frame->setBaseFontPointSize(ui->frame->baseFontPointSize() - 1);
        }
    }
    catch (const OPParser::opparser_error &e)
    {
        ui->statusBar->showMessage(QString(e.what()));
        calc.init();
        mml.init();
    }
}

void MainWindow::on_input()
{
    // Insert the string from the button tool tip
    doInsert(
        ((QPushButton *) sender())->toolTip()
    );
}

void MainWindow::on_input_f()
{
    // Insert the string from the button text
    doInsert(
          ((QPushButton *) sender())->text()
        + "()"
    , -1);
}

void MainWindow::on_input_sct()
{
    const bool needDeg = ui->pushButton_13->isChecked();
    const bool needArc = ui->pushButton_14->isChecked();

    // Insert "deg()" for arc func
    if (needDeg && needArc)
    {
        doInsert("deg()", -1);
    }

    // Insert the string from the button text and "arc" button
    doInsert(
          (needArc ? "a" : "")
        + ((QPushButton *) sender())->text()
        + "()"
    , -1);

    // Insert "rad()" for non-arc func
    if (needDeg && !needArc)
    {
        doInsert("rad()", -1);
    }

}

void MainWindow::on_input_scth()
{
    const bool needArc = ui->pushButton_14->isChecked();

    // Insert the string from the button text and "arc" button
    doInsert(
          (needArc ? "a" : "")
        + ((QPushButton *) sender())->text()
        + "()"
    , -1);
}

void MainWindow::doInsert(const QString &value, const int offset)
{
    const QString &old = ui->lineEdit->text();

    // If one is letter and another is letter or digit, add " "
    // For example, "3" + "pi" -> "3 pi"
    if (
        ui->lineEdit->text() != ""
        &&
        (
            (
                (*(old.end()-1)).isLetter() && value[0].isLetterOrNumber()
            )
            ||
            (
                value[0].isLetter() && (*(old.end()-1)).isLetterOrNumber()
            )
        )
    )
    {
        ui->lineEdit->insert(" ");
    }

    // Do insert
    ui->lineEdit->insert(value);

    // Do move
    ui->lineEdit->setCursorPosition(ui->lineEdit->cursorPosition() + offset);
}

void MainWindow::on_pushButton_57_clicked()
{
    ui->lineEdit->backspace();
}

void MainWindow::on_pushButton_58_clicked()
{
    ui->lineEdit->clear();
}
