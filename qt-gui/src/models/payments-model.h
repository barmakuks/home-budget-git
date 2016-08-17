#pragma once
#include <QAbstractTableModel>

#include "payment-document.h"
#include "payment-type.h"

class PaymentsModel : public QAbstractTableModel
{
public:
    PaymentsModel();

    void Reload(const QDate& minDate, const QDate& maxDate);

    void Reload(const std::string& minDate, const std::string& maxDate);

public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

private:
    const hb::PaymentDocument& GetPaymentItemRef(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:
    hb::PaymentsPtr m_documents;
    hb::PaymentTypesMapPtr m_docTypes;
};
