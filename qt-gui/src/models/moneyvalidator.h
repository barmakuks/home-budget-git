#ifndef MONEYVALIDATOR_H
#define MONEYVALIDATOR_H

#include <QDoubleValidator>

class MoneyValidator: public QDoubleValidator{
    // QValidator interface
public:
    virtual State validate(QString& input, int& pos) const;
};

#endif // MONEYVALIDATOR_H
