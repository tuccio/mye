#pragma once

#include <mye/math/Math.h>
#include <mye/core/IWindow.h>

class View
{

public:

	View(void);
	~View(void);

	virtual void Activate(void) { }
	virtual void Deactivate(void) { }

	virtual mye::math::Vector2i GetPosition(void) const;
	virtual void SetPosition(const mye::math::Vector2i &position);

	virtual mye::math::Vector2i GetSize(void) const;
	virtual void SetSize(const mye::math::Vector2i &size);

	virtual void Update(void) { }
	virtual void Render(void) { }

private:

	mye::math::Vector2i m_size;
	mye::math::Vector2i m_position;

};

