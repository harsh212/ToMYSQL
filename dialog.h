#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QFileSystemModel>
#include<QDir>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    void browse();
    QString getname();
    QString get_filetype();
    QString getpath();
    bool isaccepted();

    ~Dialog();

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void on_buttonBox_accepted();

    void on_Dialog_destroyed();

private:
    Ui::Dialog *ui;
    QFileSystemModel *dirmodel;
    QString path_selected,name;
    QString filetype;
    bool accept=false;
};

#endif // DIALOG_H
