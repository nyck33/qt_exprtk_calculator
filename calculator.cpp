//C++ Mathematical Expression Toolkit Library
#include "exprtk.hpp"
//extern "C"{
//#include "tinyexpr.h"
//}

#include "calculator.h"
#include "ui_calculator.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <QString>
#include <QTextStream>

//for debug
QTextStream out(stdout);

//pointer to Calculator ui
//Ui::Calculator *ui;

//Create the UI class and assign it to header
Calculator::Calculator(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Calculator){
        //setup the UI
        ui->setupUi(this);
        //put 0.0 in lineEdit
        ui->lineEdit->setText(QString::number(calcVal));
        //will hold references to all number buttons
        int numDigits = 10;
        QPushButton *digitButtons[numDigits];
        //iterate and locate buttons
        for(int i = 0; i<numDigits; ++i){
            QString buttonName = "Button" + QString::number(i);
            //get the buttons by name and add to array
            digitButtons[i] = Calculator::findChild<QPushButton *>(buttonName);
            //digitButtons[i] = new QPushButton(buttonName, this);
            //when button is released cal num_pressed()
            connect(digitButtons[i], SIGNAL(released()), this, SLOT(digitClicked()));
        }

    //Connect signals and slots for buttons
    //arithmetic
    connect(ui->ButtonAdd, SIGNAL(released()), this, SLOT(addMultOperatorClicked()));
    connect(ui->ButtonSubtr, SIGNAL(released()), this, SLOT(addMultOperatorClicked()));
    connect(ui->ButtonMult, SIGNAL(released()), this, SLOT(addMultOperatorClicked()));
    connect(ui->ButtonDiv, SIGNAL(released()), this, SLOT(addMultOperatorClicked()));
    connect(ui->ButtonDecPt, SIGNAL(released()), this, SLOT(addMultOperatorClicked()));
    //bedmas
    connect(ui->ButtonLBrack, SIGNAL(released()), this, SLOT(addMultOperatorClicked()));
    connect(ui->ButtonRBrack, SIGNAL(released()), this, SLOT(addMultOperatorClicked()));
    //special
    connect(ui->ButtonSignChange, SIGNAL(released()), this, SLOT(changeSignClicked()));
    connect(ui->ButtonClear, SIGNAL(released()), this, SLOT(clear()));
    connect(ui->ButtonBack, SIGNAL(released()), this, SLOT(backspaceClicked()));
    connect(ui->ButtonEnter, SIGNAL(released()), this, SLOT(enterClicked()));
    //math
    connect(ui->ButtonSqrt, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    connect(ui->ButtonSquare, SIGNAL(released()), this, SLOT(unaryOperatorClicked()));
    }

    Calculator::~Calculator(){
        delete ui;
    }

    //enter or = sign, after M+, when it shouldn't be pressed or after complete op?
    //RM sets trigger = false so no problem
    void Calculator::enterClicked(){
        //if ans is on screen stay there until cleared
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //m+ -> 2 -> enter
        else{
            //out << "93" << endl;
            ansTrigger = true;
            //do the calculation
            calcAns();
            //lineEdit it
            sendAnsTolineEdit();
        }
    }
    //clear lineEdit but memory sticks
    void Calculator::clear(){
        currOpQ = QString::fromStdString("");
        outputQ= QString::fromStdString("");
        errorMsg = QString::fromStdString("");
        calcVal = 0.0;
        changeSignTrigger = false;
        ansTrigger = false;
        //shows blank
        showDisplay(currOpQ);
    }

    //deletes entries one at a time
    void Calculator::backspaceClicked(){
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        else{
            int lenCurrOpQ = currOpQ.size();
            //if change sign deleted, need to reverse flag, so flag positions of "-" sign
            //QStringRef temp(&currOpQ, 0, lenCurrOpQ-1);
            QString temp = currOpQ.left(lenCurrOpQ-1);
            currOpQ = temp;
            showDisplay(currOpQ);
        }
    }

    //display screen
    void Calculator::showDisplay(QString op_string){
        ui->lineEdit->setText(QString::fromStdString(""));
        ui->lineEdit->setText(op_string);
    }

    void Calculator::digitClicked(){
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
            currOpQ.append(buttonVal);
            //lineEdit
            showDisplay(currOpQ);
        }
    }
    //sqrt and ^, ex. //if changeSign, (-log(25))
    void Calculator::unaryOperatorClicked(){
        //Sender returns a pointer to the button pressed
        QPushButton *button = (QPushButton *)sender();
        //get operand symbol of the button
        QString buttonVal = button->text();
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //is sqrt(4), let user manually enter brackets
        else{
            currOpQ.append(buttonVal);
            showDisplay(currOpQ);
        }
    }

    //add to qstring
    void Calculator::addMultOperatorClicked(){
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
            currOpQ.append(buttonVal);
            //lineEdit
            showDisplay(currOpQ);
        }
    }

    //use trigger and check after next operator entered
    //if trigger set, enter closing parentheses
    void Calculator::changeSignClicked(){
        //Sender returns a pointer to the button pressed
        QPushButton *button = (QPushButton *)sender();
        //get operand symbol of the button
        QString buttonVal = button->text();
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //if pushed once already, turn off on
        else if(!ansTrigger && changeSignTrigger){
            changeSignTrigger = false;
            //delete 1 char from currOpQ
            backspaceClicked();
        }
        //on main screen and pos
        else if(!changeSignTrigger){
            //add to qstring
            currOpQ.append(QString::fromStdString("-"));
            //lineEdit
            changeSignTrigger = true;
            showDisplay(currOpQ);

        }
    }
    //http://www.partow.net/programming/exprtk/index.html
    void Calculator::calcAns(){
        //convert to std::String
        //currOpStd = currOpQ.toStdString();
        const std::string expression_string = currOpQ.toStdString();
        //numeric type
        typedef double T;
        typedef exprtk::expression<T> expression_t;
        typedef exprtk::parser<T> parser_t;

        //instantiate classes
        expression_t expression;
        parser_t parser;

        if(!parser.compile(expression_string, expression)){
            //construct errorMsg
            errorMsg = defErrorMsg;
            errorMsg.append(QString::fromStdString("Calc"));
        }
        else{
            T result = expression.value();
            //out << "425: " << double(result)<<endl;
            calcVal = (double)result;
        }

    }

    //check error msg, output it or
    void Calculator::sendAnsTolineEdit(){
        if(errorMsg.size()>defErrorMsgLen){
            outputQ = errorMsg;
        }
        else{
            //nothing wrong so output ans
            ansTrigger = true;
            //reset everything
            errorMsg = defErrorMsg;
            currOpQ = QString::fromStdString("");
            //output
            outputQ = QString::number(calcVal);

        }
        showDisplay(outputQ);
    }
