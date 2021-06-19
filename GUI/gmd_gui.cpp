#include "gmd_gui.h"
#include "ui_gmd_gui.h"

gmd_gui::gmd_gui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gmd_gui)
{
    ui->setupUi(this);
}

gmd_gui::~gmd_gui()
{
    delete ui;
}
