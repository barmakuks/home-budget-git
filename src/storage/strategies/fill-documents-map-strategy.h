#ifndef FILLDOCUMENTSMAPSTRATEGY_H
#define FILLDOCUMENTSMAPSTRATEGY_H

#include "idatabase-engine.h"
#include "document.h"
namespace hb
{
namespace storage
{

class FillDocumentsMapStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef core::DocumentsMapPtr ResultType;

    FillDocumentsMapStrategy();

    virtual void NewRecord();

    virtual void ApplyRecord();

    virtual void AddColumnValue(const std::string& fieldName, const std::string& value);

    const ResultType& Result() const
    {
        return m_documents;
    }

private:
    typedef ResultType::element_type    map_type;
    typedef map_type::mapped_type       record_type;
    typedef record_type::element_type   doc_type;

    ResultType  m_documents;
    record_type m_currentDoc;

    std::shared_ptr<core::Amount> m_fromAmount;
    std::shared_ptr<core::Amount> m_toAmount;

};

} // namespace storage
} // namespace hb

#endif // FILLDOCUMENTSMAPSTRATEGY_H
