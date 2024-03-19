#include "lcdcontroller.h"

lcdController::lcdController()
{
    pTens = nullptr;
    pOnes = nullptr;
    pTenths = nullptr;
    pHundreths = nullptr;
    pThousandths = nullptr;
}

void lcdController::setDigits(QLCDNumber    *tens,
                              QLCDNumber    *ones,
                              QLCDNumber    *tenths,
                              QLCDNumber    *hundreths,
                              QLCDNumber    *thousandths,
                              qreal         maxValue)
{
    pTens = tens;
    pOnes = ones;
    pTenths = tenths;
    pHundreths = hundreths;
    pThousandths = thousandths;

    iMaxValue = qFloor(maxValue);
    fToD.floatToDigits(maxValue, iTens, iOnes, iTenths, iHundreths, iThousandths);

    if (!checkDigits(iTens, iOnes, iTenths, iHundreths, iThousandths))
    {
        iTens = 0;
        iOnes = 0;
        iTenths = 0;
        iHundreths = 0;
        iThousandths = 0;
    }
}

void lcdController::setValues(int tens,
                              int ones,
                              int tenths,
                              int hundreths,
                              int thousandths)
{
    if (checkDigits(tens, ones, tenths, hundreths, thousandths))
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
}

void lcdController::upTens()
{
    if (checkDigits(iTens + 1, iOnes, iTenths, iHundreths, iThousandths))
    {
        iTens++;
        pTens->display(iTens);
    }
}

void lcdController::downTens()
{
    if (checkDigits(iTens - 1, iOnes, iTenths, iHundreths, iThousandths))
    {
        iTens--;
        pTens->display(iTens);
    }
}

void lcdController::upOnes()
{
    if (checkDigits(iTens, iOnes + 1, iTenths, iHundreths, iThousandths))
    {
        iOnes++;
        pOnes->display(iOnes);
    }
}

void lcdController::downOnes()
{
    if (checkDigits(iTens, iOnes - 1, iTenths, iHundreths, iThousandths))
    {
        iOnes--;
        pOnes->display(iOnes);
    }
}

void lcdController::upTenths()
{
    if (checkDigits(iTens, iOnes, iTenths + 1, iHundreths, iThousandths))
    {
        iTenths++;
        pTenths->display(iTenths);
    }
}

void lcdController::downTenths()
{
    if (checkDigits(iTens, iOnes, iTenths - 1, iHundreths, iThousandths))
    {
        iTenths--;
        pTenths->display(iTenths);
    }
}

void lcdController::upHundreths()
{
    if (checkDigits(iTens, iOnes, iTenths, iHundreths + 1, iThousandths))
    {
        iHundreths++;
        pHundreths->display(iHundreths);
    }
}

void lcdController::downHundreths()
{
    if (checkDigits(iTens, iOnes, iTenths, iHundreths - 1, iThousandths))
    {
        iHundreths--;
        pHundreths->display(iHundreths);
    }
}

void lcdController::upThousandths()
{
    if (checkDigits(iTens, iOnes, iTenths, iHundreths, iThousandths + 1))
    {
        iThousandths++;
        pThousandths->display(iThousandths);
    }
}

void lcdController::downThousandths()
{
    if (checkDigits(iTens, iOnes, iTenths, iHundreths, iThousandths - 1))
    {
        iThousandths--;
        pThousandths->display(iThousandths);
    }
}

bool lcdController::checkDigits(int tens, int ones, int tenths, int hundreths, int thousandths)
{
    bool    result = false;

    if ((tens >= 0) && (ones >= 0) && (tenths >= 0) && (hundreths >= 0) && (thousandths >= 0))
    {
        if ((tens < 10) && (ones < 10) && (tenths < 10) && (hundreths < 10) && (thousandths < 10))
        {
            int     newValue;

            newValue = (tens * 10000) + (ones * 1000) + (tenths * 100) + (hundreths * 10) + (thousandths * 1);
            if (newValue <= iMaxValue)
            {
                result = true;
            }
        }
    }

    // Else do nothing

    return result;
}
