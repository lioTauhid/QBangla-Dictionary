#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class HomeWindow; }
QT_END_NAMESPACE

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

private slots:
    void on_wordSearch_textEdited(const QString &arg1);

    void on_wordSearch_editingFinished();

    void on_actionThem_triggered();

private:
    Ui::HomeWindow *ui;
};
#endif // HOMEWINDOW_H
