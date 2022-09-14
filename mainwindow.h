#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <bitset>
#include <map>
#include <helpwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_Compile_Btn_clicked();

    void on_Run_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_actionReset_triggered();

    void on_actionHelp_triggered();

    void on_actionCreators_triggered();

    void on_save_clicked();

private:

    void display();

    Ui::MainWindow *ui;
    int ctr;
    int lineCounter;
    int execution;
    int compiled;
    int ramStart;
    int ramEnd;
    int dataFlag;

    // REGISTERS
    std::bitset<16> ram[4096];
    std::bitset<16> AC;
    std::bitset<16> DR;
    std::bitset<12> AR;
    std::bitset<16> IR;
    std::bitset<12> PC;
    std::bitset<16> TR;
    std::bitset<8> INPR;
    std::bitset<8> OUTR;

    //FLAGS
    bool E;
    bool FGI;
    bool FGO;
    bool IEN;
    bool flagISZ;

    std::map<std::string, int> variable;
    std::map<std::string, int> branchLocation;
    std::map<std::string, int> variableLocation;

    //HELP WINDOW
    helpWindow* help;
};
#endif // MAINWINDOW_H
