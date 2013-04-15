#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void printTest(int num);
    void printTest2(int num);
    void printTest3(int num);
    void printTest4(int num);
    void printTest5(int num);
    void printTest6(int num);
    void checkChanged(bool b);
    void checkChanged2(bool b);
    void checkChanged3(bool b);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
