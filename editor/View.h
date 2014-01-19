#pragma once

#include <Eigen/Eigen>
#include <mye/core/IWindow.h>

class View
{

public:

	View(void);
	~View(void);

	virtual void Activate(void) { }
	virtual void Deactivate(void) { }

	virtual Eigen::Vector2i GetPosition(void) const;
	virtual void SetPosition(const Eigen::Vector2i &position);

	virtual Eigen::Vector2i GetSize(void) const;
	virtual void SetSize(const Eigen::Vector2i &size);

	virtual void Update(void) { }
	virtual void Render(void) { }

private:

	Eigen::Vector2i m_size;
	Eigen::Vector2i m_position;

};

