#ifndef DIALOGPICK_H
#define DIALOGPICK_H

#include <QAbstractButton>
#include <QDialog>

namespace Ui {
class DialogPick;
}

class DialogPick : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPick(QWidget *parent = nullptr);
    ~DialogPick();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::DialogPick *ui;
};

#endif // DIALOGPICK_H
