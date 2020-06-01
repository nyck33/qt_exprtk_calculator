#ifndef CALCULATOR_H
#define CALCULATOR_H

//C++ Mathematical Expression Toolkit Library
#include "exprtk.hpp"
/*
extern "C"{
#include "tinyexpr.h"
}
*/
#include <QWidget>
#include <vector>
#include <algorithm>
#include <string>
#include <QString>
#include <iostream>

namespace Ui {
class Calculator;
}

class Calculator : public QWidget{
    Q_OBJECT  //base class of all QT objects, also for event handling

public:
    //parent=0 has no parent
    explicit Calculator(QWidget *parent = 0);
    ~Calculator();

//execute slots when signal submitted
protected slots:
    //misc ops
    void enterClicked();
    void clear();
    void backspaceClicked();
    void showDisplay(QString op_string);
    virtual void unaryOperatorClicked();
    //arithmetic basic
    void digitClicked();
    void addMultOperatorClicked();
    void changeSignClicked();
    //calls imported library to calculate currOpQ
    void calcAns();
    //translate to qstring for output
    void sendAnsTolineEdit();
    //pure virtual to be implemented in derived classes
    virtual void piClicked() = 0;

//private:
protected:
    //pointer to Calculator ui
    Ui::Calculator *ui;
    //vars
    double calcVal = 0.0;
    QString sumInMemory=QString::fromStdString("");
    //main string to be sent to
    QString currOpQ = QString::fromStdString("");
    //translate to std::string
    //https://stackoverflow.com/questions/4214369/how-to-convert-qstring-to-stdstring
    //default error msg
    QString defErrorMsg = QString::fromStdString("ERROR");
    int defErrorMsgLen = defErrorMsg.size();
    //changes each time
    QString errorMsg;
    //translate back to qstring for output
    QString outputQ;
    //sign change extra brackets
    bool changeSignTrigger = false;
    //ans is on screen
    bool ansTrigger = false;
    //M+ while memVal full -> show memVal -> freezeInput
    //bool blockInput = false;


};

#endif // CALCULATOR_H






    /*
    QString operators[14]
    = {"neg", "M+", "M-","RM",
       "(", ")","backspace", "/",
       "*", "-", "+", "=", "C", "."};

    //container for current operation
    //vector<QString> current_op;
    //array of all Qstring operators



    //container for all operantors for comparison using find()
    //vector<Qstring> operators_vec;

    //triggers

    bool unaryOperatorTrigger = false;
    bool additiveTrigger = false;
    bool multiplicativeTrigger = false;

    bool buttonSinTrigger = false;
    bool buttonCosTrigger = false;
    bool buttonTanTrigger = false;
    bool buttonDecPtTrigger = false;
    bool buttonClearMemoryTrigger = false;
    bool buttonChangeSignTrigger = false;
    bool buttonLBrackTrigger = false;
    bool buttonRBrackTrigger = false;
    bool buttonSin = false;
    bool buttonCos = false;
    bool buttonTan = false;
    bool buttonMemMinus = false;
    bool buttonMemPlus = false;
    bool buttonMemRecall = false;
    bool buttonSqrt = false;
    bool buttonPow = false;
    bool buttonInverse = false;
    bool buttonEnter = false;
    bool buttonLog = false;
    bool buttonAbs = false;
    bool buttonBin = false;
    bool buttonHex = false;
    bool buttonDec = false;
    bool buttonRad = false;
    bool buttonDegree = false;
    */

    //Button *CreateButton(const QString &text, const char *member);

    //Divide by zero
    /*
    bool calculate(double rightOperand, const Qstring &pendingOperator);
    double sumInMemory;
    double sumSoFar;
    double factorStar;
    QString pendingAdditiveOperator;
    Qstring pendingMultiplicativeOperator;
    bool waitingForOperand;
    //for display
    QLineEdit *display;
    //display numerals
    enum {NumDigitButtons = 10};
    Button *digitButtons[NumDigitButtons];
    */



