// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <iostream>
#include <string.h>

#include "CStringReader.h"


void deleter(void* tmpPtr) {
    if (tmpPtr) UnmapViewOfFile(tmpPtr);
}

CStringReader::CStringReader(PCWSTR pathFile) noexcept :m_bSuccess(TRUE), m_hFile(nullptr), m_lpMap(nullptr, &deleter), 
                                                                                                m_pPos(nullptr), m_liSize{0,0} {
    InitializeCriticalSection(&m_section);
    //получение HANDLE для файла
    ATL::CHandle FileHandle(CreateFile(pathFile, GENERIC_READ, FILE_SHARE_READ, nullptr, 
                                                    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0)); 
    if (FileHandle == INVALID_HANDLE_VALUE) {
        m_bSuccess = FALSE;
        return;
    }
    if (!GetFileSizeEx(FileHandle, &m_liSize)) {
        m_bSuccess = FALSE;
        return;
    }
    
    //маппируем файл в память
    HANDLE tmpH = CreateFileMapping(FileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr); 
    if (tmpH == INVALID_HANDLE_VALUE || tmpH == nullptr) {
        m_bSuccess = FALSE;
        std::cout<<GetLastError();
        return;
    }
    m_hFile.Attach(tmpH);

    //открываем файл в виртуальной памяти
    std::unique_ptr<void, void(*)(void*)> tmpPtr(MapViewOfFile(m_hFile, FILE_MAP_READ, 0, 0, 0), &deleter); 
    m_lpMap = move(tmpPtr);
             
    if (m_lpMap == nullptr) {
        m_bSuccess = FALSE;
    }
    m_pPos = reinterpret_cast<char*>(m_lpMap.get());
}
CStringReader::~CStringReader()
{
    DeleteCriticalSection(&m_section);
}


const BOOL CStringReader::IsInizializationCorrect() const noexcept
{
    return m_bSuccess;
}

//Функция чтения строк из файла
std::string CStringReader::GetCurrentString()                                       
{
    EnterCriticalSection(&m_section);
    if (m_pPos == reinterpret_cast<char*>(m_lpMap.get()) + m_liSize.QuadPart)
    {
        LeaveCriticalSection(&m_section);
        throw std::exception("End of File");        
    }
    char* tmpPtr = strchr(m_pPos, '\n');
    std::string tmpString;
    if (tmpPtr != nullptr) {
       
        tmpString.insert(0,m_pPos, tmpPtr - m_pPos);        
        m_pPos += tmpString.size() + 1;        
    }
    else {
        tmpString.insert(0, m_pPos);
        m_pPos += tmpString.size();        
    }
    LeaveCriticalSection(&m_section);
    return tmpString;    
}
