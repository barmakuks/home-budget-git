#ifndef ALLFILTER_H
#define ALLFILTER_H

namespace hb
{
namespace storage
{

class AllDocumentsFilter: public core::IFilter
{
public:
    AllDocumentsFilter()
    {
    }

    std::string From() const
    {
        return "documents";
    }

    std::string WhereCondition() const
    {
        return "";
    }
};

} // namespace storage
} // namespace hb

#endif // ALLFILTER_H
