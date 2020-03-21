#include "SampleHandler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace {

    SampleHandler* g_instance = NULL;

}  // namespace

enum {
    ID_CMD_REFRESH = 0,
    ID_SHOW_DEV_TOOLS
};

SampleHandler::SampleHandler()
    : is_closing_(false) {
    DCHECK(!g_instance);
    g_instance = this;
}

SampleHandler::~SampleHandler() {
    g_instance = NULL;
}

// static
SampleHandler* SampleHandler::GetInstance() {
    return g_instance;
}

void SampleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();

    // Add to the list of existing browsers.
    browser_list_.push_back(browser);
}

bool SampleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();

    // Closing the main window requires special handling. See the DoClose()
    // documentation in the CEF header for a detailed destription of this
    // process.
    if (browser_list_.size() == 1) {
        // Set a flag to indicate that the window close should be allowed.
        is_closing_ = true;
    }

    // Allow the close. For windowed browsers this will result in the OS close
    // event being sent.
    return false;
}

void SampleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();

    // Remove from the list of existing browsers.
    BrowserList::iterator bit = browser_list_.begin();
    for (; bit != browser_list_.end(); ++bit) {
        if ((*bit)->IsSame(browser)) {
            browser_list_.erase(bit);
            break;
        }
    }

    if (browser_list_.empty()) {
        // All browser windows have closed. Quit the application message loop.
        CefQuitMessageLoop();
    }
}

void SampleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    ErrorCode errorCode,
    const CefString& errorText,
    const CefString& failedUrl) {
    CEF_REQUIRE_UI_THREAD();

    // Don't display an error for downloaded files.
    if (errorCode == ERR_ABORTED)
        return;

    // Display a load error message.
    std::stringstream ss;
    ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL " << std::string(failedUrl) <<
        " with error " << std::string(errorText) << " (" << errorCode <<
        ").</h2></body></html>";
    frame->LoadString(ss.str(), failedUrl);
}

void SampleHandler::CloseAllBrowsers(bool force_close) {
    if (!CefCurrentlyOn(TID_UI)) {
        // Execute on the UI thread.
        CefPostTask(TID_UI,
            base::Bind(&SampleHandler::CloseAllBrowsers, this, force_close));
        return;
    }

    if (browser_list_.empty())
        return;

    BrowserList::const_iterator it = browser_list_.begin();
    for (; it != browser_list_.end(); ++it)
        (*it)->GetHost()->CloseBrowser(force_close);
}

// Devtools
void SampleHandler::ShowDevelopTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at)
{
    CefWindowInfo windowInfo;
    CefBrowserSettings settings;
    windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), "DevTools");
    browser->GetHost()->ShowDevTools(windowInfo, this, settings, CefPoint());
}

void SampleHandler::CloseDevelopTools(CefRefPtr<CefBrowser> browser)
{
    browser->GetHost()->CloseDevTools();
}

// KeyEvent
bool SampleHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
    const CefKeyEvent& event,
    CefEventHandle os_event,
    bool* is_keyboard_shortcut)
{
    if (event.type != KEYEVENT_RAWKEYDOWN)
        return false;

    if (event.windows_key_code == 116)//F5刷新
        browser->Reload();
    else if (event.windows_key_code == VK_F12)
    {
        ShowDevelopTools(browser, CefPoint(0, 0));
    }
    return false;
}

bool SampleHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser,
    const CefKeyEvent& event,
    CefEventHandle os_event) {
    return false;
}

// ContextMenu
void SampleHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    CefRefPtr<CefMenuModel> model)
{
    if (model->GetCount() > 0) {// 刷新菜单
        model->AddSeparator();
        model->AddItem(ID_CMD_REFRESH, __TEXT("刷新"));
        model->AddItem(ID_SHOW_DEV_TOOLS, __TEXT("开发者选项"));
    }
}

bool SampleHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    int command_id, EventFlags event_flags)
{
    switch (command_id)
    {
    case ID_CMD_REFRESH:
        browser->Reload();
        break;
    case ID_SHOW_DEV_TOOLS:
    {
        ShowDevelopTools(browser, CefPoint(0, 0));
        break;
    }
    default:
        break;
    }
    return false;
}

void SampleHandler::OnBeforeDownload(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    const CefString& suggested_name,
    CefRefPtr<CefBeforeDownloadCallback> callback)
{
    callback->Continue(download_item->GetURL(), true);
}

void SampleHandler::OnDownloadUpdated(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    CefRefPtr<CefDownloadItemCallback> callback)
{
    if (download_item->IsComplete())
    {
        OutputDebugString(L"下载成功");
        if (browser->IsPopup() && !browser->HasDocument())
        {
            //browser->GetHost()->ParentWindowWillClose();
            browser->GetHost()->CloseBrowser(true);
        }
    }
    else
    {
        //如果取消应该跳转到一个网页
        browser->GoBack();
    }
}