#include "payments-model.h"
#include <QColor>

#include "../convert-utils.h"
#include "string-format.h"
#include "engine.h"

PaymentsModel::PaymentsModel()
{
}

void PaymentsModel::Reload(const QDate& minDate, const QDate& maxDate)
{
    Reload(hb::utils::NormalizeDate(minDate),
           hb::utils::NormalizeDate(maxDate));
}

void PaymentsModel::Reload(const std::string& minDate, const std::string& maxDate)
{
    using namespace hb::core;

    m_documents = Engine::GetInstance().GetPayments(minDate, maxDate);
    m_docTypes = Engine::GetInstance().GetPaymentTypes();

    reset();
}

int PaymentsModel::rowCount(const QModelIndex& parent) const
{
    return m_documents ? m_documents->size() : 0;
}

struct Columns
{
    enum {Date, PaymentName, Amount, Counter_1, Counter_2, Note};
};

int PaymentsModel::columnCount(const QModelIndex& parent) const
{
    return 6;
}

QVariant PaymentsModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole :
    {
        return GetCellAlignment(index);
    }

    case Qt::DisplayRole:
    {
        return GetCellString(index);
    }

    case Qt::TextColorRole:
    {
        return GetCellForecolor(index);
    }

    case Qt::BackgroundColorRole:
    {
        return GetCellBackColor(index);
    }

    default:
    {
        return QVariant();
    }
    }
}

const hb::core::PaymentDocument& PaymentsModel::GetPaymentItemRef(int index) const
{
    return *(m_documents->at(index));
}

QVariant PaymentsModel::GetCellString(const QModelIndex& index) const
{
    using namespace hb::core;
    using namespace hb::utils;

    const PaymentDocument& doc = GetPaymentItemRef(index.row());

    switch (index.column())
    {
    case Columns::Amount:
    {
        return Tr(hb::utils::FormatMoney(doc.Amount()));
    }
    case Columns::Counter_1:
    {
        return Tr(doc.Counter1());
    }
    case Columns::Counter_2:
    {
        return Tr(doc.Counter2());
    }
    case Columns::Date:
    {
        return Tr(hb::utils::FormatDate(doc.DocDate()));
    }
    case Columns::PaymentName:
    {
        return Tr(m_docTypes->at(doc.PaymentType())->Name());
    }
    case Columns::Note:
    {
        return Tr(doc.Note());
    }
    default:
    {
        return QVariant("no data");
    }
    }
}

QVariant PaymentsModel::GetCellAlignment(const QModelIndex& index) const
{
    switch (index.column())
    case Columns::Amount:
    case Columns::Counter_1:
    case Columns::Counter_2:
    {
        return Qt::AlignVCenter + Qt::AlignRight;
    }
    return QVariant();
}

QVariant PaymentsModel::GetCellForecolor(const QModelIndex& index) const
{
    if (index.column() == Columns::Amount)
    {
        const hb::core::PaymentDocument& doc = GetPaymentItemRef(index.row());

        return doc.Amount() >= 0 ? Qt::black : Qt::red;
    }
    return QVariant();
}

QVariant PaymentsModel::GetCellBackColor(const QModelIndex& index) const
{
    if (index.row() % 2)
    {
        return QColor(0xFAFAFA);
    }

    return QVariant();
}
