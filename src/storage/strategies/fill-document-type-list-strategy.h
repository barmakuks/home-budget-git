#ifndef FILLDOCUMENTTYPELISTSTRATEGY_H
#define FILLDOCUMENTTYPELISTSTRATEGY_H

#include "idatabase-engine.h"
#include "documenttype.h"
namespace hb
{
namespace storage
{

class FillDocumentTypeListStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef core::DocumentTypeListPtr ResultType;

    FillDocumentTypeListStrategy();

    virtual void NewRecord();

    virtual void ApplyRecord();

    virtual void AddColumnValue(const std::string& fieldName, const std::string& value);

    const ResultType& Result() const
    {
        return m_typelist;
    }

private:
    core::DocumentTypeListPtr m_typelist;
    core::DocumentTypePtr     m_currentDocType;
};

} // namespace storage
} // namespace hb

#endif // FILLDOCUMENTTYPELISTSTRATEGY_H
