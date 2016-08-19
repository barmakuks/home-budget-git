#include "get-doc-engine.h"
#include <engine.h>

namespace
{
std::unique_ptr<hb::DocEngine> doc_engine;
}

hb::DocEngine& hb::InitDocEngine(const storage::StoragePtr& storage)
{
    assert(!doc_engine);
    doc_engine.reset(new hb::DocEngine(storage));

    return *doc_engine;
}

hb::DocEngine& hb::GetDocEngine()
{
    assert(doc_engine);

    return *doc_engine;
}
