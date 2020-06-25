#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSql>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include<QDebug>
#include<QVariant>
#include<QStringListModel>
#include<QMessageBox>
#include<QFileSystemModel>
#include "dialog.h"
#include <QXlsx/header/xlsxdocument.h>
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include <QComboBox>

using namespace QXlsx;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void database_name();
    void set_base_schema();
    void execute_query();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void read_xlsx();


    void on_pushButton_5_clicked();

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_actionAnshika_triggered();

private:
    Ui::MainWindow *ui;
    QStringListModel*model;
    QString database_selected,filepath;
    int row_count;
    int col_count;
    QStringList attribute,datatype;

};
#endif // MAINWINDOW_H
