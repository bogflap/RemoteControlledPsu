#ifndef LCDFLOATTODIGITS_H
#define LCDFLOATTODIGITS_H

class lcdFloatToDigits
{
public:
    explicit lcdFloatToDigits();

    void    floatToDigits(  qreal value,
                            int& tens,
                            int& ones,
                            int& tenths,
                            int& hundreths,
                            int& thousandths);

    ~lcdFloatToDigits();
};

#endif // LCDFLOATTODIGITS_H
