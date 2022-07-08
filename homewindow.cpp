#include "ui_homewindow.h"
#include "db_connetion.h"
#include "QInputDialog"
#include <QSettings>
#include <QSqlRelationalTableModel>
#include <homewindow.h>
#include <QDesktopServices>
#include <aboutdialog.h>
#include <QMimeData>
#include <QClipboard>

QSqlTableModel* suggestionModel;

HomeWindow::HomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    QSettings setting;
    QString color=setting.value("themColor").toString();
    if(color!="Default"){
        this->setStyleSheet("color:#0000fff;");
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
    suggestionModel->setFilter("EnBnAnt1.word  LIKE '"+arg1+"%' LIMIT 50");
}

void HomeWindow::on_wordSearch_editingFinished()
{
    QSqlRelationalTableModel *model=new QSqlRelationalTableModel();
    QSqlRelationalTableModel *model2=new QSqlRelationalTableModel();
    QSqlTableModel *exampleModel= new QSqlTableModel();
    QString word = ui->wordSearch->text();

    exampleModel= new QSqlTableModel();
    exampleModel->setTable("Examples");
    exampleModel->select();

    model->setTable("BnSynPos1");
    model->setFilter("BnSynPos1.pron = '"+word.toLower()+"'");
    model->setRelation(0, QSqlRelation("BnSynPos2", "id", "meaning"));
    model->select();

    model2->setTable("EnBnAnt1");
    model2->setFilter("EnBnAnt1.word LIKE '"+word+"'");
    model2->setRelation(0, QSqlRelation("EnBnAnt2", "id", "meaning"));
    model2->select();

    if(model->rowCount()==0){
        ui->meaningEt->setText(model2->index(0,0).data().toString());

    }else {
        ui->meaningEt->setText(model->index(0,0).data().toString());
        ui->synoEt->setText(model->index(0,2).data().toString());
    }
    ui->antoEt->setText(model2->index(0,2).data().toString());

    exampleModel->setFilter("Examples.word LIKE '"+word.toUpper()+"%'");
    ui->exampleEt->setText(exampleModel->index(0,1).data().toString());
}

void HomeWindow::on_actionThem_triggered()
{
    QStringList thems;
    thems<<"Default"<<"White"<<"Dark"<<"Pink";
    QString color = QInputDialog::getItem(NULL, tr("Choose Theme"), tr("Theme:"), thems);
    QSettings setting;
    setting.setValue("themColor", color);
    if(color!="Default"){
        ui->centralwidget->setStyleSheet("background-color: '"+color+"';");
        return;
    }
    ui->centralwidget->setStyleSheet("background-image: url(:/Gradient-Abstract-Shapes-Background-Purple.jpg);");
}


void HomeWindow::on_actionLanguage_triggered()
{
    QStringList langs;
    langs<<"English"<<"বাংলা";
    QString lang = QInputDialog::getItem(NULL, tr("Choose Language"), tr("Language:"), langs);
    QSettings setting;
    setting.setValue("lang", lang);
    QMessageBox::information(this, "Warning!", "Restart app menually, to change language.", QMessageBox::Ok);
    QString lang2=setting.value("lang").toString();
}


void HomeWindow::on_actionAbout_triggered()
{
    AboutDialog ab;
    ab.exec();
}


void HomeWindow::on_actionFont_install_triggered()
{
    QDesktopServices::openUrl(QUrl("kalpurush.ttf", QUrl::TolerantMode));
}


void HomeWindow::on_actionExit_triggered()
{
    this->close();
}


void HomeWindow::on_pastBtn_clicked()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    ui->wordSearch->setText(mimeData->text());
}


void HomeWindow::on_searchBtn_clicked()
{
    on_wordSearch_editingFinished();
}


void HomeWindow::on_clearBtn_clicked()
{
    foreach(QTextEdit *widget, this->findChildren<QTextEdit*>()) {
        widget->clear();
    }
    ui->wordSearch->setText("");
}


void HomeWindow::on_tableSuggestion_clicked(const QModelIndex &index)
{
    ui->wordSearch->setText(ui->tableSuggestion->model()->data(index).toString());
    on_wordSearch_editingFinished();
}
