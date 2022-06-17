#include "ui_homewindow.h"
#include "db_connetion.h"

#include <QSettings>
#include <QSqlRelationalTableModel>
#include <dialogpick.h>
#include <homewindow.h>

QSqlTableModel* suggestionModel;

HomeWindow::HomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Bijoy Ekush Bangla Dictionary");
    QSettings setting;
    QString color=setting.value("themColor").toString();
    if(color!="default"){
        this->setStyleSheet("background-color: white;");
        ui->centralwidget->setStyleSheet("background-color: '"+color+"';");
    }
    createConnection();

    suggestionModel= new QSqlTableModel();
    suggestionModel->setTable("EnBnAnt1");
    suggestionModel->select();

    ui->tableSuggestion->setModel(suggestionModel);
    ui->tableSuggestion->hideColumn(0);
    ui->tableSuggestion->hideColumn(2);
}

HomeWindow::~HomeWindow()
{
    delete ui;
}


void HomeWindow::on_wordSearch_textEdited(const QString &arg1)
{
    suggestionModel->setFilter("EnBnAnt1.word  LIKE '"+arg1+"%'");
}

void HomeWindow::on_wordSearch_editingFinished()
{
    QSqlRelationalTableModel *model=new QSqlRelationalTableModel();
    QSqlRelationalTableModel *model2=new QSqlRelationalTableModel();
    QSqlTableModel *exampleModel= new QSqlTableModel();

    exampleModel= new QSqlTableModel();
    exampleModel->setTable("Examples");
    exampleModel->select();

    model->setTable("BnSynPos1");
    model->setFilter("BnSynPos1.pron = '"+ui->wordSearch->text()+"'");
    model->setRelation(0, QSqlRelation("BnSynPos2", "id", "meaning"));
    model->select();

    model2->setTable("EnBnAnt1");
    model2->setFilter("EnBnAnt1.word LIKE '"+ui->wordSearch->text()+"'");
    model2->setRelation(0, QSqlRelation("EnBnAnt2", "id", "meaning"));
    model2->select();

    if(model->rowCount()==0){
        ui->meaningEt->setText(model2->index(0,0).data().toString());

    }else {
        ui->meaningEt->setText(model->index(0,0).data().toString());
        ui->synoEt->setText(model->index(0,2).data().toString());
    }
    ui->antoEt->setText(model2->index(0,2).data().toString());

    exampleModel->setFilter("Examples.word LIKE '"+ui->wordSearch->text()+"%'");
    ui->exampleEt->setText(exampleModel->index(0,1).data().toString());

}

void HomeWindow::on_actionThem_triggered()
{
    DialogPick dd;
    if(dd.exec()==QDialog::Accepted){
        QSettings setting;
        QString color=setting.value("themColor").toString();
        if(color!="default"){
//            this->setStyleSheet("background-color: white;");
            ui->centralwidget->setStyleSheet("background-color: '"+color+"';");
        }
    }
}

