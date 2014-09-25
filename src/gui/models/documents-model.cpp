#include "documents-model.h"

#include <QDate>
#include <QColor>

#include "engine.h"
#include "string-format.h"
#include "../convert-utils.h"

DocumentsModel::DocumentsModel()
{
    using namespace hb::core;

    Engine& model = Engine::GetInstance();

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
    Reload(hb::utils::NormalizeDate(minDate),
           hb::utils::NormalizeDate(maxDate),
           accountId,
           currencyId);
}

void DocumentsModel::Reload(const std::string& minDate,
                            const std::string& maxDate,
                            const hb::AccountId accountId,
                            const hb::CurrencyId currencyId)
{
    using namespace hb::core;

    m_documents = Engine::GetInstance().GetDocuments(minDate, maxDate, accountId, currencyId);

    reset();
}

namespace
{
struct Columns
{
    enum {Date = 0, DocType, AccountFrom, AccountTo, AmountFrom, AmountTo, Note, Shop, Count};
};
}

int DocumentsModel::rowCount(const QModelIndex& parent) const
{
    int row_count = (m_documents) ? m_documents->size() : 0;

    return row_count;
}

int DocumentsModel::columnCount(const QModelIndex& parent) const
{
    return Columns::Count;
}

QVariant DocumentsModel::data(const QModelIndex& index, int role) const
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
    using namespace hb::utils;

    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
        case Columns::Date:
        {
            return Tr("Дата");
        }
        case Columns::DocType:
        {
            return Tr("Описание");
        }
        case Columns::AccountFrom:
        {
            return Tr("Сч. расх.");
        }
        case Columns::AccountTo:
        {
            return Tr("Сч. дох.");
        }
        case Columns::AmountFrom:
        {
            return Tr("Сумма расх.");
        }
        case Columns::AmountTo:
        {
            return Tr("Сумма дох.");
        }
        case Columns::Note:
        {
            return Tr("Прим.");
        }
        case Columns::Shop:
        {
            return QVariant();
        }
        }
    }

    return QVariant();
}

const hb::core::Document& DocumentsModel::GetDocumentItemRef(int index) const
{
    return *GetDocumentItemPtr(index);
}

const hb::core::DocumentPtr& DocumentsModel::GetDocumentItemPtr(int index) const
{
    return m_documents->at(index);
}

QVariant DocumentsModel::GetCellString(const QModelIndex& index) const
{
    using namespace hb::core;
    using namespace hb::utils;

    const Document& doc = GetDocumentItemRef(index.row());

    switch (index.column())
    {
    case Columns::Date:
    {
        return Tr(hb::utils::FormatDate(doc.DocDate()));
    }

    case Columns::DocType:
    {
        const auto it = m_docTypes->find(doc.DocType());

        if (it != m_docTypes->end())
        {
            return Tr(it->second->Name());
        }

        return QVariant();
    }

    case Columns::AccountFrom:
    {
        if (doc.AmountFrom().is_initialized())
        {
            const hb::AccountId account = doc.AmountFrom().get().Account();

            return Tr(hb::utils::FormatAccountName(*(m_accounts->at(account))));
        }

        return QVariant();
    }

    case Columns::AccountTo:
    {
        if (doc.AmountTo().is_initialized())
        {
            const hb::AccountId account = doc.AmountTo().get().Account();

            return Tr(hb::utils::FormatAccountName(*(m_accounts->at(account))));
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

            return Tr(value);
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
            return Tr(value);
        }

        return QVariant();
    }

    case Columns::Note:
    {
        return Tr(doc.Note());
    }

    case Columns::Shop:
    {
        return Tr(doc.Shop());
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

    const Document& doc = GetDocumentItemRef(index.row());

    switch (index.column())
    {
    case Columns::AccountFrom:
    {
        if (doc.AmountFrom().is_initialized())
        {
            const AccountPtr account = m_accounts->at(doc.AmountFrom()->Account());
            return QColor(account->ForegroundColor());
        }
        break;
    }
    case Columns::AccountTo:
    {
        if (doc.AmountTo().is_initialized())
        {
            const AccountPtr account = m_accounts->at(doc.AmountTo()->Account());
            return QColor(account->ForegroundColor());
        }
        break;
    }
    case Columns::AmountFrom:
    {
        if (doc.AmountFrom().is_initialized())
        {
            const CurrencyPtr cur = m_currencies->at(doc.AmountFrom()->Currency());
            return QColor(cur->ForegroundColor());
        }
        break;
    }
    case Columns::AmountTo:
    {
        if (doc.AmountTo().is_initialized())
        {
            const CurrencyPtr cur = m_currencies->at(doc.AmountTo()->Currency());
            return QColor(cur->ForegroundColor());
        }
        break;
    }
    }

    return QVariant();
}

QVariant DocumentsModel::GetCellBackColor(const QModelIndex& index) const
{
    if (index.row() % 2)
    {
        return QColor(0xFAFAFA);
    }

    return QVariant();
}
