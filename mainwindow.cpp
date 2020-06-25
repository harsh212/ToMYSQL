#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QAccessibleEditableTextInterface>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setCentralWidget(ui->stackedWidget);
    set_base_schema();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_base_schema()
{
    database_name();
    ui->listView->setHidden(true);
    ui->label->setHidden(true);
    ui->lineEdit->setHidden(true);
    ui->lineEdit->setPlaceholderText("Enter Valid Database name ");
    ui->lineEdit_3->setPlaceholderText("Search database ");
    ui->lineEdit->setFrame(true);
    ui->pushButton->setHidden(true);
    ui->lineEdit_3->setHidden(true);
    ui->tableWidget->setHidden(true);
    ui->pushButton_5->setHidden(true);

    //Next page

}
void MainWindow::database_name()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    model=new QStringListModel;
    QStringList dbnames;
    if(db.open())
    {
        QSqlQuery query;
        query.exec("show databases");
        while(query.next())
        {
            QVariant res=query.value(0);
            dbnames.append(res.toString());
        }
        model->setStringList(dbnames);
        ui->listView->setModel(model);
        ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void MainWindow::on_radioButton_clicked()
{
    ui->listView->setHidden(true);
    ui->label->setHidden(true);
    ui->lineEdit_3->setHidden(true);
    ui->lineEdit->setHidden(false);
    ui->pushButton->setHidden(false);

}

void MainWindow::on_radioButton_2_clicked(bool checked)
{
    if(checked)
    {
        ui->listView->setHidden(false);
        ui->label->setHidden(false);
        ui->lineEdit_3->setHidden(false);
        ui->lineEdit->setHidden(true);
        ui->pushButton->setHidden(true);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString name=ui->lineEdit->text();
    name=name.trimmed();
    QString q="create database "+name;
    QSqlQuery query;
    if(query.exec(q))
    {
        query.exec("show databases");
        QStringList dbnames;
        while(query.next())
        {
            QVariant res=query.value(0);
            dbnames.append(res.toString());
        }
        model->removeRows(0,model->rowCount());
        model->setStringList(dbnames);
        ui->listView->setModel(model);
        ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        on_radioButton_2_clicked(true);
    }
    else
    {
        QMessageBox::information(this,"Failed ","Invalid Database name",QMessageBox::Ok);
    }



}

void MainWindow::on_pushButton_2_clicked()
{
    //SUBMIT
    QVariant name=ui->listView->currentIndex().data();
     database_selected=name.toString();
    //qDebug()<<dbname;
    if(database_selected.isNull())
        QMessageBox::warning(this,"Unselected database","All values are null \nCannot Proceed",QMessageBox::Ok);
    else
    {
         ui->stackedWidget->setCurrentWidget(ui->page_4);
         ui->label_2->setText("Server Name:    localhost\nDatabase Name:  "+database_selected);
    }


}

void MainWindow::on_pushButton_3_clicked()
{
    close();
}

void MainWindow::on_pushButton_4_clicked()
{
    //Browse File folder

    Dialog dial;
    dial.exec();
    filepath=dial.getpath();
    QString filename=dial.getname();
    ui->label_4->setText(filename+" selected ");
    if(dial.get_filetype()=="xlsx")
    read_xlsx();
    else
        ui->label_4->setText(filename+" selected! Select .xlsx file");
}
void MainWindow::read_xlsx()
{
    QXlsx::Document xlsx(filepath);
    row_count=xlsx.dimension().rowCount();
    col_count=xlsx.dimension().columnCount();
    ui->tableWidget->setHidden(false);
    ui->pushButton_5->setHidden(false);

    ui->tableWidget->setRowCount(col_count);
    ui->tableWidget->setColumnCount(2);
    //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnWidth(0,ui->tableWidget->width()/2-10);
    ui->tableWidget->setColumnWidth(1,ui->tableWidget->width()/2-10);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);


    QTableWidgetItem* hitem,*ritem;
    hitem=new QTableWidgetItem;
    hitem->setText("Attributes ");
    ui->tableWidget->setHorizontalHeaderItem(0,hitem);

    ritem=new QTableWidgetItem;
    ritem->setText("Set Datatype ");
    ui->tableWidget->setHorizontalHeaderItem(1,ritem);

    int i=1;

    while (i<=col_count)
    {
        QTableWidgetItem*item;
        item=new QTableWidgetItem;
        item->setText(xlsx.read(1,i).toString());
        attribute.append(xlsx.read(1,i).toString());
        ui->tableWidget->setItem(i-1,0,item);
        i++;

    }
    QString q="use "+database_selected;
    QSqlQuery query,query2;
    query.exec(q);

}

void MainWindow::execute_query()
{

}

void MainWindow::on_pushButton_5_clicked()
{
    int i=1;
    QXlsx::Document xlsx(filepath);
    while (i<=col_count)
    {
        QTableWidgetItem*item;
        item=new QTableWidgetItem;
        item=ui->tableWidget->item(i-1,1);
        datatype.append(item->text());
        i++;
    }
    if(ui->lineEdit_2->text()=="")
        QMessageBox::warning(this,"Invalid Table Name","Empty table name",QMessageBox::Ok);
    else
    {
        QSqlQuery query;
        QString Qtext="Create table "+ui->lineEdit_2->text()+"(";
        i=0;
        while (i<col_count)
        {
            Qtext+=attribute[i];
            Qtext+=" "+datatype[i];
            i++;
            if(i!=col_count)
                Qtext+=" ,";
        }
        Qtext+=")";
        if(!query.exec(Qtext))
        {

           QMessageBox::warning(this,"MYSQL Error ",query.lastError().text(),QMessageBox::Ok);
        }
        else
        {
            QMessageBox::warning(this,"Successfull ","Table created Successfully",QMessageBox::Ok);
        }
        Qtext="Insert into "+ui->lineEdit_2->text()+" value  ";
        for(int i=2;i<=row_count;i++)
        {
            Qtext+="( ";
            for(int j=1;j<=col_count;j++)
            {
                if(datatype[j-1][0]=='v'||datatype[j-1][0]=='V'||datatype[j-1][0]=='c'||datatype[j-1][0]=='c')
                   Qtext+="'";
                Qtext+=xlsx.read(i,j).toString();
                if(datatype[j-1][0]=='v'||datatype[j-1][0]=='V'||datatype[j-1][0]=='c'||datatype[j-1][0]=='c')
                   Qtext+="'";
                if(j!=col_count)
                Qtext+=",";
            }
            if(i!=row_count)
                Qtext+="),";
        }
        Qtext+=")";
        if(!query.exec(Qtext))
            QMessageBox::warning(this,"MYSQL Error ",query.lastError().text(),QMessageBox::Ok);
        else
            QMessageBox::warning(this,"Successfull ","Query Executed!",QMessageBox::Ok);

    }

}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    QStringList dbnames;
    dbnames.append(arg1);
    dbnames.removeAll(arg1);
    QSqlQuery query;
    bool show;
    query.exec("show databases");
    while(query.next())
    {
        show=true;
        QVariant res=query.value(0);
        QString tomatch=ui->lineEdit_3->text();
        for(int i=0;i<tomatch.size();i++)
        {
            if(tomatch[i]!=res.toString()[i])
                show=false;
        }
        if(show==true)
        dbnames.append(res.toString());
    }
    model->setStringList(dbnames);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

void MainWindow::on_actionAnshika_triggered()
{
    QMessageBox::information(this,"Hello Anshika","This presentation is for you ",QMessageBox::Ok);
}
