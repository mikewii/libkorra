#pragma once

#include <QWidget>
#include "MH4U/Extentions/lmd.hpp"

namespace Ui {
class gui_lmd;
}

class gui_lmd : public QWidget
{
    Q_OBJECT

public:
    explicit gui_lmd(QWidget *parent = nullptr);
    ~gui_lmd();

    void open(const QString& path);

private slots:
    void on_btn_Open_clicked(void);
    void on_list_Strings_itemSelectionChanged(void);
    void on_text_Replace_textChanged(void);
    void on_btn_Save_clicked(void);
    void on_btn_SaveAs_clicked(void);

private:
    MH4U::LMD::cLMD *lmd = nullptr;
    Ui::gui_lmd     *ui = nullptr;

    std::wstring_convert<std::codecvt_utf8_utf16<utf16>, utf16> converter;
    std::filesystem::path   filepath;
    QStringList             original_strings;
    QVector<QString>        replaced_strings;

    void populate(void);
    void unlock(void);
    void clear(void);
    void save(const std::filesystem::path& path = "");
    void save_change_color(void);
};

