#include "documents-model.h"

#include <QDate>
#include <QColor>

#include "engine.h"
#include "string-format.h"
#include "utils/convert-utils.h"
#include "hb_chrono.h"

DocumentsModel::DocumentsModel()
{
    hb::Engine& model = hb::Engine::GetInstance();

    m_accounts = model.GetAccounts();

    m_currencies = model.GetCurrencies();

    Reload(QDate::currentDate(), QDate::currentDate(), hb::EmptyId, hb::EmptyId,
           hb::DocTypeIdList());
}

void DocumentsModel::Reload(const QDate& minDate,
                            const QDate& maxDate,
                            const hb::AccountId accountId,
                            const hb::CurrencyId currencyId,
                            const hb::DocTypeIdList& doc_types)
{
    Reload(hb::qt_utils::NormalizeDate(minDate), hb::qt_utils::NormalizeDate(maxDate), accountId,
           currencyId, doc_types);
}

void DocumentsModel::Reload(const std::string& minDate,
                            const std::string& maxDate,
                            const hb::AccountId accountId,
                            const hb::CurrencyId currencyId,
                            const hb::DocTypeIdList& doc_types)
{
    beginResetModel();
    m_documents = hb::Engine::GetInstance().GetDocuments(minDate, maxDate, accountId, currencyId,
                                                         doc_types);

    endResetModel();
}

namespace
{
struct Columns
{
    enum
    {
        Date = 0,
        DocType,
        AmountFrom,
        AmountTo,
        AccountFrom,
        AccountTo,
        Note,
        Shop,
        Count
    };
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
    case Qt::TextAlignmentRole:
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
    using namespace hb::qt_utils;

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
            return Tr("Опис");
        }

        case Columns::AccountFrom:
        {
            return Tr("Рах. витр.");
        }

        case Columns::AccountTo:
        {
            return Tr("Рах. дох.");
        }

        case Columns::AmountFrom:
        {
            return Tr("Сума витрат");
        }

        case Columns::AmountTo:
        {
            return Tr("Сума дохід");
        }

        case Columns::Note:
        {
            return Tr("Примітка");
        }

        case Columns::Shop:
        {
            return QVariant();
        }
        }
    }

    return QVariant();
}

const hb::Document& DocumentsModel::GetDocumentItemRef(int index) const
{
    return *GetDocumentItemPtr(index);
}

const hb::DocumentPtr& DocumentsModel::GetDocumentItemPtr(int index) const
{
    return m_documents->at(index);
}

QVariant DocumentsModel::GetCellString(const QModelIndex& index) const
{
    using namespace hb;
    using namespace hb::qt_utils;

    const Document& doc = GetDocumentItemRef(index.row());

    switch (index.column())
    {
    case Columns::Date:
    {
        return Tr(hb::qt_utils::FormatDate(doc.DocDate()));
    }

    case Columns::DocType:
    {
        hb::DocumentTypeListPtr docTypes = Engine::GetInstance().GetTypeList();

        const auto it = docTypes->find(doc.DocType());

        if (it != docTypes->end())
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

            return Tr(hb::qt_utils::FormatAccountName(*(m_accounts->at(account))));
        }

        return QVariant();
    }

    case Columns::AccountTo:
    {
        if (doc.AmountTo().is_initialized())
        {
            const hb::AccountId account = doc.AmountTo().get().Account();

            return Tr(hb::qt_utils::FormatAccountName(*(m_accounts->at(account))));
        }

        return QVariant();
    }

    case Columns::AmountFrom:
    {
        if (doc.AmountFrom().is_initialized())
        {
            std::string value = hb::qt_utils::FormatMoney(doc.AmountFrom().get().Value());
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
            std::string value = hb::qt_utils::FormatMoney(doc.AmountTo().get().Value());
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
    if (index.column() == Columns::AmountFrom || index.column() == Columns::AmountTo)
    {
        return Qt::AlignVCenter + Qt::AlignRight;
    }

    return Qt::AlignVCenter;
}

QVariant DocumentsModel::GetCellForecolor(const QModelIndex& index) const
{
    using namespace hb;
    using namespace hb::qt_utils;

    const Document& doc = GetDocumentItemRef(index.row());

    switch (index.column())
    {
    case Columns::Date:
    {
        if (hb::chrono::GetWeekDay(doc.DocDate()) == hb::chrono::WeekDays[hb::chrono::Sunday])
        {
            return QColor(0xB00000);
        }

        break;
    }

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
