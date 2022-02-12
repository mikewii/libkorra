#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardItemModel>

#include "Tools/File.hpp"
#include "Tools/Folder.hpp"
#include "Tools/ARC.hpp"
#include "MH4U/MH4U.hpp"

QList<QStandardItem *> itemList;
QStandardItemModel model;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    for (auto* item : qAsConst(itemList))
    {
        if (item)
        {
            delete item;
            item = nullptr;
        }
    }
    delete ui;
}

void MainWindow::on_btn_arcOpen_clicked(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));

    if (fileName.isEmpty())
        return;

    File::File_To_CC(fileName.toStdString(), File::oneFile);

    std::vector<Pair> vector;

    ARC arc(File::oneFile, vector);

    arc.ExtractAll();

    itemList.reserve(vector.size());
    model.clear();
    for (const auto& pair : vector)
    {
        QStandardItem* item = new  QStandardItem;

        item->setText(pair.info.Filename);

        itemList.push_back(item);
    }

    for (auto i = 0; i < itemList.size(); i++)
    {
        model.setItem(i, 0, itemList.at(i));
    }

    this->ui->listArcFilelist->setModel(&model);
}

