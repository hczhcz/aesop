#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <sstream>
#include "mmlwidget/QtMmlWidget"
#include "opparser/opcalc.hpp"
#include "opparser/opcalcnear.hpp"
#include "opmml/opmml.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_57_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_59_clicked();

    void on_lineEdit_returnPressed();

public slots:
    void on_input();
    void on_input_f();
    void on_input_sct();
    void on_input_scth();

private:
    Ui::MainWindow *ui;
    OPParser::Calc calc;
    OPParser::MML mml;

    // 1+_ -> 1+sin(_)
    void doInsert(const QString &value, const int offset = 0);

    // 1+2 -> sin(1+2)
    void doApply(const QString &before, const QString &after);

    // Parse, run and print result
    void doRun(const QString &value);
};

#endif // MAINWINDOW_H
