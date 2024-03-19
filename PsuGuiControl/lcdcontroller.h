#ifndef LCDCONTROLLER_H
#define LCDCONTROLLER_H

#include <QLCDNumber>

#include "lcdfloattodigits.h"

class lcdController
{
public:
            lcdController();

    void    setDigits(QLCDNumber*   tens,
                        QLCDNumber* ones,
                        QLCDNumber* tenths,
                        QLCDNumber* hundreths,
                        QLCDNumber* thousandths,
                        qreal       maxValue);
    void    setValues(  int tens,
                        int ones,
                        int tenths,
                        int hundreths,
                        int thousandths);
    void    upTens();
    void    downTens();
    void    upOnes();
    void    downOnes();
    void    upTenths();
    void    downTenths();
    void    upHundreths();
    void    downHundreths();
    void    upThousandths();
    void    downThousandths();

private:
    bool    checkDigits(int tens, int ones, int tenths, int hundreths, int thousandths);

private:
    QLCDNumber* pTens;
    QLCDNumber* pOnes;
    QLCDNumber* pTenths;
    QLCDNumber* pHundreths;
    QLCDNumber* pThousandths;

    int         iTens;
    int         iOnes;
    int         iTenths;
    int         iHundreths;
    int         iThousandths;
    int         iMaxValue;

    lcdFloatToDigits    fToD;
};

#endif // LCDCONTROLLER_H
