#ifndef QTWEBENGINE_H
#define QTWEBENGINE_H

#include "iweb-engine.h"
#include <QNetworkAccessManager>

#include <memory>
#include <map>

class QNetworkReply;
class QNetworkAccessManager;

namespace hb
{
namespace web
{

class QtWebEngine:
    public QObject,
    public hb::core::IWebEngine
{
    Q_OBJECT
public:
    QtWebEngine();
    virtual ~QtWebEngine() {}

    // IWebEngine interface
public:
    virtual RequestId SendRequest(const std::string& url, const std::string& request, const core::IRequestListenerPtr& callback);

private slots:
    void finishedSlot(QNetworkReply* reply);

private:
    std::shared_ptr<QNetworkAccessManager> m_manager;
    typedef std::map<QNetworkReply*, core::IRequestListenerPtr> CallbackMap;

    CallbackMap m_callbacks;
    RequestId   m_index;
};

} // namespace web
} // namespace hb
#endif // QTWEBENGINE_H
