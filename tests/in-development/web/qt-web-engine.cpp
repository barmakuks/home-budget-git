#include "qt-web-engine.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <iostream>

#include <pthread.h>

namespace
{
    pthread_mutex_t locker_mutex;
}


namespace hb
{
namespace web
{
QtWebEngine::QtWebEngine():
    m_manager(new  QNetworkAccessManager(0)),
    m_index(0)
{
    QObject::connect(m_manager.get(), SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
}

hb::core::IWebEngine::RequestId QtWebEngine::SendRequest(const std::string& url,
        const std::string& request,
        const core::IRequestListenerPtr& callback)
{
    pthread_mutex_lock(&locker_mutex);

    const std::string qurl = url + "?" + request;

    QNetworkReply* reply = m_manager->get(QNetworkRequest(QUrl(qurl.c_str())));

    m_callbacks.insert(CallbackMap::value_type(reply, callback));

    m_requests.insert(RequestsMap::value_type(reply, ++m_index));

    pthread_mutex_unlock(&locker_mutex);

    return m_index;
}

void QtWebEngine::finishedSlot(QNetworkReply* reply)
{

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here
        QByteArray bytes = reply->readAll();  // bytes
        QString response(bytes); // string

        CallbackMap::const_iterator cb = m_callbacks.find(reply);
        RequestsMap::const_iterator rb = m_requests.find(reply);

        if (cb != m_callbacks.end() && cb->second && rb != m_requests.end())
        {
            cb->second->OnWebResponseRecieved(m_requests[reply], response.toUtf8().constData());

            pthread_mutex_lock(&locker_mutex);
            m_callbacks.erase(m_callbacks.find(reply));
            m_requests.erase(m_requests.find(reply));
            pthread_mutex_unlock(&locker_mutex);
        }
    }
    // Some http error received
    else
    {
        // handle errors here
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    reply->deleteLater();
}


} // namespace web
} // namespace hb
