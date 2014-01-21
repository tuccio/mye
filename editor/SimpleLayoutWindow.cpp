#include "SimpleLayoutWindow.h"

#define SLW_ROWS 0
#define SLW_COLS 1



SimpleLayoutWindow::SimpleLayoutWindow(void) :
	m_splits(1, 1),
	m_views(1),
	m_toolbar(NULL)
{
	m_views[0] = NULL;
	AddListener(&m_listener);
}


SimpleLayoutWindow::~SimpleLayoutWindow(void)
{
}

bool SimpleLayoutWindow::DispatchCommand(unsigned int id)
{

	if (!Window::DispatchCommand(id))
	{
		
		if (m_toolbar && m_toolbar->DispatchCommand(id))
		{
			return true;
		}

		return false;

	}

	return true;

}

mye::win::Toolbar* SimpleLayoutWindow::GetToolbar(void)
{
	return m_toolbar;
}

void SimpleLayoutWindow::SetToolbar(mye::win::Toolbar *toolbar)
{
	m_toolbar = toolbar;
}

Eigen::Vector2i SimpleLayoutWindow::GetScreenSplits(void) const
{
	return m_splits;
}

void SimpleLayoutWindow::SetSplitScreen(int x, int y)
{
	assert(x >= 0 && y >= 0);
	m_splits = Eigen::Vector2i(x, y);
	m_views.resize(m_splits.x() * m_splits.y(), NULL);
}

View* SimpleLayoutWindow::GetSplitView(int i, int j) const
{
	assert(i < m_splits.x() && j < m_splits.y());
	return m_views[i * m_splits[SLW_COLS] + j];
}

void SimpleLayoutWindow::SetSplitView(int i, int j, View *view)
{

	View *&viewPtr = m_views[i * m_splits[SLW_COLS] + j];

	if (viewPtr)
	{
		viewPtr->Deactivate();
	}

	viewPtr = view;
	
	viewPtr->SetPosition(_ComputeSplitPosition(i, j));
	viewPtr->SetSize(_ComputeSplitSize(i, j));
	viewPtr->Activate();

}

Eigen::Vector2i SimpleLayoutWindow::_ComputeSplitPosition(int i, int j)
{

	Eigen::Vector2i base(0, 0);

	if (m_toolbar)
	{
		if (m_toolbar->IsVertical())
		{
			base.x() = m_toolbar->GetSize().x();
		}
		else
		{
			base.y() = m_toolbar->GetSize().y();
		}
	}

	Eigen::Vector2i position(base);

	for (int x = 0; x < j; x++)
	{
		position.x() += GetSplitSize(i, x).x();
	}

	for (int y = 0; y < i; y++)
	{
		position.y() += GetSplitSize(y, j).y();
	}

	return position;

}

Eigen::Vector2i SimpleLayoutWindow::_ComputeSplitSize(int i, int j)
{

	Eigen::Vector2i size = GetSize();

	if (m_toolbar)
	{
		if (m_toolbar->IsVertical())
		{
			size.x() -= m_toolbar->GetSize().x();
		}
		else
		{
			size.y() -= m_toolbar->GetSize().y();
		}
	}

	return Eigen::Vector2i((float) size.x() / m_splits[SLW_COLS],
		(float) size.y() / m_splits[SLW_ROWS]);

}

void SimpleLayoutWindow::Update(void)
{

	for (View *view : m_views)
	{
		if (view)
		{
			view->Update();
		}
	}

}

void SimpleLayoutWindow::Render(void)
{

	for (View *view : m_views)
	{
		if (view)
		{
			view->Render();
		}
	}

}

Eigen::Vector2i SimpleLayoutWindow::GetSplitSize(int i, int j)
{
	View *view = GetSplitView(i, j);
	return (view ? view->GetSize() : Eigen::Vector2i::Zero());
}

Eigen::Vector2i SimpleLayoutWindow::GetSplitPosition(int i, int j)
{
	View *view = GetSplitView(i, j);
	return (view ? view->GetPosition() : Eigen::Vector2i(-1, -1));
}

void SimpleLayoutWindow::ResizeViews(void)
{

	for (int i = 0; i < m_splits[SLW_ROWS]; i++)
	{

		for (int j = 0; j < m_splits[SLW_COLS]; j++)
		{

			View *view = GetSplitView(i, j);
			auto size = _ComputeSplitSize(i, j);

			if (view)
			{
				view->SetSize(size);
				view->SetPosition(_ComputeSplitPosition(i, j));
			}
			
		}

	}
}