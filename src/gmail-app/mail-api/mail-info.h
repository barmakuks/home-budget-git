#ifndef MAILINFO_H
#define MAILINFO_H

#include <string>
#include <vector>

struct MailInfo
{
    std::string date;
    std::string sender;
    std::string receiver;
    std::string subject;
};

typedef std::vector<MailInfo> MailInfoList;

class IMailInfoReceiver
{
public:
    virtual void ReceiveMailInfo(const MailInfoList& list) = 0;
};

class IMailInfoRequest
{
public:
    virtual void RequestMailInfo(IMailInfoReceiver* receiver) = 0;
};

#endif // MAILINFO_H
