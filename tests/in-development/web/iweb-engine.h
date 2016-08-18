#pragma once

#include <string>
#include <memory>

namespace hb
{

class IRequestListener;

typedef std::shared_ptr<IRequestListener> IRequestListenerPtr;

class IWebEngine
{
public:
    virtual ~IWebEngine(){}

    typedef int RequestId;

public:

    /** Sends request on specified url
      @param url [in] URL for request
      @param request Request body
      @param callback reference to function which will be called on responce recieve
      @returns Unique request identifier. If error returns -1.
    */
    virtual RequestId SendRequest(const std::string& url,
                                  const std::string& request,
                                  const IRequestListenerPtr& callback) = 0;
};

typedef std::shared_ptr<IWebEngine> IWebEnginePtr;

class IRequestListener
{
public:
    virtual void OnWebResponseRecieved(IWebEngine::RequestId requestId, const std::string& response) = 0;
};

} // namespace hg

