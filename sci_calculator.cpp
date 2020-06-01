//C++ Mathematical Expression Toolkit Library
#include "exprtk.hpp"
//extern "C"{
//#include "tinyexpr.h"
//}

#include "calculator.h"
#include "sci_calculator.h"
#include "ui_calculator.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <QString>
#include <QTextStream>
#include <math.h>


//call parent constructor and wire new buttons
Sci_calculator::Sci_calculator(QWidget *parent):
    Calculator(parent){

    //Ui::Calculator *ui = getUiPtr();

    connect(ui->ButtonSin, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonCos, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonTan, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonAsin, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonAcos, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonAtan, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonAtan2, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonD2R, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonR2D, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonExp, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonPow, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonAbs, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonLog, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonLogn,SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonModulo, SIGNAL(released()), this, SLOT(addMultOperatorClicked()));
    connect(ui->ButtonPi, SIGNAL(released()), this, SLOT(piClicked()));
    connect(ui->ButtonAvg, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonMul, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonComma, SIGNAL(released()), this, SLOT(addMultOperatorClicked()));
}

Sci_calculator::~Sci_calculator(){}

void Sci_calculator::piClicked(){
    //Sender returns a pointer to the button pressed
    QPushButton *button = (QPushButton *)sender();
    //get operand symbol of the button
    QString buttonVal = button->text();
    if(ansTrigger==true){
        showDisplay(outputQ);
    }

    //on main screen
    else{
        //add buttonVal
        currOpQ.append(pi_str);
        //lineEdit
        showDisplay(currOpQ);
    }
}


