//C++ Mathematical Expression Toolkit Library
#include "exprtk.hpp"
//extern "C"{
//#include "tinyexpr.h"
//}
#include "calculator.h"
#include "sci_calculator.h"
#include <QApplication>

#include <vector>
#include <algorithm>
#include <QString>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sci_calculator w;
    w.show();

    return a.exec();
}
