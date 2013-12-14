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
    if (!ui->pushButton_59->isChecked() || arg1.size() == 0) {
        // Do nothing
        return;
    }
    else
    {
        doRun(arg1);
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
    if (ui->pushButton_12->isChecked())
    {
        // Insert the string from the button text
        doInsert(
              ((QPushButton *) sender())->text()
            + "()"
        , -1);
    }
    else
    {
        // Apply the string from the button text
        doApply(((QPushButton *) sender())->text() + "(", ")");
    }
}

void MainWindow::on_input_sct()
{
    const bool needDeg = ui->pushButton_13->isChecked();
    const bool needArc = ui->pushButton_14->isChecked();

    if (ui->pushButton_12->isChecked())
    {
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
    else
    {
        // Apply "rad()" for non-arc func
        if (needDeg && !needArc)
        {
            doApply("rad(", ")");
        }

        // Apply the string from the button text and "arc" button
        doApply(
              (needArc ? "a" : "")
            + ((QPushButton *) sender())->text()
            + "("
        , ")");

        // Apply "deg()" for arc func
        if (needDeg && needArc)
        {
            doApply("deg(", ")");
        }
    }

}

void MainWindow::on_input_scth()
{
    const bool needArc = ui->pushButton_14->isChecked();

    if (ui->pushButton_12->isChecked())
    {
        // Insert the string from the button text and "arc" button
        doInsert(
              (needArc ? "a" : "")
            + ((QPushButton *) sender())->text()
            + "()"
        , -1);
    }
    else
    {
        // Apply the string from the button text and "arc" button
        doApply(
              (needArc ? "a" : "")
            + ((QPushButton *) sender())->text()
            + "("
        , ")");
    }
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

void MainWindow::doApply(const QString &before, const QString &after)
{
    ui->lineEdit->setText(before + ui->lineEdit->text() + after);
}

void MainWindow::doRun(const QString &value)
{
    try
    {
        // Do calculation
        calc.parse(value.toStdString());
        const OPParser::CalcData resultC = calc.finishByData();
        const OPParser::CalcData resultCNear = fnear(resultC);

        std::string resultS = value.toStdString();

        // If not NaN, find near value
        if (resultCNear == resultCNear)
        {
            // Find x ~= result
            const auto found1 = OPParser::NearValue.find(resultCNear);
            if (found1 != OPParser::NearValue.end())
            {
                resultS += "=" + found1->second;
            }
            else
            {
                // Find x ~= -result
                const auto found2 = OPParser::NearValue.find(-resultCNear);
                if (found2 != OPParser::NearValue.end())
                {
                    resultS += "=" + found2->second;
                }
            }
        }

        {
            // Format result as string
            std::ostringstream oss;
            oss << resultCNear;
            auto floatS = oss.str();

            // Format exp
            auto ePos = floatS.find("e");
            if (ePos != std::string::npos)
            {
                floatS.replace(ePos, 1, " e^");
            }

            resultS += "=" + floatS;
        }

        // Generate MML
        mml.parse(resultS);
        const OPParser::MMLData resultM = mml.finishByData();

        // Print
        ui->statusBar->showMessage(QString(std::to_string(resultC).c_str()));
        ui->frame->setContent(QString(resultM.c_str()));

        // Scale the font size to fit in
        ui->frame->setBaseFontPointSize(16);
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

void MainWindow::on_pushButton_57_clicked()
{
    ui->lineEdit->backspace();
}

void MainWindow::on_pushButton_58_clicked()
{
    ui->lineEdit->clear();
}

void MainWindow::on_pushButton_59_clicked()
{
    on_lineEdit_textChanged(ui->lineEdit->text());
}

void MainWindow::on_lineEdit_returnPressed()
{
    doRun(ui->lineEdit->text());
}
