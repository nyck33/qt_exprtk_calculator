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
    connect(ui->ButtonAdd, SIGNAL(released()), this, SLOT(additiveOperatorClicked()));
    connect(ui->ButtonSubtr, SIGNAL(released()), this, SLOT(additiveOperatorClicked()));
    connect(ui->ButtonMult, SIGNAL(released()), this, SLOT(multiplicativeOperatorClicked()));
    connect(ui->ButtonDiv, SIGNAL(released()), this, SLOT(multiplicativeOperatorClicked()));
    //misc
    connect(ui->ButtonDecPt, SIGNAL(released()), this, SLOT(decPtClicked()));
    connect(ui->ButtonClear, SIGNAL(released()), this, SLOT(clear()));
    connect(ui->ButtonChangeSign, SIGNAL(released()), this, SLOT(changeSignClicked()));
    connect(ui->ButtonBack, SIGNAL(released()), this, SLOT(backspaceClicked()));
    connect(ui->ButtonEnter, SIGNAL(released()), this, SLOT(enterClicked()));
    //bedmas
    connect(ui->ButtonLBrack, SIGNAL(released()), this, SLOT(leftBrackClicked()));
    connect(ui->ButtonRBrack, SIGNAL(released()), this, SLOT(rightBrackClicked()));
    //memory
    connect(ui->ButtonMemMinus, SIGNAL(released()), this, SLOT(clearMemory()));
    connect(ui->ButtonMemPlus, SIGNAL(released()), this, SLOT(setMemory()));
    connect(ui->ButtonMemRecall, SIGNAL(released()), this, SLOT(readMemory()));
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
        //on mem screen, enter must be pressed after memVal entered
        else if(onMemScreen){
            //leaves memory screen
            onMemScreen = false;
            //if changed Sign to neg in memory
            if(changeSignTrigger==true){
                sumInMemory.append(QString::fromStdString(")"));
                changeSignTrigger=false;
                showDisplay(currOpQ);
            }
            //no sign change for sum in memory
            else{
                showDisplay(currOpQ);
            }
        }
        // on currOp screen
        else{
            //do the calculation
            calcAns();
            //lineEdit it
            sendAnsTolineEdit();
        }

    }
    //clear lineEdit but memory sticks
    void Calculator::clear(){
        currOpQ = QString::fromStdString("");
        currOpStd = "";
        outputQ= QString::fromStdString("");
        calcVal = 0.0;
        changeSignTrigger = false;
        ansTrigger = false;
        onMemScreen = false;
        rmTrigger = false;
        //shows blank
        showDisplay(currOpQ);
    }

    //deletes entries one at a time
    void Calculator::backspaceClicked(){
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        else{
            if(onMemScreen==true){
                if(sumInMemory.size() > 0){
                    int lenSumInMemory = sumInMemory.size();
                    //QStringRef temp(&currOpQ, 0, lenCurrOpQ-1);
                    QString temp = sumInMemory.left(lenSumInMemory-1);
                    sumInMemory = temp;
                    showDisplay(sumInMemory);
                }
            }
            else{
                int lenCurrOpQ = currOpQ.size();
                //QStringRef temp(&currOpQ, 0, lenCurrOpQ-1);
                QString temp = currOpQ.left(lenCurrOpQ-1);
                currOpQ = temp;
                showDisplay(currOpQ);
            }
        }

    }
    //display screen
    void Calculator::showDisplay(QString op_string){
        ui->lineEdit->setText(QString::fromStdString(""));
        ui->lineEdit->setText(op_string);
    }
    //M+ pushed so value goes to sumInMemory
    void Calculator::setMemory(){
        onMemScreen = true;
        //save a calculation answer, overwrite memory
        if(ansTrigger){
            clearMemory();
            sumInMemory = outputQ;
            clear();
        }
        //if val already in memory but not the answer
        else if(!ansTrigger && memoryTrigger){
            //same behavior as RM
            readMemory();
        }
        //no val in memory
        else{
            memoryTrigger = true;
            //show blank
            showDisplay(sumInMemory);
        }
    }
    //clears memory regardless of state
    void Calculator::clearMemory(){
        onMemScreen = false;
        //clear Memory
        sumInMemory = QString::fromStdString("");
        memoryTrigger = false;
        //if ans on screen
        if(ansTrigger){
            showDisplay(outputQ);
        }
        else{
            showDisplay(currOpQ);
        }
    }

    //lineEdit and return value in sumInMemory
    void Calculator::readMemory(){
        if(ansTrigger==true){
            clear();
            showDisplay(sumInMemory);
        }
        //show the memVal
        else{
            onMemScreen = true;
            showDisplay(sumInMemory);
        }
        //set trigger for adding current sumInMemory to currOpQ in
        //binary operator functions, for incorrect ops,
        //ex. 2+2->RM, let the math machine handle it or leave it onscreen
        //for user to correct
        rmTrigger = true;
    }

    //if on ans screen or RM was pushed prior, then digit is not inputted
    void Calculator::digitClicked(){
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //if RM was pushed, user must push clear to get back
        //else they just see the memVal
        else if(rmTrigger){
            showDisplay(sumInMemory);
        }
        else{
            //Sender returns a ptr to the button pressed
            QPushButton *button = (QPushButton *)sender();
            //Get digit clicked
            QString buttonVal = button->text();
            //append to currOpQ if memoryTrigger not set
            //if set to sumInMemory
            if(onMemScreen==true){
                sumInMemory.append(buttonVal);
                showDisplay(sumInMemory);
            }
            else{
                currOpQ.append(buttonVal);
                showDisplay(currOpQ);
            }
        }
    }
    void Calculator::decPtClicked(){
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //if RM was pushed, user must push clear to get back
        //else they just see the memVal
        else if(rmTrigger){
            showDisplay(sumInMemory);
        }
        else{
            //Sender returns a pointer to the button pressed
            QPushButton *button = (QPushButton *)sender();
            //get operand symbol of the button
            QString buttonVal = button->text();
            if(onMemScreen==true){
                sumInMemory.append(buttonVal);
                showDisplay(sumInMemory);
            }
            else{
                currOpQ.append(buttonVal);
                showDisplay(currOpQ);
            }
        }
    }

    //add to qstring
    void Calculator::additiveOperatorClicked(){
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //RM->currOp needs clear(), no operators in memory
        else if(onMemScreen==true){
            showDisplay(sumInMemory);
        }
        //on main screen
        else{
            //Sender returns a pointer to the button pressed
            QPushButton *button = (QPushButton *)sender();
            //get operand symbol of the button
            QString buttonVal = button->text();
            //if changeSign
            if(changeSignTrigger==true){
                currOpQ.append(QString::fromStdString(")"));
                changeSignTrigger=false;
            }
            //2+RM+2 situation,append memVal to currOp
            if(rmTrigger){
                currOpQ.append(sumInMemory);
                rmTrigger = false;
            }
            //add to qstring
            currOpQ.append(buttonVal);

            //lineEdit
            showDisplay(currOpQ);
        }
    }

    void Calculator::multiplicativeOperatorClicked(){
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //RM->currOp needs clear(), no operators in memory
        else if(onMemScreen==true){
            showDisplay(sumInMemory);
        }
        //in currOpQ
        else{
            //Sender returns a pointer to the button pressed
            QPushButton *button = (QPushButton *)sender();
            //get operand symbol of the button
            QString buttonVal = button->text();
            //if changeSign
            if(changeSignTrigger==true){
                currOpQ.append(QString::fromStdString(")"));
                changeSignTrigger=false;
            }
            //2+RM+2 situation,append memVal to currOp
            if(rmTrigger){
                currOpQ.append(sumInMemory);
                rmTrigger = false;
            }
            currOpQ.append(buttonVal);
            //lineEdit
            showDisplay(currOpQ);
        }

    }
    //use trigger and check after next operator entered
    //if trigger set, enter closing parentheses
    void Calculator::changeSignClicked(){
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //if pushed once already, turn off
        else if(changeSignTrigger){
            changeSignTrigger = false;
            //delete 2 chars from currOpQ
            backspaceClicked();
            backspaceClicked();
        }
        //if RM was pushed, user must push clear to get back
        //else they just see the memVal
        else if(rmTrigger){
            showDisplay(sumInMemory);
        }
        //currently false
        else{
            changeSignTrigger = true;
            currOpQ.append(QString::fromStdString("(-"));
            showDisplay(currOpQ);
        }
    }
    void Calculator::leftBrackClicked(){
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //no brackets except for negs in memory
        else if(onMemScreen){
            showDisplay(sumInMemory);
        }
        //if RM was pushed, user must push clear to get back
        //else they just see the memVal
        else if(rmTrigger){
            showDisplay(sumInMemory);
        }
        //in currOpQ
        else{
            //Sender returns a pointer to the button pressed
            QPushButton *button = (QPushButton *)sender();
            //get operand symbol of the button
            QString buttonVal = button->text();
            currOpQ.append(buttonVal);
            //currOpQ.append(QString::fromStdString("("));
            //lineEdit
            showDisplay(currOpQ);
        }
    }
    //check if changeSignTrigger set, then double bracket close
    void Calculator::rightBrackClicked(){
        if(ansTrigger==true){
            showDisplay(outputQ);
        }
        //if RM was pushed, user must push clear to get back
        //else they just see the memVal
        else if(rmTrigger){
            showDisplay(sumInMemory);
        }
        else{
            //Sender returns a pointer to the button pressed
            QPushButton *button = (QPushButton *)sender();
            //get operand symbol of the button
            QString buttonVal = button->text();
            //currOpQ.append(QString::fromStdString(")"));
            currOpQ.append(buttonVal);
            //extra bracket for negative number
            if(changeSignTrigger==true){
                //currOpQ.append(QString::fromStdString(")"));
                currOpQ.append(buttonVal);
                changeSignTrigger=false;
            }
            //lineEdit
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

        expression_t expression;
        parser_t parser;

        if(!parser.compile(expression_string, expression)){
            //construct errorMsg
            errorMsg = defErrorMsg;
            errorMsg.append(QString::
                            fromStdString("Calc"));
        }
        else{
            T result = expression.value();
            calcVal = (double)result;
        }

    }

    //check error msg, output it or
    void Calculator::sendAnsTolineEdit(){
        if(errorMsg.size()>defErrorMsgLen){
            outputQ = errorMsg;
        }
        else{
            //nothing wrong so output
            //ans trigger
            ansTrigger = true;
            //reset everything
            errorMsg = defErrorMsg;
            calcVal = 0.0;
            currOpQ = QString::fromStdString("");
            //unused
            currOpStd = "";
            onMemScreen = false;
            rmTrigger = false;
            //output
            outputQ = QString::number(calcVal);

        }
        showDisplay(outputQ);
    }

    /*
    void Calculator::calcAns(){
        int error = 0;
        currOpStd = currOpQ.toStdString();
        //call outside function
        const char *expression = currOpStd.c_str();
        calcVal = te_interp(expression, &error);
        //check error
        if(error!=0){
            errorMsg = defErrorMsg;
            errorMsg.append(QString::
                            fromStdString(std::to_string(error)));
            //change NaN to string
            currOpStd = std::to_string(calcVal);
         }

    }
    */

    /*
    //////////////////////////////////////////////////////////////////////////
    //memory
    Button *ButtonMemMinus, SIGNAL(released(), this, S)
    Button *ButtonDecPt = createbutton(tr("."), SLOT(pointClicked()));
    //tr function translates strings so 302/261's UTF code is +-
    Button *ButtonChangeSign = createButton(tr("\302\261"), SLOT(changeSignClicked()));
    Button *ButtonBack = createButton(tr("Backspace"), SLOT(backspaceClicked()));
    Button *ButtonClear = createButton(tr("Clear All"), SLOT(clearAll()));
    //Bedmas
    Button *ButtonLBrack =createButton(tr("\050"));
    Button *ButtonRBrack =createButton(tr("\051"));
    //trig
    Button *ButtonSin = createButton(tr("sin"), SLOT(Button(sinClicked())));
    Button *ButtonCos = createButton(tr("cos"), SLOT(Button(cosClicked())));
    Button *ButtonTan = createButton(tr("tan"), SLOT(Button(tanClicked())));

    //memory
    Button *ButtonMemMinus = createButton(tr("M-"), SLOT(Button(clearMemory())));
    Button *ButtonMemPlus = createButton(tr("M+"), SLOT(Button(setMemory())));
    Button *ButtonMemRecall = createButton(tr("RM"), SLOT(Button(readMemory())));
    //arithmetic
    Button *ButtonDiv = CreateButton(tr("/"), SLOT(Button(multiplicativeOperatorClicked())));
    Button *ButtonMult = CreateButton(tr("*"), SLOT(Button(multiplicativeOperatorClicked()));
    Button *ButtonAdd = CreateButton(tr("+"), SLOT(Button(additiveOperatorClicked())));
    Button *ButtonSubtr = CreateButton(tr("-"), SLOT(Button(additiveOperatorClicked())));
    //unary
    Button *ButtonSqrt = CreateButton(tr("sqrt"), SLOT(Button(unaryOperatorClicked())));
    Button *ButtonPow = CreateButton(tr("pow"), SLOT(Button(unaryOperatorClicked())));
    Button *ButtonInverse = CreateButton(tr("inv"), SLOT(Button(unaryOperatorClicked())));
    Button *ButtonEnter = CreateButton(tr("="), SLOT(Button(unaryOperatorClicked())));
    Button *ButtonLog = CreateButton(tr("log"), SLOT(Button(unaryOperatorClicked())));
    Button *ButtonAbs = CreateButton(tr("abs"), SLOT(Button(unaryOperatorClicked())));
    //base
    Button *ButtonBin = CreateButton(tr("bin"), SLOT(Button(binaryClicked())));
    Button *ButtonHex = CreateButton(tr("hex"), SLOT(Button(hexClicked())));
    Button *ButtonDec = CreateButton(tr("dec"), SLOT(Button(decimalClicked())));
    //unit circle
    Button *ButtonRad = CreateButton(tr("rad"), SLOT(Button(radianClicked())));
    Button *ButtonDeg = CreateButton(tr("deg"), SLOT(Button(degreesClicked())));
    //clear
    Button *clearMemoryButton = CreateButton(tr("CE"), SLOT(Button(clear())));
    */




