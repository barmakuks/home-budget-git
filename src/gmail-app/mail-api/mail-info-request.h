#ifndef MAILINFOREQUEST_H
#define MAILINFOREQUEST_H

#include "mail-info.h"
#include <memory>

typedef std::shared_ptr<IMailInfoRequest>  IMailInfoRequestPtr;

class MailInfoRequest
{
public:
    static void Init(const IMailInfoRequestPtr& impl);
    static void RequestMailInfo(IMailInfoReceiver* receiver);
private:
    static IMailInfoRequestPtr m_impl;
};

#endif // MAILINFOREQUEST_H
