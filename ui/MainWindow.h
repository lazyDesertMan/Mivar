#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadFile_triggered();

private slots:
    void on_exit_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW2_H
