#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <map>
#include <stdlib.h>
#include <QPixmap>
#include <QShortcut>
#include <QTableWidget>
#include <QMessageBox>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{


    ui->setupUi(this);


    this->setFixedSize(1280, 660);


    //main icon
    setWindowIcon(QIcon("C:/Users/Radikal/Documents/GitHub/computer-architecture-/pics/mainIcon.png"));


    //Background
    QPixmap bkgnd("C:/Users/Radikal/Documents/GitHub/computer-architecture-/pics/BCK.jpg");
              bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
              QPalette palette;
              palette.setBrush(QPalette::Window/*Background*/, bkgnd);
              this->setPalette(palette);



    setWindowTitle("Mano Simulator");

    ui->pushButton->setDefault(true);

    ui->pushButton->setDefault(true);

    ui->pushButton->setFocus();

    ui->tableWidget->setRowCount(1);

    ui->tableWidget->setColumnCount(4);

    ui->tableWidget_2->setColumnCount(3);

    ui->label_19->setText(QString::number(0));


    //Counter reset
    lineCounter = 0;

    E = 0;

    execution = 0;

    compiled = 0;

    ctr = 1;

    dataFlag = 0;


    //Register reset
    AR = 0;

    PC = 0;

    DR = 0;

    AC = 0;

    INPR = 0;

    OUTR = 0;

    TR = 0;


    display();

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->tableWidget->insertRow(ctr);
    ctr++;
}


template <size_t N>
std::bitset<N> increment ( std::bitset<N> in ) {

    for ( size_t i = 0; i < N; ++i ) {
        if ( in[i] == 0 ) {
            in[i] = 1;
            break;
            }
        in[i] = 0;
        }
    return in;

    }


std::string decToHexa(int num)
{


    std::map<int, char> m;

    char digit = '0';
    char c = 'a';

    for (int i = 0; i <= 15; i++)
    {
        if (i < 10)
        {
            m[i] = digit++;
        }
        else
        {
            m[i] = c++;
        }
    }

    std::string res = "";

    if (!num)
    {
        return "0";
    }


    if (num > 0)
    {
        while (num)
        {
            res = m[num % 16] + res;
            num /= 16;
        }
    }


    else
    {

        uint n = num;


        while (n)
        {
            res = m[n % 16] + res;
            n /= 16;
        }
    }

    return res;
}


void MainWindow::on_Compile_Btn_clicked()
{

    if (compiled)
        return;

     ui->listWidget->clear();

    QString InstructionStr;

    QTableWidgetItem *label = ui->tableWidget->item(execution, 0);

    QTableWidgetItem *instruction = ui->tableWidget->item(execution, 1);

    QTableWidgetItem *operand = ui->tableWidget->item(execution, 2);

    QTableWidgetItem *indirect = ui->tableWidget->item(execution, 3);


    if (instruction)
        InstructionStr = instruction->text();


    InstructionStr = InstructionStr.toUpper();


    // ORG
    if (InstructionStr == "ORG")
    {

        int start = operand->text().toInt();

        for (int j = 0; j < 4096; j++)
        {
            ui->tableWidget_2->insertRow(j);
        }

        lineCounter = start - 1;

        ramStart = lineCounter;

    }
    // memory reference instructions
    else if (InstructionStr == "LDA")
    {

        if(!indirect)
        {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 : NOP");

        ui->listWidget->addItem("T4 : DR <- M[AR]");

        ui->listWidget->addItem("T5 : AC <- DR, SC <- 0");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("LDA " + operand->text());

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *HEXADR = new QTableWidgetItem;

        HEXADR->setText("2" + QString::number(variableLocation.at(operand->text().toStdString())));

        ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


        lineCounter++;


        int ldaValue = variable.at(operand->text().toStdString());

        AC = std::bitset<16>(ldaValue);

        }
        else
        {

            ui->listWidget->addItem("T0 FETCH: AR <- PC");

            ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

            ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

            ui->listWidget->addItem("T3 : AR <- M[AR]");

            ui->listWidget->addItem("T4 : DR <- M[AR]");

            ui->listWidget->addItem("T5 : AC <- DR, SC <- 0");


            int ldaloc = variable.at(operand->text().toStdString());


                for (auto& it : variableLocation) {

                    if (it.second == ldaloc) {

                        std::string varName = it.first;

                        int ldaValue = variable.at(varName);

                        AC = std::bitset<16>(ldaValue);

                    }
                }


                QTableWidgetItem *newItem = new QTableWidgetItem;

                newItem->setText("LDA " + operand->text() + " I");

                ui->tableWidget_2->setItem(lineCounter, 1, newItem);


                QTableWidgetItem *HEXADR = new QTableWidgetItem;

                HEXADR->setText("A" + QString::number(variableLocation.at(operand->text().toStdString())));

                ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


                lineCounter++;

        }


    }
    else if (InstructionStr == "AND")
    {


        if (!indirect)
        {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 : NOP");

        ui->listWidget->addItem("T4 : DR <- M[AR]");

        ui->listWidget->addItem("T5 : AC <- AC ^ DR, SC <- 0");


        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("AND " + operand->text());

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *HEXADR = new QTableWidgetItem;

        HEXADR->setText("0" + QString::number(variableLocation.at(operand->text().toStdString())));

        ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


        lineCounter++;


        int andValue = variable.at(operand->text().toStdString());

        DR = std::bitset<16>(andValue);


        for (int i = 0; i < 16; i++)
        {
            AC[i] = AC[i] and DR[i];
        }

        }
        else
        {

            ui->listWidget->addItem("T0 FETCH: AR <- PC");

            ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

            ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

            ui->listWidget->addItem("T3 : AR <- M[AR]");

            ui->listWidget->addItem("T4 : DR <- M[AR]");

            ui->listWidget->addItem("T5 : AC <- AC ^ DR, SC <- 0");

            int andloc = variable.at(operand->text().toStdString());


                for (auto& it : variableLocation) {

                    if (it.second == andloc) {

                        std::string varName = it.first;

                        int andValue = variable.at(varName);

                        DR = std::bitset<16>(andValue);


                        for (int i = 0; i < 16; i++)
                        {
                            AC[i] = AC[i] and DR[i];
                        }

                    }
                }


                QTableWidgetItem *newItem = new QTableWidgetItem;

                newItem->setText("AND " + operand->text() + " I");

                ui->tableWidget_2->setItem(lineCounter, 1, newItem);


                QTableWidgetItem *HEXADR = new QTableWidgetItem;

                HEXADR->setText("8" + QString::number(variableLocation.at(operand->text().toStdString())));

                ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


                lineCounter++;
        }

    }
    else if (InstructionStr == "ADD")
    {

        if(!indirect)
        {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 : NOP");

        ui->listWidget->addItem("T4 : DR <- M[AR]");

        ui->listWidget->addItem("T5 : AC <- AC + DR, E <- Cout, SC <- 0");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("ADD " + operand->text());

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *HEXADR = new QTableWidgetItem;

        HEXADR->setText("1" + QString::number(variableLocation.at(operand->text().toStdString())));

        ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


        lineCounter++;


        int addValue = variable.at(operand->text().toStdString());

        DR = std::bitset<16>(addValue);


        bool carry = false;

        for (int i = 0; i < 16; i++)
        {

            bool tmp = AC[i];

            AC[i] = AC[i] xor DR[i] xor carry;

            carry = tmp and DR[i] or carry and DR[i] or tmp and carry;
        }

        E = carry;

        }
        else
        {

            ui->listWidget->addItem("T0 FETCH: AR <- PC");

            ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

            ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

            ui->listWidget->addItem("T3 : AR <- M[AR]");

            ui->listWidget->addItem("T4 : DR <- M[AR]");

            ui->listWidget->addItem("T5 : AC <- AC + DR, E <- Cout, SC <- 0");


            int addloc = variable.at(operand->text().toStdString());


                for (auto& it : variableLocation) {

                    if (it.second == addloc) {

                        std::string varName = it.first;

                        int addValue = variable.at(varName);

                        DR = std::bitset<16>(addValue);

                        bool carry = false;

                        for (int i = 0; i < 16; i++)
                        {

                            bool tmp = AC[i];

                            AC[i] = AC[i] xor DR[i] xor carry;

                            carry = tmp and DR[i] or carry and DR[i] or tmp and carry;
                        }

                        E = carry;

                    }
                }


                QTableWidgetItem *newItem = new QTableWidgetItem;

                newItem->setText("ADD " + operand->text() + " I");

                ui->tableWidget_2->setItem(lineCounter, 1, newItem);


                QTableWidgetItem *HEXADR = new QTableWidgetItem;

                HEXADR->setText("9" + QString::number(variableLocation.at(operand->text().toStdString())));

                ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


                lineCounter++;

        }


    }
    else if (InstructionStr == "STA")
    {

        if(!indirect){

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 : NOP");

        ui->listWidget->addItem("T4 : M[AR] <- AC, SC <- 0");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("STA " + operand->text());

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *HEXADR = new QTableWidgetItem;

        HEXADR->setText("3" + QString::number(variableLocation.at(operand->text().toStdString())));

        ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


        lineCounter++;


        int locValue = variableLocation.at(operand->text().toStdString());

        AR = std::bitset<12>(locValue);

        ram[locValue] = AC;

        variable.at(operand->text().toStdString()) = AC.to_ulong();

        }
        else
        {

            ui->listWidget->addItem("T0 FETCH: AR <- PC");

            ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

            ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

            ui->listWidget->addItem("T3 : AR <- M[AR]");

            ui->listWidget->addItem("T4 : M[AR] <- AC, SC <- 0");


            int staloc = variable.at(operand->text().toStdString());


                            for (auto& it : variableLocation) {

                                if (it.second == staloc) {

                                    std::string varName = it.first;

                                    int locValue = variableLocation.at(varName);

                                    AR = std::bitset<12>(locValue);

                                    ram[locValue] = AC;

                                    variable.at(operand->text().toStdString()) = AC.to_ulong();

                                }
                            }


                            QTableWidgetItem *newItem = new QTableWidgetItem;

                            newItem->setText("STA " + operand->text() + " I");

                            ui->tableWidget_2->setItem(lineCounter, 1, newItem);


                            QTableWidgetItem *HEXADR = new QTableWidgetItem;

                            HEXADR->setText("B" + QString::number(variableLocation.at(operand->text().toStdString())));

                            ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


                            lineCounter++;
        }



    }
    else if (InstructionStr == "BUN")
    {

        if (!indirect)
        {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 : NOP");

        ui->listWidget->addItem("T4 : PC <- AR, SC <- 0");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("BUN " + operand->text());

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *HEXADR = new QTableWidgetItem;

        HEXADR->setText("4" + QString::number(branchLocation.at(operand->text().toStdString())));

        ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


        lineCounter++;


        int mv = branchLocation.at(operand->text().toStdString());        

        execution = mv - 1;

        }

    }
    else if (InstructionStr == "BSA")
    {


        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 : NOP");

        ui->listWidget->addItem("T4 : M[AR] <- PC, AR <- AR + 1");

        ui->listWidget->addItem("T5 : PC <- AR, SC <- 0");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("BSA " + operand->text());

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *HEXADR = new QTableWidgetItem;

        HEXADR->setText("5" + QString::number(variableLocation.at(operand->text().toStdString())));

        ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


        lineCounter++;


        //ui->lineEdit->setText("SALAM");

        //_sleep(5000);

        //int mv = variableLocation.at(operand->text().toStdString());

       // ui->lineEdit->setText(QString::number(mv));

        //_sleep(5000);

       // ui->tableWidget_2->item(mv - 1, 2)->setText(QString::number(execution + 1));

       // branchLocation.at(operand->text().toStdString()) = execution + 1;

       // execution = mv - 1;


    }
    else if (InstructionStr == "ISZ")
    {

        if(!indirect)
        {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 : NOP");

        ui->listWidget->addItem("T4 : DR <- M[AR]");

        ui->listWidget->addItem("T5 : DR <- DR + 1");

        ui->listWidget->addItem("T6 : M[AR] <- DR, SC <- 0, if (Z == 1) PC <- PC + 1");


        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("ISZ " + operand->text());

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *HEXADR = new QTableWidgetItem;

        HEXADR->setText("6" + QString::number(variableLocation.at(operand->text().toStdString())));

        ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);


        lineCounter++;


        DR = std::bitset<16>(variable.at(operand->text().toStdString()));

        DR = increment(DR);

        int converted = std::stoi(DR.to_string(), nullptr, 2);

        variable.at(operand->text().toStdString()) = converted;

        if (DR == 0000000000000000)
        {
            execution++;
        }

    }
     else{

            ui->listWidget->addItem("T0 FETCH: AR <- PC");

            ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

            ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

            ui->listWidget->addItem("T3 : AR <- M[AR]");

            ui->listWidget->addItem("T4 : DR <- M[AR]");

            ui->listWidget->addItem("T5 : DR <- DR + 1");

            ui->listWidget->addItem("T6 : M[AR] <- DR, SC <- 0, if (Z == 1) PC <- PC + 1");


        int iszloc = variable.at(operand->text().toStdString());


         for (auto& it : variableLocation) {

               if (it.second == iszloc) {

               std::string varName = it.first;

               DR = std::bitset<16>(variable.at(varName));

               DR = increment(DR);

               int converted = std::stoi(DR.to_string(), nullptr, 2);

               variable.at(varName) = converted;

               if (DR == 0000000000000000)
               {
                       execution++;
               }

               }

          }


          QTableWidgetItem *newItem = new QTableWidgetItem;

          newItem->setText("ISZ " + operand->text() + " I");

          ui->tableWidget_2->setItem(lineCounter, 1, newItem);



          QTableWidgetItem *HEXADR = new QTableWidgetItem;

          HEXADR->setText("E" + QString::number(variableLocation.at(operand->text().toStdString())));

          ui->tableWidget_2->setItem(lineCounter, 2, HEXADR);



          lineCounter++;
    }

    }
    ////////////////////////////////////////////////////////////////////////////
    // register reference instructions
    else if (InstructionStr == "CLA")
    {


        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 CLEAR AC: AC <- 0");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("CLA");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7800");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        AC = std::bitset<16>(0);

    }
    else if (InstructionStr == "CLE")
    {


        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 CLEAR E: E <- 0");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("CLE");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7400");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        E = 0;
    }
    else if (InstructionStr == "CMA")
    {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 COMPLEMENT AC: AC <- !AC");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("CMA");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7200");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        for (int k = 0; k < 16; k++)
        {
            if (AC[k] == 1)
                AC[k] = 0;
            else
                AC[k] = 1;
        }


    }
    else if (InstructionStr == "CME")
    {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 COMPLEMENT E: E <- !E");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("CME");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7100");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        if (E == 1)
            E = 0;
        else
            E = 1;

    }
    else if (InstructionStr == "CIR")
    {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 CIRCULATE RIGHT: AC <- shr AC, AC(15) <- E, E <- AC(0)");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("CIR");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7080");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        bool tmpCarry = E;

        E = AC[0];

        for (int k = 0; k < 15; k++)
        {
            AC[k] = AC[k + 1];
        }

        AC[15] = tmpCarry;


    }
    else if (InstructionStr == "CIL")
    {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 CIRCULATE LEFT: AC <- shL AC, AC(0) <- E, E <- AC(15)");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("CIL");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7040");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        bool tmpCarry = E;

        E = AC[15];

        for (int k = 15; k > 0; k--)
        {
            AC[k] = AC[k - 1];
        }

        AC[0] = tmpCarry;


    }
    else if (InstructionStr == "INC")
    {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 INCREMENT AC: AC <- AC + 1");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("INC");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7020");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        AC = increment(AC);


    }
    else if (InstructionStr == "SPA")
    {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 SKIP IF POSITIVE: if (AC(15) == 0) then PC <- PC + 1");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("SPA");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7010");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        if (AC[15] == 0)
        {
            execution++;
        }

    }
    else if (InstructionStr == "SNA")
    {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 SKIP IF NEGATIVE: if (AC(15) == 1) then PC <- PC + 1");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("SNA");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7008");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        if (AC[15] == 1)
        {
            execution++;
        }

    }
    else if (InstructionStr == "SZA")
    {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 SKIP IF AC ZERO: if (AC == 0) then PC <- PC + 1");


        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("SZA");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7004");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        if (AC == 0)
        {
            execution++;
        }

    }
    else if (InstructionStr == "SZE")
    {

        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 SKIP IF E ZERO: if (E == 0) then PC <- PC + 1");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("SZE");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7002");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        if (E == 0)
        {
            execution++;
        }

    }
    else if (InstructionStr == "HLT")
    {

        dataFlag = 1;


        ui->listWidget->addItem("T0 FETCH: AR <- PC");

        ui->listWidget->addItem("T1 FETCH: IR <- M[AR], PC <- PC + 1");

        ui->listWidget->addItem("T2 DECODE:AR <- IR(0-11) , I <- IR(15)");

        ui->listWidget->addItem("T3 HALT COMPUTER: S <- 0");



        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("HLT");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("7001");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;

    }
    // I/O reference instructions
    else if (InstructionStr == "INP")
    {

        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("INP");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("F800");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        INPR = std::bitset<8>(ui->lineEdit_2->text().toInt());

        for (int k = 0; k < 8; k++)
        {

            AC[k] = INPR[k];
        }

        FGI = 0;

    }
    else if (InstructionStr == "OUT")
    {

        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("OUT");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("F400");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        for (int k = 0; k < 8; k++)
        {

            OUTR[k] = AC[k];
        }

        FGO = 0;

    }
    else if (InstructionStr == "SKI")
    {

        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("SKI");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("F200");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        if (FGI == 1)
        {
            execution++;
        }

    }
    else if (InstructionStr == "SKO")
    {

        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("SKO");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("F100");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        if (FGO == 0)
        {
            execution++;
        }

    }
    else if (InstructionStr == "ION")
    {

        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("ION");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("F080");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        IEN = 1;

    }
    else if (InstructionStr == "IOF")
    {

        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("IOF");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("F040");

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        lineCounter++;


        IEN = 0;

    }
    else if (InstructionStr == "END")
    {

        compiled = 1;

        ramEnd = --lineCounter;

        QMessageBox::information(this,"Success","Executed Successfully!");

    }
    else if(InstructionStr == "HEX"){

        if(dataFlag)
        {

            //data label
            QTableWidgetItem *newItem3 = new QTableWidgetItem;

            newItem3->setText(label->text());

            ui->tableWidget_2->setItem(lineCounter, 0, newItem3);


            //data unit
            QTableWidgetItem *newItem = new QTableWidgetItem;

            newItem->setText("HEX");

            ui->tableWidget_2->setItem(lineCounter, 1, newItem);


            //value
            QTableWidgetItem *newItem2 = new QTableWidgetItem;

            newItem2->setText(QString::number(variable.at(label->text().toStdString())));

            ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        }
        else
        {

        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("HEX");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText(operand->text());

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        QTableWidgetItem *newItem3 = new QTableWidgetItem;

        newItem3->setText(label->text());

        ui->tableWidget_2->setItem(lineCounter, 0, newItem3);

        }


        lineCounter++;


    }
    else if(InstructionStr == "DEC"){

        if(dataFlag)
        {

            //data label
            QTableWidgetItem *newItem3 = new QTableWidgetItem;

            newItem3->setText(label->text());

            ui->tableWidget_2->setItem(lineCounter, 0, newItem3);


            //data unit
            QTableWidgetItem *newItem = new QTableWidgetItem;

            newItem->setText("DEC");

            ui->tableWidget_2->setItem(lineCounter, 1, newItem);


            //value
            QTableWidgetItem *newItem2 = new QTableWidgetItem;

            newItem2->setText(QString::number(variable.at(label->text().toStdString())));

            ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        }
        else
        {

        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText("DEC");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText(operand->text());

        ui->tableWidget_2->setItem(lineCounter, 2, newItem2);


        QTableWidgetItem *newItem3 = new QTableWidgetItem;

        newItem3->setText(label->text());

        ui->tableWidget_2->setItem(lineCounter, 0, newItem3);

        }


        lineCounter++;

    }
    else if (InstructionStr[0] >= 'A' && InstructionStr[0] <= 'Z')
    {

        QTableWidgetItem *newItem = new QTableWidgetItem;

        newItem->setText(label->text());

        ui->tableWidget_2->setItem(lineCounter, 0, newItem);


        QTableWidgetItem *newItem2 = new QTableWidgetItem;

        newItem2->setText("HEX");

        ui->tableWidget_2->setItem(lineCounter, 1, newItem2);


        QTableWidgetItem *newItem3 = new QTableWidgetItem;

        newItem3->setText(QString::number(variable.at(label->text().toStdString())));

        ui->tableWidget_2->setItem(lineCounter, 2, newItem3);


        lineCounter++;

    }

    execution++;


    display();

}


int HexToDec(std::string n) { return stoi(n, 0, 16); }


void MainWindow::on_Run_clicked()
{
    if (ctr == 1)
    {
        QMessageBox::critical(this,"Error","Instruction list is Empty!");

        return;
    }

    QString str;

    int i = 0;

    int LC = 0;

    while (true)
    {

        // ORG, set line counter
        if (i == 0)
        {
            QTableWidgetItem *itm = ui->tableWidget->item(i, 2);

            LC = itm->text().toInt();

            i++;
        }
        else
        {

            QTableWidgetItem *branchlabel = ui->tableWidget->item(i, 0);

            QTableWidgetItem *itm = ui->tableWidget->item(i, 1);

            QTableWidgetItem *operand = ui->tableWidget->item(i, 2);

            int casted = 0;

            if (itm->text().toUpper() == "HEX"){

                casted = HexToDec(operand->text().toStdString());

            }
            else if(itm->text().toUpper() == "DEC"){

                casted = operand->text().toInt();

            }


            if (itm)
            {
                str = itm->text();
                i++;
                if (str.toUpper() == "HLT")
                {
                    LC++;
                    break;
                }
            }


            if (branchlabel)
            {
                if(itm->text().toUpper() == "HEX" || itm->text().toUpper() == "DEC"){

                      branchLocation.insert(std::pair<std::string, int>(branchlabel->text().toStdString(), casted));

                }
                else{
                      branchLocation.insert(std::pair<std::string, int>(branchlabel->text().toStdString(), LC));
                }
            }


            LC++;


        }
    }


    // variable mapping
    QTableWidgetItem *label = new QTableWidgetItem;

    QTableWidgetItem *radix = new QTableWidgetItem;

    QTableWidgetItem *value = new QTableWidgetItem;

    while (true)
    {


        label = ui->tableWidget->item(i, 0);

        radix = ui->tableWidget->item(i, 1);

        value = ui->tableWidget->item(i, 2);


        str = radix->text();


        if (str.toUpper() == "END")
            break;

        int hexToDecCasted = 0;

        if (str.toUpper() == "HEX")
        {
            hexToDecCasted = HexToDec(value->text().toStdString());
        }
        else if (str.toUpper() == "DEC")
        {
            hexToDecCasted = value->text().toInt();
        }
        else{
            i++;

            LC++;

            continue;

        }

        variable.insert(std::pair<std::string, int>(label->text().toStdString(), hexToDecCasted));

        variableLocation.insert(std::pair<std::string, int>(label->text().toStdString(), LC++));


        i++;

    }

    QMessageBox::information(this,"Success","Compiled Successfully!");


}


void MainWindow::display()
{
    //AR
    ui->label_3->setText(QString::fromStdString(this->AR.to_string()));

    //PC
    ui->label_4->setText(QString::fromStdString(this->PC.to_string()));

    //DR
    ui->label_13->setText(QString::fromStdString(this->DR.to_string()));

    //AC
    ui->label_14->setText(QString::fromStdString(this->AC.to_string()));

    //E
    ui->label_15->setText(QString::number(this->E));

    //IR
    ui->label_16->setText(QString::fromStdString(this->IR.to_string()));

    //TR
    ui->label_17->setText(QString::fromStdString(this->TR.to_string()));

    //OUTR
    ui->label_18->setText(QString::fromStdString(this->OUTR.to_string()));

}


void MainWindow::on_pushButton_2_clicked()
{
    FGI = !FGO;

    QMessageBox::information(this,"FGI State","FGI toggled, FGI = " + QString::number(FGI));
}


void MainWindow::on_pushButton_3_clicked()
{
    FGO = !FGO;

    QMessageBox::information(this,"FGO State","FGO toggled, FGO = " + QString::number(FGO));
}


void MainWindow::on_actionReset_triggered()
{

    //RESET TABLES
    ui->tableWidget->clearContents();

    ui->tableWidget->setRowCount(1);



    ui->tableWidget_2->clearContents();

    ui->tableWidget_2->setRowCount(0);



    //RESET FLAGS
    lineCounter = 0;

    E = 0;

    execution = 0;

    compiled = 0;

    ctr = 1;


    //RESET REGISTERS
    AR = 0;

    PC = 0;

    DR = 0;

    AC = 0;

    INPR = 0;

    OUTR = 0;

    TR = 0;


    //CLEAR MAPS
    variable.clear();

    variableLocation.clear();

    branchLocation.clear();


    //DISPLAY CONTENTS
    display();

}


void MainWindow::on_actionHelp_triggered()
{

        helpWindow* helpW = new helpWindow(this);

        helpW->show();

        /*helpW->setText("This is a C++ application by Pouya Behzadi Far and Fatemeh Shirvani, which compiles assembly codes and runs a simulation of Mano’s Computer as detailed in:
                       Computer System Architecture, 3rd edition by M. Morris Mano.
                       This application can simulate all functions of the "Mano Machine" including input-output functions, register reference, and memory reference instructions.
                       the main purpose of this application is to assemble programs written in assembly language for the Mano Basic Machine, which is described in Chapter 6 of the reference mentioned above.
                       To use this application you should write your program in the left table, you can add rows using the "+" button.
                       In the first column, you should enter the label of a variable or function or whatever is called a label in your program. these labels will be stored in memory. you can see memory changes in the right table after compile.
                       In the second column, you should write the instruction of the memory reference or register reference orders.
                       if this is a memory reference instruction you should write the reference in the third column with "Operand" title.
                       If you are using an indirect instruction, you should write the keyword "I" in the fourth column.
                       this is the standard way of coding in this application. Follow the rules to get the expected result.
                       When you've finished coding press the compile button with the "hammer" icon to start the first turn of translation. if you got the "success" message it means everything is alright with your code.
                       To see what's happening in a basic computer when you run this code, you can press the right-most button to see every changes step by step.
                       the memory will fill and change each step and you can see the binary value stored in registers at any moment in the middle picture which is the structure of the Mano basic computer. Also in the list under the coding table, you can understand what is really happening in each clock pulse as you move through your code.
                       This application is easy to learn and code and it's also good for educational goals and to understand Basic computer better.
                       Special thanks to our dedicated professor Dr. Khorsandi.");
*/

}


void MainWindow::on_actionCreators_triggered()
{
    QMessageBox box(this);

    box.setWindowTitle("Developers");

    box.setText("Designed and Developed By :\n\n    Fatemeh Shirvani - 9925703 \n    Pouya Behzadifar - 9919803");

    box.exec();
}


void MainWindow::on_save_clicked()
{

    QString filename="C:/Users/Radikal/Documents/Qt projects/memari/Data.txt";

    QFile file( filename );


    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );


        QTableWidgetItem *label = new QTableWidgetItem;

        QTableWidgetItem *radix = new QTableWidgetItem;

        QTableWidgetItem *value = new QTableWidgetItem;


        int loc = ramStart;


        for (int i = 0; i <= ramEnd - ramStart ; i++){


            label = ui->tableWidget_2->item(loc, 0);

            radix = ui->tableWidget_2->item(loc, 1);

            value = ui->tableWidget_2->item(loc, 2);


            if(label)
            {
                stream <<  label->text() << "    ";
            }
            if(radix)
            {
                stream <<  radix->text() << "    ";
            }
            if(value)
            {
                stream <<  value->text() << "    ";
            }

            stream << "\n";


            loc++;


        }

    }

}

