#include "ToolBar.h"
#include "Utils.h"

#include <mye/core/Logger.h>

#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

#undef max

using namespace mye::win;

ToolbarWCR::ToolbarWCR(void)
{

	INITCOMMONCONTROLSEX InitCtrlEx;

	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;

	m_registered = (InitCommonControlsEx(&InitCtrlEx) ? true : false);

	if (!m_registered)
	{

		auto logger = mye::core::Logger::GetSingletonPointer();
		std::string error = GetLastErrorAsString();

		if (logger)
		{
			logger->LogError("RegisterClass failed: " + error);
		}

		ShowErrorBox(error);

	}

}

ToolbarWCR::~ToolbarWCR(void)
{
}

bool ToolbarWCR::IsRegistered(void) const
{
	return m_registered;
}

/* Toolbar */

Toolbar::Toolbar(Window &window,
				 bool vertical) :
	m_parent(window),
	m_hImageList(NULL),
	m_iconSize(32, 32),
	m_showText(false)
{
	m_vertical = vertical;
}


Toolbar::~Toolbar(void)
{

	if (m_hImageList)
	{
		ImageList_Destroy(m_hImageList);
	}

}

void Toolbar::SetVertical(bool vertical)
{
	m_vertical = vertical;
}

void Toolbar::Create(void)
{

	if (!ToolbarWCR::GetSingletonPointer())
	{
		new ToolbarWCR;
	}

	m_hRebar = CreateWindowEx(
		0,
		REBARCLASSNAME,
		NULL,
		WS_CHILD |
		RBS_BANDBORDERS |
		WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN |
		CCS_NODIVIDER |
		(m_vertical ? CCS_VERT : 0x0),
		0,
		0,
		0,
		0,
		m_parent.GetHandle(),
		(HMENU) IDGenerator::GetSingleton().Generate(),
		NULL,
		NULL);

	if (!m_hRebar)
	{
		ShowErrorBox();
		return;
	}

	m_hToolbar = CreateWindowEx(
		0,
		TOOLBARCLASSNAME,
		NULL,
		WS_CHILD |
		TBSTYLE_FLAT |
		TBSTYLE_TOOLTIPS |
		(m_vertical ? CCS_VERT | TBSTYLE_WRAPABLE : 0x0),
		0,
		0,
		0,
		0,
		m_hRebar,
		NULL,
		NULL,
		NULL);

	if (m_hToolbar)
	{

		m_hImageList = ImageList_Create(
			m_iconSize.x(),
			m_iconSize.y(),
			ILC_MASK | ILC_COLOR24,
			m_buttons.size(),
			0);

		SendMessage(m_hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0);

		std::vector<TBBUTTON> buttons(m_buttons.size());
		ZeroMemory(&buttons[0], sizeof(TBBUTTON) * buttons.size());

		for (auto it : m_buttons)
		{

			static int i = 0;

			SetLastError(0);

			HBITMAP hImage = (HBITMAP) LoadImage(NULL,
				it.second.icon.c_str(),
				IMAGE_BITMAP,
				0,
				0,
				LR_LOADFROMFILE | LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS);

			MakeTransparentBitmap(hImage, GetDC(m_parent.GetHandle()));

			if (!hImage)
			{
				ShowErrorBox();
			}

			int iBitmap = ImageList_Add(m_hImageList, hImage, NULL);
			DeleteObject(hImage);

			if (iBitmap < 0)
			{
				ShowErrorBox();
			}

			Button &button = m_buttons[it.first];

			buttons[i].iBitmap   = i;
			buttons[i].idCommand = it.first;
			buttons[i].fsState   = TBSTATE_ENABLED | TBSTATE_WRAP;
			buttons[i].fsStyle   = BTNS_BUTTON;
			buttons[i].iString   = (!m_showText || button.text.empty() ?
				-1 :
			(INT_PTR) button.text.c_str());

			i++;

		}

		SendMessage(m_hToolbar, TB_ADDBUTTONS, (WPARAM) buttons.size(), (LPARAM) &buttons[0]);
		SendMessage(m_hToolbar, TB_SETIMAGELIST, (WPARAM) 0L, (LPARAM) m_hImageList);

		REBARBANDINFO rebarBandInfo;
		ZeroMemory(&rebarBandInfo, sizeof(REBARBANDINFO));

		DWORD dwBtnSize = (DWORD) SendMessage(m_hToolbar, TB_GETBUTTONSIZE, 0L, 0x0);

		rebarBandInfo.cbSize     = REBARBANDINFO_V3_SIZE;
		rebarBandInfo.fMask      = RBBIM_COLORS | RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE;
		rebarBandInfo.fStyle     = RBS_TOOLTIPS | RBS_AUTOSIZE | RBS_BANDBORDERS | RBBS_CHILDEDGE;
		rebarBandInfo.clrFore    = GetSysColor(COLOR_BTNTEXT);
		rebarBandInfo.clrBack    = GetSysColor(COLOR_BTNFACE);
		rebarBandInfo.fStyle     = RBBS_CHILDEDGE;
		rebarBandInfo.hwndChild  = m_hToolbar;
		rebarBandInfo.cxMinChild = LOWORD(dwBtnSize);
		rebarBandInfo.cyMinChild = LOWORD(dwBtnSize);
		rebarBandInfo.cx         = (m_vertical ? HIWORD(dwBtnSize) : HIWORD(dwBtnSize) * m_buttons.size());

		SendMessage(m_hRebar, RB_INSERTBAND, -1, (LPARAM) &rebarBandInfo);

	}
	else
	{
		ShowErrorBox();
	}

}

void Toolbar::AddButton(const std::string &icon,
						const CallbackType &f,
						const std::string &text)
{
	m_buttons[IDGenerator::GetSingleton().Generate()] = Button(icon, f, text);
}

bool Toolbar::DispatchCommand(unsigned int id, unsigned int code)
{
	
	auto it = m_buttons.find(id);
	
	if (it == m_buttons.end())
	{
		return false;
	}

	it->second.function();

	return true;

}

void Toolbar::ShowText(void)
{
	m_showText = true;
}

void Toolbar::HideText(void)
{
	m_showText = false;
}

mye::math::Vector2i Toolbar::GetSize(void) const
{

	RECT rect;

	BOOL success = GetWindowRect(m_hRebar, &rect);
	
	if (!success)
	{
		return mye::math::Vector2i(0, 0);
	}

	return mye::math::Vector2i(rect.right - rect.left,
		rect.bottom - rect.top);

}

mye::math::Vector2i Toolbar::GetIconSize(void)
{
	return m_iconSize;
}

void Toolbar::SetIconSize(const mye::math::Vector2i &iconSize)
{
	m_iconSize = iconSize;
}

void Toolbar::Show(void)
{
	ShowWindow(m_hToolbar, SW_SHOW);
	ShowWindow(m_hRebar, SW_SHOW);
}

void Toolbar::Hide(void)
{
	ShowWindow(m_hToolbar, SW_HIDE);
	ShowWindow(m_hRebar, SW_HIDE);
}

void Toolbar::AutoSize(void)
{
	SendMessage(m_hToolbar, TB_AUTOSIZE, (WPARAM) 0x0, (LPARAM) 0x0);
	UpdateWindow(m_hRebar);
}

LRESULT CALLBACK Toolbar::WindowProc(HWND hWnd,
									 UINT uMsg,
									 WPARAM wParam,
									 LPARAM lParam)
{

	switch (uMsg)
	{

	case WM_CREATE:

		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

	return 0;

}