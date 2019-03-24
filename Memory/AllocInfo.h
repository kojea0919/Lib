#pragma once


struct AllocInfo {
	void *	m_ptr;
	int		m_iMemorySize;
	char	m_cFileName[128];
	int		m_iFileLine;
};