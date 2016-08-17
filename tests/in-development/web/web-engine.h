#ifndef WEBENGINE_H
#define WEBENGINE_H

#include <string>
#include "iweb-engine.h"

namespace hb
{
namespace core
{

class WebEngineProxy
{
public:

    static void Setup(const IWebEnginePtr& impl);
    static IWebEngine::RequestId SendRequest(const std::string& url,
                                             const std::string& request,
                                             const IRequestListenerPtr& callback);

private:
    WebEngineProxy() {}
private:
    static IWebEnginePtr   m_impl;
};

} // namespace core
} // namespace hg

#endif // WEBENGINE_H
