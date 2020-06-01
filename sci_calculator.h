#ifndef SCI_CALCULATOR_H
#define SCI_CALCULATOR_H

#include "calculator.h"
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
#include <math.h>  //for pi

class Sci_calculator : public Calculator{
    Q_OBJECT

public:
    explicit Sci_calculator(QWidget *parent=0);
    ~Sci_calculator();

protected slots:
    void piClicked();

protected:

    double pi = M_PI;
    //double pi = 3.14159265359;
    QString pi_str = QString::number(pi);


};
#endif // SCI_CALCULATOR_H
