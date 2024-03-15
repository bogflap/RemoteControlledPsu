#include <QString>
#include <QTextStream>

#include "lcdfloattodigits.h"

lcdFloatToDigits::lcdFloatToDigits()
{
    // Not alot to do
}

// *****************************************************************************
//
// This function basically decodes a qreal value into values that QLCDNumber can
// be initialised with
//
// *****************************************************************************
void lcdFloatToDigits::floatToDigits(qreal value,
                                     int &tens,
                                     int &ones,
                                     int &tenths,
                                     int &hundreths,
                                     int &thousandths)
{
    QString     fValue;
    QByteArray  ba;
    qsizetype   st;

    QTextStream ts(&fValue);

    ts.setFieldWidth(6);
    ts.setPadChar('0');
    ts.setRealNumberPrecision(3);
    ts.setRealNumberNotation(QTextStream::FixedNotation);
    ts.setFieldAlignment(QTextStream::AlignCenter);

    ts << value;

    ba = fValue.toLocal8Bit();

    st = ba.indexOf(".");

    if (st == 1)
    {
        tens = 0;
        ones = ba[0] - 0x30;
        tenths = ba[2] - 0x30;
        hundreths = ba[3] - 0x30;
        thousandths = ba[4] - 0x30;
    }
    else
    {
        tens = ba[0] - 0x30;
        ones = ba[1] - 0x30;
        tenths = ba[3] - 0x30;
        hundreths = ba[4] - 0x30;
        thousandths = ba[5] - 0x30;
    }
}

lcdFloatToDigits::~lcdFloatToDigits()
{
    // Not alot to do
}
