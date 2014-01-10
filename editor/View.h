#pragma once

#include <Eigen/Eigen>

class View
{

public:

	View(void);
	~View(void);

	virtual void Resize(const Eigen::Vector2i &size) { }
	virtual void Update(void) { }
	virtual void Render(void) { }

};

