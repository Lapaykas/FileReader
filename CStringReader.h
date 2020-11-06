#pragma once
#include <windows.h>
#include <string> 
#include <atlbase.h>
#include <memory>

class CStringReader final
{
public:
	CStringReader(PCWSTR)					noexcept;
	~CStringReader()						noexcept;
	const BOOL IsInizializationCorrect()	const noexcept;
	std::string GetCurrentString()			;

private:
	std::unique_ptr<void, void(*)(void*)>  m_lpMap;
	BOOL	m_bSuccess;
	char*	m_pPos;
	LARGE_INTEGER m_liSize;
	ATL::CHandle m_hFile;
	CRITICAL_SECTION m_section;
};

