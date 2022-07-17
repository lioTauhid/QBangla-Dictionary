#ifndef BIJOYEKUSH_BANGLA_DICTIONARY_H
#define BIJOYEKUSH_BANGLA_DICTIONARY_H

#include <QMainWindow>
#include<QtSql>

#include<QFileInfo>

namespace Ui {
class BijoyEkush_Bangla_Dictionary;
}

class BijoyEkush_Bangla_Dictionary : public QMainWindow
{
    Q_OBJECT

public:
    explicit BijoyEkush_Bangla_Dictionary(QWidget *parent = 0);
    ~BijoyEkush_Bangla_Dictionary();

private slots:
    void on_lineEdit_editingFinished();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_tableView_pressed(const QModelIndex &index);

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::BijoyEkush_Bangla_Dictionary *ui;
};

#endif // BIJOYEKUSH_BANGLA_DICTIONARY_H
