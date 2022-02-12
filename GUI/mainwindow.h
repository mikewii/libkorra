#pragma once

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
    void on_btn_arcOpen_clicked(void);
    void on_btn_lmdEditor_clicked(void);

private:
    Ui::MainWindow  *ui;
    QList<QWidget*> gui;
};

