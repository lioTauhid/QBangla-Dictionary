#include "ui_homewindow.h"
#include "db_connetion.h"
#include "QInputDialog"
#include <QSettings>
#include <homewindow.h>
#include <QDesktopServices>
#include <aboutdialog.h>
#include <QMimeData>
#include <QClipboard>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QSqlTableModel>


QSqlTableModel* suggestionModel;
QClipboard *clipboard;

HomeWindow::HomeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomeWindow)
{
  ui->setupUi(this);
  QSettings setting;
  QString color = setting.value("themColor").toString();
    if (color == "White") {
        this->setStyleSheet("background-color: '"+color+"'; color: blue;");
        ui->centralwidget->setStyleSheet("background-color: '" + color + "'; color: blue;");
    } else if (color == "Dark") {
        this->setStyleSheet("background-color: '"+color+"'; color: white");
        ui->centralwidget->setStyleSheet("background-color: '" + color + "'; color: white");
    } else if (color == "Pink") {
        this->setStyleSheet("background-color: '"+color+"'; color:blue;");
        ui->centralwidget->setStyleSheet("background-color: '" + color + "'; color: blue;");
    }
  createConnection();

  suggestionModel = new QSqlTableModel();
  suggestionModel->setTable("english");
  suggestionModel->select();
  ui->tableSuggestion->setModel(suggestionModel);
  ui->tableSuggestion->hideColumn(0);
  m_speech = new QTextToSpeech(this);
  //    m_speech->setVolume(80.0);
  //    m_speech->setRate(10.0);
  //    m_speech->setPitch(10.0);
  //    m_speech->say("The word is QtTextToSpeech");

  clipboard = QApplication::clipboard();
  connect(clipboard,SIGNAL(dataChanged()), SLOT(clip_data_changed()));
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::clip_data_changed()
{
    on_pastBtn_clicked();
}


void HomeWindow::on_wordSearch_textEdited(const QString &arg1)
{
    suggestionModel->setFilter("english.word  LIKE '"+arg1+"%' LIMIT 50");
}

void HomeWindow::on_wordSearch_editingFinished()
{
    QString word = ui->wordSearch->text();
    if (word.isEmpty()){
        return;
    }
    QSqlTableModel *model=new QSqlTableModel();

    model->setTable("bangla");
    model->setFilter("serial IN (SELECT serial FROM english WHERE word LIKE '"+word+"')");
    model->select();

    ui->meaningEt->setText(model->index(0,1).data().toString());
    ui->antoEt->setText(model->index(0,4).data().toString().replace("[", "").replace("]", "").replace('"', "").replace(',', ", ").toLower());
    ui->definEt->setText("- "+model->index(0,5).data().toString().replace("[", "").replace("]", "").replace('"', "").replace(".", "\n").replace(",", "- "));
    ui->exampleEt->setText(model->index(0,6).data().toString());

    QSqlTableModel *extraModel=new QSqlTableModel();
    extraModel->setTable("bn_en");

    QJsonDocument doc = QJsonDocument::fromJson(model->index(0,2).data().toString().toUtf8());
    QJsonArray obj = doc.array();
    QString samePos;
    QString qq = "";
    QList<QString> typeSerial;
    QSqlTableModel *typeModel=new QSqlTableModel();
    typeModel->setTable("types");
    typeModel->select();
    for(QJsonValueRef s : obj) {
        QString type = typeModel->index(s.toArray().at(0).toInt()-1, 1).data().toString();
        for (int i=0;i<s.toArray().count();i++) {
            double j = s.toArray()[i].toDouble();
            if (i==0){
                if(type=="MORE"){
                    samePos = "SYNONYM";
                }else {
                    samePos = type;
                }
            }else {
                typeSerial.append(samePos);
                qq.append( ","+QString::number(j));
            }
        }
    }
    extraModel->setFilter("serial IN ("+qq.remove(0,1)+")");
    extraModel->select();
    QStandardItemModel *posModel = new QStandardItemModel(this);
    QList<QStandardItem *> items;

    for (int i=0;i<extraModel->rowCount();i++) {
        items.clear();
        posModel->setColumnCount(3);
        items.append(new QStandardItem(typeSerial[i]));
        items.append(new QStandardItem(extraModel->index(i,1).data().toString()));
        items.append(new QStandardItem(extraModel->index(i,2).data().toString().replace("[", "").replace("]", "").replace('"', "").replace(',', ", ").toLower()));
        posModel->appendRow(items);
    }
    ui->posTable->setModel(posModel);
//    ui->posTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->posTable->setColumnWidth(0, (ui->posTable->width()*20)/100);
    ui->posTable->setColumnWidth(1, (ui->posTable->width()*30)/100);
    ui->posTable->setColumnWidth(2, (ui->posTable->width()*50)/100);
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
      if (color == "White") {
          this->setStyleSheet("background-color: '"+color+"'; color: blue;");
          ui->centralwidget->setStyleSheet("background-color: '" + color + "'; color: blue;");
      } else if (color == "Dark") {
          this->setStyleSheet("background-color: '"+color+"'; color: white");
          ui->centralwidget->setStyleSheet("background-color: '" + color + "'; color: white");
      } else if (color == "Pink") {
          this->setStyleSheet("background-color: '"+color+"'; color:blue;");
          ui->centralwidget->setStyleSheet("background-color: '" + color + "'; color: blue;");
      } else {
          this->setStyleSheet(" ");
          ui->centralwidget->setStyleSheet("color:#0000ff; background-image: url(:/assets/Gradient-Abstract-Shapes-Background-Purple.jpg);");
      }
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
    ui->wordSearch->setText(clipboard->mimeData()->text());
    on_wordSearch_editingFinished();
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
    ui->posTable->setModel(NULL);
}

void HomeWindow::on_speakBtn_clicked()
{
//    m_speech->say(" ,"+ui->wordSearch->text());
    m_speech->say(ui->wordSearch->text());

}




