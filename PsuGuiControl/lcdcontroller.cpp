#include "lcdcontroller.h"

lcdController::lcdController()
{
    pTens = nullptr;
    pOnes = nullptr;
    pTenths = nullptr;
    pHundreths = nullptr;
    pThousandths = nullptr;
}

void lcdController::setDigits(QLCDNumber *tens,
                              QLCDNumber *ones,
                              QLCDNumber *tenths,
                              QLCDNumber *hundreths,
                              QLCDNumber *thousandths)
{
    pTens = tens;
    pOnes = ones;
    pTenths = tenths;
    pHundreths = hundreths;
    pThousandths = thousandths;

    iTens = 0;
    iOnes = 0;
    iTenths = 0;
    iHundreths = 0;
    iThousandths = 0;

    minTens = 0;
    minOnes = 0;
    minTenths = 0;
    minHundreths = 0;
    minThousandths = 0;

    maxTens = 3;
    maxOnes = 9;
    maxTenths = 9;
    maxHundreths = 9;
    maxThousandths = 9;
}

void lcdController::setValues(int tens,
                              int ones,
                              int tenths,
                              int hundreths,
                              int thousandths)
{
    iTens = tens;
    iOnes = ones;
    iTenths = tenths;
    iHundreths = hundreths;
    iThousandths = thousandths;

    pTens->display(iTens);
    pOnes->display(iOnes);
    pTenths->display(iTenths);
    pHundreths->display(iHundreths);
    pThousandths->display(iThousandths);
}

void lcdController::upTens()
{
    if (iTens < maxTens)
    {
        iTens++;
        pTens->display(iTens);
    }
}

void lcdController::downTens()
{
    if (iTens > minTens)
    {
        iTens--;
        pTens->display(iTens);
    }
}

void lcdController::upOnes()
{
    if (iOnes < maxOnes)
    {
        iOnes++;
        pOnes->display(iOnes);
    }
}

void lcdController::downOnes()
{
    if (iOnes > minOnes)
    {
        iOnes--;
        pOnes->display(iOnes);
    }
}

void lcdController::upTenths()
{
    if (iTenths < maxTenths)
    {
        iTenths++;
        pTenths->display(iTenths);
    }
}

void lcdController::downTenths()
{
    if (iTenths > minTenths)
    {
        iTenths--;
        pTenths->display(iTenths);
    }
}

void lcdController::upHundreths()
{
    if (iHundreths < maxHundreths)
    {
        iHundreths++;
        pHundreths->display(iHundreths);
    }
}

void lcdController::downHundreths()
{
    if (iHundreths > minHundreths)
    {
        iHundreths--;
        pHundreths->display(iHundreths);
    }
}

void lcdController::upThousandths()
{
    if (iThousandths < maxThousandths)
    {
        iThousandths++;
        pThousandths->display(iThousandths);
    }
}

void lcdController::downThousandths()
{
    if (iThousandths > minThousandths)
    {
        iThousandths--;
        pThousandths->display(iThousandths);
    }
}
