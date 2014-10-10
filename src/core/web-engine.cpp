#include "web-engine.h"

namespace hb
{
namespace core
{

IWebEnginePtr WebEngine::m_impl;

void WebEngine::Setup(const IWebEnginePtr& impl)
{
    ASSERT_RETURN_VOID(!m_impl);
    m_impl = impl;
}

IWebEngine::RequestId WebEngine::SendRequest(const std::string& url,
        const std::string& request,
        const IRequestListenerPtr &callback)
{
    ASSERT_RETURN(m_impl, -1);

    return m_impl->SendRequest(url, request, callback);
}

} // namespace core
} // namespace hg

