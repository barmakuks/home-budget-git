#ifndef WEBENGINE_H
#define WEBENGINE_H

#include <string>
#include <memory>

#include "assert_macro.h"
#include "iweb-engine.h"

namespace hb
{
namespace core
{

class WebEngine
{
public:

    static void Setup(const IWebEnginePtr& impl);
    static IWebEngine::RequestId SendRequest(const std::string& url,
                                             const std::string& request,
                                             const IRequestListenerPtr& callback);

private:
    WebEngine(){}
private:
    static IWebEnginePtr   m_impl;
};

} // namespace core
} // namespace hg

#endif // WEBENGINE_H
