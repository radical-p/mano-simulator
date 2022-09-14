#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QMainWindow>

namespace Ui {
class helpWindow;
}

class helpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit helpWindow(QWidget *parent = nullptr);
    ~helpWindow();

private:
    Ui::helpWindow *ui;
};

#endif // HELPWINDOW_H
