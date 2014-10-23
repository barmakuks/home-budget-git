#include "mail-info-request.h"

IMailInfoRequestPtr MailInfoRequest::m_impl;

void MailInfoRequest::Init(const IMailInfoRequestPtr& impl)
{
    m_impl = impl;
}

void MailInfoRequest::RequestMailInfo(IMailInfoReceiver* receiver)
{
    if (m_impl)
    {
        m_impl->RequestMailInfo(receiver);
    }
}
