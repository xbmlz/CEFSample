#include "include/cef_app.h"

// Implement application-level callbacks for the browser process.
class SampleApp : public CefApp, public CefBrowserProcessHandler {

public:
	SampleApp();

    // CefApp methods:
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
        return this;
    }

    // CefBrowserProcessHandler methods:
    // virtual void OnContextInitialized() OVERRIDE;

private:
    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(SampleApp);
};