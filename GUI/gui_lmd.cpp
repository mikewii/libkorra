#include "gui_lmd.h"
#include "ui_gui_lmd.h"

#include <QFileDialog>

#include "Tools/File.hpp"

gui_lmd::gui_lmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gui_lmd)
{
    ui->setupUi(this);
}

gui_lmd::~gui_lmd()
{
    if (this->lmd)
    {
        delete this->lmd;
        this->lmd = nullptr;
    }

    delete ui;
}

void gui_lmd::open(const QString &path)
{
    CContainer container;

    const auto& res = File::File_To_CC(path.toStdString(), container, MH4U::LMD::LMD_MAGIC_HEX);

    if (res)
    {
        this->filepath = path.toStdString();
        const auto& filename = this->filepath.filename();

        this->setWindowTitle(QString::fromStdString(filename.string())); // fromStdString for windows compatibility

        this->lmd = new MH4U::LMD::cLMD(container);

        const auto& vStrings = this->lmd->Get_U16String_vector();

        this->original_strings.reserve(vStrings.size());
        this->replaced_strings.resize(vStrings.size());

        for (const auto& str : vStrings)
            this->original_strings.push_back(this->converter.to_bytes(str).c_str());

        this->populate();
        this->unlock();
    }
    else return; // notify
}

void gui_lmd::on_btn_Open_clicked(void)
{
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Open .lmd File"));

    if (!fileName.isEmpty())
    {
        this->clear();
        this->open(fileName);
    }
}

void gui_lmd::populate(void)
{
    this->ui->list_Strings->addItems(this->original_strings);
    this->ui->list_Strings->item(0)->setSelected(true);
    this->ui->text_Original->setText(this->converter.to_bytes(this->lmd->Get_U16String_vector().at(0)).c_str());
}

void gui_lmd::unlock(void)
{
    this->ui->btn_Save->setEnabled(true);
    this->ui->btn_SaveAs->setEnabled(true);
    this->ui->list_Strings->setCurrentRow(0);
}

void gui_lmd::clear(void)
{
    if (this->lmd)
    {
        delete this->lmd;
        this->lmd = nullptr;
    }

    if (!this->original_strings.isEmpty())
        this->original_strings.clear();

    if (!this->replaced_strings.isEmpty())
        this->replaced_strings.clear();

    this->ui->list_Strings->clear();
}

void gui_lmd::save(const std::filesystem::__cxx11::path &path)
{
    const auto& savepath = path.empty() ? this->filepath : path;

    for (auto i = 0; i < this->replaced_strings.size(); i++)
    {
        const auto& str = this->replaced_strings.at(i).toStdU16String();

        if (!str.empty())
            this->lmd->replace_String(str, i);
    }

    CContainer container;

    this->lmd->write(container);
    this->save_change_color();

    File::CC_To_File(savepath, container);
}

void gui_lmd::save_change_color(void)
{
    for (auto i = 0; i < this->ui->list_Strings->count(); i++)
    {
        const auto& item    = this->ui->list_Strings->item(i);
        const auto& bkg     = item->background();

        if (bkg == Qt::GlobalColor::yellow)
            item->setBackground(Qt::GlobalColor::green);
    }
}


void gui_lmd::on_list_Strings_itemSelectionChanged(void)
{
    const auto id = this->ui->list_Strings->currentIndex().row();

    if (id != -1 && this->lmd) // TODO: lock instead of checking for lmd
    {
        this->ui->text_Original->setText(this->converter.to_bytes(this->lmd->Get_U16String_vector().at(id)).c_str());
        this->ui->text_Replace->setText(this->replaced_strings.at(id));
    }
}


void gui_lmd::on_text_Replace_textChanged(void)
{
    const auto& text = this->ui->text_Replace->toPlainText();
    const auto& id = this->ui->list_Strings->currentIndex().row();

    if (!text.isEmpty() && id != -1)
    {
        this->ui->list_Strings->item(id)->setText(text);
        this->replaced_strings.replace(id, text);
        this->ui->list_Strings->item(id)->setBackground(Qt::GlobalColor::yellow);
    }
    else
    {
        if (id != -1)
        {
            this->ui->list_Strings->item(id)->setText(this->converter.to_bytes(this->lmd->Get_U16String_vector().at(id)).c_str());
            this->ui->list_Strings->item(id)->setBackground(QBrush());
        }
    }
}


void gui_lmd::on_btn_Save_clicked(void)
{
    this->save();
}


void gui_lmd::on_btn_SaveAs_clicked(void)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as"));

    this->save(fileName.toStdString());
}

