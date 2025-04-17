#pragma once

#include <functional>

class NObjectHelper
{
public:
	NObjectHelper(std::function<void()> create_func,
		std::function<void()> release_func):
		m_create_func(create_func), m_release_func(release_func)
	{
		if (m_create_func) { m_create_func(); }
	}

	~NObjectHelper()
	{
		if (m_release_func) { m_release_func(); }
	}

private:
	std::function<void()> m_create_func;
	std::function<void()> m_release_func;
};