#include "include/cef_client.h"
#include <list>

class SampleHandler : public CefClient,
    public CefDisplayHandler,
    public CefKeyboardHandler,
    public CefDownloadHandler,
    public CefLifeSpanHandler,
    public CefLoadHandler, 
    public CefContextMenuHandler{
public:
    SampleHandler();
    ~SampleHandler();

    // Provide access to the single global instance of this object.
    static SampleHandler* GetInstance();

    // CefClient methods:
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
        return this;
    }
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
        return this;
    }
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
        return this;
    }

    // CefDisplayHandler methods:
    virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
        const CefString& title) OVERRIDE;

    // CefLifeSpanHandler methods:
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

    // CefLoadHandler methods:
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        ErrorCode errorCode,
        const CefString& errorText,
        const CefString& failedUrl) OVERRIDE;

    // Request that all existing browser windows close.
    void CloseAllBrowsers(bool force_close);

    bool IsClosing() const { return is_closing_; }

    // EXT
    // Show Devtools
    void ShowDevelopTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at);
    void CloseDevelopTools(CefRefPtr<CefBrowser> browser);

    // F5
    virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() //实现  
    {
        return this;
    }
    virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
        const CefKeyEvent& event,
        CefEventHandle os_event,
        bool* is_keyboard_shortcut) OVERRIDE;
    virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
        const CefKeyEvent& event,
        CefEventHandle os_event) OVERRIDE;


    // ContextMenu
    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() //实现  
    {
        return this;
    }
    virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefContextMenuParams> params,
        CefRefPtr<CefMenuModel> model) OVERRIDE;

    virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefContextMenuParams> params,
        int command_id,
        EventFlags event_flags) OVERRIDE;

    //download
    virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() { return this; }
    virtual void OnBeforeDownload(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefDownloadItem> download_item,
        const CefString& suggested_name,
        CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
    virtual void OnDownloadUpdated(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefDownloadItem> download_item,
        CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

private:
    // List of existing browser windows. Only accessed on the CEF UI thread.
    typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
    BrowserList browser_list_;

    bool is_closing_;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(SampleHandler);
};