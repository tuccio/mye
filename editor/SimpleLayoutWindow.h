#pragma once

#include "View.h"

#include <mye/win/Window.h>
#include <mye/win/Toolbar.h>
#include <vector>

class SimpleLayoutWindow :
	public mye::win::Window
{

public:

	typedef std::vector<View*> ViewsList;
	typedef std::vector<View*>::iterator ViewsIterator;

	SimpleLayoutWindow(void);
	~SimpleLayoutWindow(void);

	bool DispatchCommand(unsigned int id, unsigned int code);

	mye::win::Toolbar* GetToolbar(void);
	void SetToolbar(mye::win::Toolbar *toolbar);

	mye::math::Vector2i GetScreenSplits(void) const;
	void SetSplitScreen(int x, int y);

	View* GetSplitView(int i, int j) const;
	void SetSplitView(int i, int j, View *view);

	mye::math::Vector2i GetSplitPosition(int i, int j);
	mye::math::Vector2i GetSplitSize(int i, int j);

	void Update(void);
	void Render(void);

	void ResizeViews(void);

	const mye::core::Keyboard& Keyboard(void) const;
	const mye::core::Mouse& Mouse(void) const;

private:

	mye::math::Vector2i _ComputeSplitPosition(int i, int j);
	mye::math::Vector2i _ComputeSplitSize(int i, int j);

	mye::core::Keyboard m_keyboard;
	mye::core::Mouse m_mouse;

	mye::math::Vector2i m_splits;
	ViewsList m_views;
	mye::win::Toolbar *m_toolbar;

	class LayoutUpdater :
		public Listener
	{

	public:

		void OnResize(IWindow *window, const mye::math::Vector2i &size)
		{
			static_cast<SimpleLayoutWindow*>(window)->ResizeViews();
		}

	} m_listener;

};

