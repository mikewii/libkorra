#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>

#include "MHXX/Strings/Items.hpp"
#include "types.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
    this->Populate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent( QDragEnterEvent *event )
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent( QDropEvent *event )
{
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString fileName = url.toLocalFile();

        this->file.setText(0, fileName);

        ui->arcFileView->addTopLevelItem(&this->file);

    }
}


void MainWindow::Populate( void )
{
    auto&           list = *this->ui->ItemsStr;
    QStringList     strList(MHXX::Items.begin(), MHXX::Items.end());


    u32 i = 0;
    for (auto& str : strList )
        str = QString::asprintf("%4d\t", i++) + str;

    list.addItems(strList);
}


void MainWindow::on_openFile_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this);

    this->file.setText(0, filename);

    ui->arcFileView->addTopLevelItem(&this->file);
}

