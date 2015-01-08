#include "moneyvalidator.h"

QValidator::State MoneyValidator::validate(QString& input, int& pos) const
{
//    std::cout << "validate: " << Convert(input) << " pos: " << pos ;
    const QValidator::State state = QDoubleValidator::validate(input, pos);

    if (state == QValidator::Intermediate || state == QValidator::Acceptable)
    { // do additional validation
        static QChar decimalPoint = locale().decimalPoint();
        static QChar altDecimalPoint = decimalPoint == '.' ? ',' : '.';
        input = input.replace(altDecimalPoint, decimalPoint);
    }

//    std::cout << " result: " << Convert(input) << " pos: " << pos << " state: " << state << std::endl;
    return state;
}

