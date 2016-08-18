#pragma once
#include <QDoubleValidator>

class MoneyValidator : public QDoubleValidator
{
    // QValidator interface
public:
    virtual State validate(QString& input, int& pos) const;
};
