#include <iostream>

#include "mail-info.h"
#include "mail-info-request.h"
#include "gmail-api.h"

#include <boost/python.hpp>

#include <semaphore.h>
#include <thread>

using namespace std;

sem_t semaphore;

class Receiver: public IMailInfoReceiver
{
    // IMailInfoReceiver interface
public:
    virtual void ReceiveMailInfo(const MailInfoList &list)
    {
        for (MailInfo info: list)
        {
            std::cout << "**********************" << std::endl;
            std::cout << info.date << std::endl;
            std::cout << "From: " << info.sender << std::endl;
            std::cout << "To: " << info.receiver << std::endl;
            std::cout << "Subj: " << info.subject << std::endl;
        }

        sem_post(&semaphore);
    }

}recevier;

int main(int argc, char **argv)
{
    IMailInfoRequestPtr request(new GMailInfoRequest(argc, argv));
    MailInfoRequest::Init(request);

    sem_init(&semaphore, 0, -1);

    std::cout << "********************** Create request**************************" << std::endl;

    pthread_t t;
    if (pthread_create(&t,
                       NULL,
                       [](void* /*data*/)->void*{ MailInfoRequest::RequestMailInfo(&recevier); return NULL;},
                       NULL))
    {
        return -1;
    }

    std::cout << "********************** Wait for response***********************" << std::endl;
    sem_wait(&semaphore);
//    pthread_join(t, NULL);
    std::cout << "********************** Responce received***********************" << std::endl;


    return 0;
}

