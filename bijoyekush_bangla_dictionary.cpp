#include "bijoyekush_bangla_dictionary.h"
#include "ui_bijoyekush_bangla_dictionary.h"
#include "about.h"
#include "help.h"

BijoyEkush_Bangla_Dictionary::BijoyEkush_Bangla_Dictionary(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BijoyEkush_Bangla_Dictionary)
{
    ui->setupUi(this);
    QSqlDatabase mydb=QSqlDatabase::addDatabase("QSQLITE");

    mydb.setDatabaseName("dictionary.db");

    if(!mydb.open())
        ui->label->setText("Database missing");
    else
        ui->label->setText("We are added 100000+ word,also phrases");

}

BijoyEkush_Bangla_Dictionary::~BijoyEkush_Bangla_Dictionary()
{
    delete ui;
}

void BijoyEkush_Bangla_Dictionary::on_lineEdit_editingFinished()
{
    QString wordui =ui->lineEdit->text();
    QSqlQuery query,query3;
    query.exec("SELECT b FROM english WHERE e LIKE '"+wordui+"'");
            while (query.next()) {
            QString wordui = query.value(0).toString();

            ui->textEdit->setText(wordui); }

            query3.exec("SELECT b FROM word WHERE e LIKE '"+wordui+"' ");
                        if (query3.next()){
                            QString wordui =query3.value(0).toString();
                            ui->textEdit_2->setText(wordui);
                                                         }
                        else {
                            ui->textEdit_2->setText(0);
                        }
}

void BijoyEkush_Bangla_Dictionary::on_lineEdit_textEdited(const QString &arg1)
{

    QString ex2 =ui->lineEdit->text();
    QSqlQueryModel * modal=new QSqlQueryModel();
    QSqlQuery* qry=new QSqlQuery();
    qry->prepare("select e,b from english WHERE e LIKE '"+ex2+"%'");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);

}

void BijoyEkush_Bangla_Dictionary::on_tableView_pressed(const QModelIndex &index)
{
    QString val=ui->tableView->model()->data(index).toString();
           ui->lineEdit->setText(val);

           QSqlQuery qr,qr2;

           qr.exec("SELECT b FROM english WHERE e LIKE '"+val+"'");
           while (qr.next()) {
           QString val2 =qr.value(0).toString();

           ui->textEdit->setText(val2);
                                          }

           qr2.exec("SELECT b FROM word WHERE e LIKE '"+val+"'");
           if (qr2.next()) {
           QString val3 =qr2.value(0).toString();

           ui->textEdit_2->setText(val3);}
           else
               ui->textEdit_2->setText(0);
}

void BijoyEkush_Bangla_Dictionary::on_pushButton_4_clicked()
{
   About ab;
   ab.setModal(true);
   ab.exec();
}

void BijoyEkush_Bangla_Dictionary::on_pushButton_3_clicked()
{
    Help h;
    h.setModal(true);
    h.exec();
}
