#ifndef LCDDIGITSTOFLOAT_H
#define LCDDIGITSTOFLOAT_H

#include <QtGlobal>

class lcdDigitsToFloat
{
public:
    lcdDigitsToFloat();

    qreal   digitsToFloat(int tens, int ones, int tenths, int hundreths, int thousandths);
};

#endif // LCDDIGITSTOFLOAT_H
