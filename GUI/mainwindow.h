#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

private slots:
    void on_openFile_clicked();

private:
    Ui::MainWindow          *ui;
    QTreeWidgetItem         file;
    QList<QTreeWidgetItem>  tree;

    void Populate( void );
    void dragEnterEvent( QDragEnterEvent *event ) override;
    void dropEvent( QDropEvent *event ) override;
};

#endif // MAINWINDOW_H
