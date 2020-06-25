#include "dialog.h"
#include "ui_dialog.h"

#include<QDebug>
#include<QChar>
#include<QMessageBox>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    browse();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::browse()
{
    dirmodel=new QFileSystemModel(this);
    dirmodel->setRootPath("C:/");
    ui->treeView->setModel(dirmodel);


}

void Dialog::on_treeView_clicked(const QModelIndex &index)
{
    QString path_selected=  dirmodel->fileInfo(index).absoluteFilePath();
    QString name= dirmodel->fileName(index);
    ui->lineEdit->setText(path_selected);
}

void Dialog::on_buttonBox_accepted()
{
    QModelIndex index=ui->treeView->currentIndex();
    path_selected=  dirmodel->fileInfo(index).absoluteFilePath();
    name=dirmodel->fileInfo(index).fileName();
    filetype=dirmodel->fileInfo(index).completeSuffix();
    if(dirmodel->fileInfo(index).completeSuffix()!="xlsx")
    {
        QMessageBox::information(this,"Type unmatched ","File you selected is not excel file\n Selection of .xlsx files are only valid",QMessageBox::Ok);
    }
    else
    {
        path_selected=QDir::toNativeSeparators(path_selected);
        accept=true;
    }

}

QString Dialog:: getname()
{
    return name;
}
QString Dialog::getpath()
{
    return path_selected;
}
bool Dialog::isaccepted()
{
    return accept;
}
QString Dialog::get_filetype()
{
    return filetype;
}
void Dialog::on_Dialog_destroyed()
{

}
