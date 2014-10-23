#ifndef GMAILAPI_H
#define GMAILAPI_H

#include "mail-info.h"

class GMailInfoRequest:
        public IMailInfoRequest,
        public IMailInfoReceiver
{
    // IMailInfoRequest interface
public:
    GMailInfoRequest(int argc, char** argv):
        m_argc(argc),
        m_argv(argv)
    {
    }

    virtual void RequestMailInfo(IMailInfoReceiver *receiver);

    // IMailInfoReceiver interface
public:
    virtual void ReceiveMailInfo(const MailInfoList &list);

private:
    IMailInfoReceiver*  m_receiver;
    const int           m_argc;
    char**              m_argv;
};



#endif // GMAILAPI_H
