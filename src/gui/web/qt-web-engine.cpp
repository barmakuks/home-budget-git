#include "qt-web-engine.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <iostream>

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
    const std::string qurl = url + request;

    QNetworkReply* reply = m_manager->get(QNetworkRequest(QUrl(qurl.c_str())));

    m_callbacks.insert(CallbackMap::value_type(reply, callback));

    return ++m_index;
}

void QtWebEngine::finishedSlot(QNetworkReply* reply)
{
//    // Reading attributes of the reply e.g. the HTTP status code
//    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//    // Or the target URL if it was a redirect:
//    QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
//    // see CS001432 on how to handle this

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        // read data from QNetworkReply here
        QByteArray bytes = reply->readAll();  // bytes
        QString response(bytes); // string

        CallbackMap::const_iterator cb = m_callbacks.find(reply);

        if (cb != m_callbacks.end() && cb->second)
        {
            cb->second->OnWebResponseRecieved(m_index, response.toUtf8().constData());

            m_callbacks.erase(cb);
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
