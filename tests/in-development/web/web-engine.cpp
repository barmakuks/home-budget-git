#include <web-engine.h>

namespace hb
{
namespace core
{

IWebEnginePtr WebEngineProxy::m_impl;

void
WebEngineProxy::Setup(const IWebEnginePtr& impl)
{
    ASSERT_RETURN_VOID(!m_impl);
    m_impl = impl;
}

IWebEngine::RequestId
WebEngineProxy::SendRequest(const std::string& url,
                            const std::string& request,
                            const IRequestListenerPtr& callback)
{
    ASSERT_RETURN(m_impl, -1);

    return m_impl->SendRequest(url, request, callback);
}

} // namespace core
} // namespace hg

