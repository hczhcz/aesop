#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mmlwidget/QtMmlWidget"
#include "opparser/opcalc.hpp"
#include "opparser/opcalcnear.hpp"

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

public slots:
    void on_input();
    void on_input_f();
    void on_input_sct();
    void on_input_scth();

private:
    Ui::MainWindow *ui;
    QtMmlWidget *mml;
    OPParser::Calc calc;

    void doInsert(const QString &value, const int offset = 0);
};

#endif // MAINWINDOW_H
