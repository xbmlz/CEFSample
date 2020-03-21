#include "SampleHandler.h"
#include  <tchar.h>

enum {
	ID_CMD_REFRESH = 0,
	ID_SHOW_DEV_TOOLS
};

SampleHandler::SampleHandler()
{
	hWnd_ = NULL;
}


SampleHandler::~SampleHandler()
{
}

void SampleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	if (!m_cefBrowser.get())
	{
		m_cefBrowser = browser;
		m_BrowserHwnd = browser->GetHost()->GetWindowHandle();
	}
}

// Download
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
		//MessageBox.Show("下载成功");
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

// DevelopTools
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

bool SampleHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	////同一个browser可能有多个浏览器窗口(在新的子窗口打开链接,而不是在当前窗口跳转时会出现.),此时返回值可能会被用来做一些特别处理.想了解请参考官方demo.
	//if (CefCurrentlyOn(TID_UI))
	//{
	//	return true;
	//}
	return false;	//返回true取消关闭
}
void SampleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	if (m_BrowserHwnd == browser->GetHost()->GetWindowHandle())
	{
		// 浏览器窗口被销毁的话,browser指针置为NULL.
		m_cefBrowser = NULL;
	}

}
