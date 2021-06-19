#ifndef GMD_GUI_H
#define GMD_GUI_H

#include <QWidget>

namespace Ui {
class gmd_gui;
}

class gmd_gui : public QWidget
{
    Q_OBJECT

public:
    explicit gmd_gui(QWidget *parent = nullptr);
    ~gmd_gui();

private:
    Ui::gmd_gui *ui;
};

#endif // GMD_GUI_H
