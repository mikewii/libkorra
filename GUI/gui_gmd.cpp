#include "gui_gmd.hpp"
#include "ui_gui_gmd.h"

gui_gmd::gui_gmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gui_gmd)
{
    ui->setupUi(this);
}

gui_gmd::~gui_gmd()
{
    delete ui;
}
