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
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QSqlTableModel* suggestionModel;

HomeWindow::HomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    QSettings setting;
    QString color=setting.value("themColor").toString();
    if(color!=""){
        if(color!="Default"){
            this->setStyleSheet("color:#0000fff;");
            ui->centralwidget->setStyleSheet("background-color: '"+color+"';");
        }
    }
    createConnection();

    suggestionModel= new QSqlTableModel();
    suggestionModel->setTable("english");
    suggestionModel->select();
    ui->tableSuggestion->setModel(suggestionModel);
    ui->tableSuggestion->hideColumn(0);
}

HomeWindow::~HomeWindow()
{
    delete ui;
}


void HomeWindow::on_wordSearch_textEdited(const QString &arg1)
{
    suggestionModel->setFilter("english.word  LIKE '"+arg1+"%' LIMIT 50");
}

void HomeWindow::on_wordSearch_editingFinished()
{
    QSqlTableModel *model=new QSqlTableModel();
    QString word = ui->wordSearch->text();
    if (word.isEmpty()){
        return;
    }

    model->setTable("bangla");
    model->setFilter("serial IN (SELECT serial FROM english WHERE word LIKE '"+word+"')");
    model->select();

    ui->meaningEt->setText(model->index(0,1).data().toString());
    ui->antoEt->setText(model->index(0,4).data().toString());
    ui->definEt->setText(model->index(0,5).data().toString());
    ui->exampleEt->setText(model->index(0,6).data().toString());

    QSqlTableModel *extraModel=new QSqlTableModel();
    QString tt= "[[3,8,9],[2,10],[4,11]]";

    QJsonDocument doc = QJsonDocument::fromJson(model->index(0,2).data().toString().toUtf8());
    QJsonArray obj = doc.array();
    QString qq = "";
    for(QJsonValueRef s : obj) {
        int id = s.toArray().at(0).toInt();
        QString type = "Noun";
        extraModel->setTable("bn_en");
        for (int i=1;i<s.toArray().count();i++) {
            double j = s.toArray()[i].toDouble();
            qq.append( ","+QString::number(j));
        }
    }
    qDebug()<<qq;

    extraModel->setFilter("serial IN ("+qq.remove(0,1)+")");
    extraModel->select();
    ui->posTable->setModel(extraModel);
    ui->posTable->hideColumn(0);

//    qDebug()<<extraModel->index(0,2).data().toString();

//    qDebug() << obj[0].toArray()[0];
//    qDebug()<<ex;

//    ui->synoEt->setText(model->index(0,2).data().toString());
//    ui->posEt->setText(model->index(0,2).data().toString());
}


void HomeWindow::on_tableSuggestion_clicked(const QModelIndex &index)
{
    ui->wordSearch->setText(ui->tableSuggestion->model()->data(index).toString());
    on_wordSearch_editingFinished();
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
}


void HomeWindow::on_actionAbout_triggered()
{
    AboutDialog ab;
    ab.exec();
}


void HomeWindow::on_actionFont_install_triggered()
{
    QDesktopServices::openUrl(QUrl(QCoreApplication::applicationDirPath()+"/kalpurush.ttf", QUrl::TolerantMode));
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
