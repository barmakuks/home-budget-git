#include "documents-model.h"

#include <QDate>
#include <QColor>

#include "model.h"
#include "string-format.h"

DocumentsModel::DocumentsModel()
{
    using namespace hb::core;

    Model& model = Model::GetInstance();

    m_accounts = model.GetAccounts();

    m_currencies = model.GetCurrencies();

    m_docTypes = model.GetTypeList();

    Reload(QDate::currentDate(), QDate::currentDate(), hb::EmptyId, hb::EmptyId);
}

void DocumentsModel::Reload(const QDate& minDate,
                            const QDate& maxDate,
                            const hb::AccountId accountId,
                            const hb::CurrencyId currencyId)
{
    Reload(minDate.toString("yyyyMMdd").toUtf8().constData(),
           maxDate.toString("yyyyMMdd").toUtf8().constData(),
           accountId,
           currencyId);
}

void DocumentsModel::Reload(const std::string& minDate,
                            const std::string& maxDate,
                            const hb::AccountId accountId,
                            const hb::CurrencyId currencyId)
{
    using namespace hb::core;

    m_documents = Model::GetInstance().GetDocuments(minDate, maxDate, accountId, currencyId);

    reset();
}

namespace
{
struct Columns
{
    enum {Date = 0, DocType, AccountFrom, AccountTo, AmountFrom, AmountTo, Note, Shop, Count};
};
}

int DocumentsModel::rowCount(const QModelIndex &parent) const
{
    int row_count = (m_documents) ? m_documents->size() : 0;

    return row_count;
}

int DocumentsModel::columnCount(const QModelIndex &parent) const
{
    return Columns::Count;
}

QVariant DocumentsModel::data(const QModelIndex &index, int role) const
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

QVariant DocumentsModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
        case Columns::Date:
            return QObject::tr("Дата");
        case Columns::DocType:
            return QObject::tr("Описание");
        case Columns::AccountFrom:
            return QObject::tr("Сч. расх.");
        case Columns::AccountTo:
            return QObject::tr("Сч. дох.");
        case Columns::AmountFrom:
            return QObject::tr("Сумма расх.");
        case Columns::AmountTo:
            return QObject::tr("Сумма дох.");
        case Columns::Note:
            return QObject::tr("Прим.");
        case Columns::Shop:
            return QVariant();
        }
    }

    return QVariant();
}


const hb::core::Document& DocumentsModel::GetDocumentItem(int index) const
{
    return *(m_documents->at(index));
}

QVariant DocumentsModel::GetCellString(const QModelIndex& index) const
{
    using namespace hb::core;

    const Document& doc = GetDocumentItem(index.row());

    switch (index.column())
    {
    case Columns::Date:
    {
        return QObject::tr(hb::utils::FormatDate(doc.DocDate()).c_str());
    }
    case Columns::DocType:
    {
        const auto it = m_docTypes->find(doc.DocType());

        if (it != m_docTypes->end())
        {
            return QObject::tr(it->second->Name().c_str());
        }

        return QVariant();
    }
    case Columns::AccountFrom:
    {
        if (doc.AmountFrom().is_initialized())
        {
            const hb::AccountId account = doc.AmountFrom().get().Account();

            return QObject::tr(hb::utils::FormatAccountName(*(m_accounts->at(account))).c_str());
        }
        return QVariant();
    }
    case Columns::AccountTo:
    {
        if (doc.AmountTo().is_initialized())
        {
            const hb::AccountId account = doc.AmountTo().get().Account();

            return QObject::tr(hb::utils::FormatAccountName(*(m_accounts->at(account))).c_str());
        }
        return QVariant();
    }
    case Columns::AmountFrom:
    {
        if (doc.AmountFrom().is_initialized())
        {
            std::string value = hb::utils::FormatMoney(doc.AmountFrom().get().Value());
            const hb::CurrencyId cur = doc.AmountFrom().get().Currency();

            value = value + " " + m_currencies->at(cur)->Symbol();

            return QObject::tr(value.c_str());
        }
        return QVariant();
    }
    case Columns::AmountTo:
    {
        if (doc.AmountTo().is_initialized())
        {
            std::string value = hb::utils::FormatMoney(doc.AmountTo().get().Value());
            const hb::CurrencyId cur = doc.AmountTo().get().Currency();

            value = value + " " + m_currencies->at(cur)->Symbol();
            return QObject::tr(value.c_str());
        }
        return QVariant();
    }
    case Columns::Note:
    {
        return QObject::tr(doc.Note().c_str());
    }
    case Columns::Shop:
    {
        return QObject::tr(doc.Shop().c_str());
    }
    default:
    {
        return QVariant("no data");
    }
    }
}

QVariant DocumentsModel::GetCellAlignment(const QModelIndex& index) const
{
    if (index.column() == Columns::AmountFrom
        || index.column() == Columns::AmountTo)
    {
        return Qt::AlignVCenter + Qt::AlignRight;
    }
    return Qt::AlignVCenter;
}

QVariant DocumentsModel::GetCellForecolor(const QModelIndex& index) const
{
    using namespace hb::core;

    const Document& doc = GetDocumentItem(index.row());

    if (index.column() == Columns::AccountFrom
        || index.column() == Columns::AmountFrom)
    {
        if (doc.AmountFrom().is_initialized())
        {
            const AccountPtr account = m_accounts->at(doc.AmountFrom()->Account());
            return QColor(account->ForegroundColor());
        }
    }
    else if (index.column() == Columns::AccountTo
        || index.column() == Columns::AmountTo)
    {
        if (doc.AmountTo().is_initialized())
        {
            const AccountPtr account = m_accounts->at(doc.AmountTo()->Account());
            return QColor(account->ForegroundColor());
        }
    }

    return QVariant();
}

QVariant DocumentsModel::GetCellBackColor(const QModelIndex &index) const
{
    if (index.row() % 2)
    {
        return QColor(0xFAFAFA);
    }
    return QVariant();
}
