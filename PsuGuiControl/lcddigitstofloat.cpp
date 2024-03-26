#include "lcddigitstofloat.h"

lcdDigitsToFloat::lcdDigitsToFloat()
{
    // Does not do alot
}

qreal lcdDigitsToFloat::digitsToFloat(int tens,
                                      int ones,
                                      int tenths,
                                      int hundreths,
                                      int thousandths)
{
    int     iThou;
    qreal   fThou;

    iThou = thousandths;
    iThou += hundreths * 10;
    iThou += tenths * 100;
    iThou += ones * 1000;
    iThou += tens * 10000;

    fThou = iThou;

    return fThou;
}
