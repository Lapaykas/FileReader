// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <windows.h>
#include <atlbase.h>
#include <chrono>
#include <map>
#include <vector>

#include "CStringReader.h"
#include "TrieHeap.h"

struct params {
CRITICAL_SECTION section;
CStringReader* STRINGREADER;
node* root;
MinHeap* HEAP;
};


void WordsFromStringToTrie(node* root,const std::string &argString) noexcept
{
    char* pPos = nullptr;
    char* word = nullptr;
    static const char arrayPunctuationMarks[] = " <>_~`&%@^+-=[]{}\\|/!?$;1234567890()#*:,.'\"\t\n\r";
    std::vector <char> tmpArrayWords(argString.size()+1, '\0');
    memcpy(tmpArrayWords.data(), argString.c_str(), argString.size());      

    word = strtok_s(tmpArrayWords.data(), arrayPunctuationMarks, &pPos);   
    if (word == nullptr)
    {
        return;
    }
    add(root, word);
    while (word != nullptr)
    {
        word = strtok_s(nullptr, arrayPunctuationMarks, &pPos);
        if (word == nullptr)
        {
            break;
        }
        add(root, word);
    }    
}

unsigned WINAPI FindWords(LPVOID lpParam) noexcept
{
    params* sParamsToFunc = (params*)lpParam;
    std::string tmpString;    
        do
        {    
            try {
            tmpString = sParamsToFunc->STRINGREADER->GetCurrentString();            
            }
            catch (const std::exception& e) {                
                break;
            }                      
            std::vector <std::string> WordsVector;            
            EnterCriticalSection(&sParamsToFunc->section);                       
            WordsFromStringToTrie(sParamsToFunc->root, tmpString);
            LeaveCriticalSection(&sParamsToFunc->section);            
        } while (true);
   
     return 0;
}

//функция создания потоков и ожидающая их завершение
VOID CreateThreadsCrititcalSection(PCWSTR path,int countThreads, std::vector<HANDLE> &vectodHandle) noexcept
{  
    params paramToThread;
    CStringReader READER(path);
    MinHeap HEAP(20);
    node* root = new node();
    if (!READER.IsInizializationCorrect())
    {
        return;
    }
    paramToThread.STRINGREADER = &READER;   
    paramToThread.root = root;
    paramToThread.HEAP = &HEAP;
    InitializeCriticalSection(&paramToThread.section);    
    for (auto i = 0; i < countThreads; i++)    
    {        
        vectodHandle[i]=reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, FindWords, &paramToThread, 0, nullptr));
    }
    WaitForMultipleObjects(vectodHandle.size(), vectodHandle.data(), TRUE, INFINITE);   
    DeleteCriticalSection(&paramToThread.section);
    std::string cur_str;
    writeToHeap(root, cur_str, HEAP);    
    for (int i = 19; i >= 0; i--)
        std::cout << HEAP.m_heap_array[i].first << "\t" << HEAP.m_heap_array[i].second << std::endl;    
    freeNodes(root, true);
}


int main()
{
    PCWSTR PATH = L"D:\\big.txt";  
    
    int countThreads;
    std::cout << "Enter the number of threads: ";
    std::cin >> countThreads;
    if (countThreads > 40)
    {
        std::cout << "Too many threads" << std::endl;
        std::cout << "Enter number of threads between 1-40" << std::endl;
        return ERROR_INVALID_PARAMETER;
    }

    std::vector<HANDLE> threads(countThreads);    
    

    auto start = std::chrono::system_clock::now();
    CreateThreadsCrititcalSection(PATH, countThreads,threads);        
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time elapsed = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;
    return 0;
}


