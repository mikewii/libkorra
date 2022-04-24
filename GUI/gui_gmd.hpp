#ifndef GUI_GMD_HPP
#define GUI_GMD_HPP

#include <QWidget>

namespace Ui {
class gui_gmd;
}

class gui_gmd : public QWidget
{
    Q_OBJECT

public:
    explicit gui_gmd(QWidget *parent = nullptr);
    ~gui_gmd();

private:
    Ui::gui_gmd *ui;
};

#endif // GUI_GMD_HPP
