#include <iostream>
#include <stdlib.h>
#include <crtdbg.h>

#include "TrieHeap.h"


MinHeap::MinHeap(const int size_of_heap) noexcept : m_heap_size(size_of_heap), m_current_size(0)
{
	m_heap_array = new WordStat[size_of_heap];
	for (int i = 0; i < size_of_heap; i++)
		m_heap_array[i].second = 0;
}

MinHeap::~MinHeap()
{
	delete[] m_heap_array;
}

int MinHeap::GetMinimumValue() noexcept
{
	return m_heap_array[0].second;
}

void MinHeap::AddElementToHeap(const WordStat& element) noexcept
{	
	if (element.second < m_heap_array[0].second)
	{
		return;
	}
	else 
	{
		m_heap_array[0] = element;
	}
}

void MinHeap::Swap(WordStat* x, WordStat* y) noexcept
{	
	WordStat temp = *x;
	*x = *y;
	*y = temp;
}

void MinHeap::Heapify(const int size, const int pos) noexcept
{
	int index_minimum = pos; 
	int index_left = 2 * pos + 1; 
	int index_right = 2 * pos + 2;  


	if (index_left < size && m_heap_array[index_left].second > m_heap_array[index_minimum].second)
		index_minimum = index_left;


	if (index_right < size && m_heap_array[index_right].second > m_heap_array[index_minimum].second)
		index_minimum = index_right;


	if (index_minimum != pos)
	{
		Swap(&m_heap_array[pos], &m_heap_array[index_minimum]);

		Heapify(size, index_minimum);
	}
}

void MinHeap::HeapSort(const int size) noexcept
{// Build heap (rearrange array) 
	for (int i = size / 2 - 1; i >= 0; i--)
		Heapify( size, i);
	
	for (int i = size - 1; i > 0; i--)
	{		
		Swap(&m_heap_array[0], &m_heap_array[i]);		
		Heapify(i, 0);
	}

}

void add(node* root, const std::string& s)  noexcept
{
	node* cur_node = root;								//текущая вершина
	if (cur_node == nullptr)
		return;
	for (int i = 0; i < s.length(); i++) 
	{
		char c = tolower(s[i]);			
		if (cur_node->next[c - 'a'] == nullptr) 
		{
			cur_node->next[c - 'a'] = new node(); //создаем новый узел под корнем
			//std::cout <<"Word "<<c<<" "<<cur_node << std::endl;
		}
		cur_node = cur_node->next[c - 'a'];				//cur_v становится новой вершиной
	}

	cur_node->weigth++;
	cur_node->endOfWord = true;
}

void writeToHeap(node* cur_node, std::string& cur_str, MinHeap& HEAP) noexcept
{			
	WordStat elem_to_heap;
	if (cur_node == nullptr)
		return;
	if (cur_node->endOfWord)
	{
		// std::cout << cur_str << " Weigth " << cur_node->weigth << std::endl;
		elem_to_heap.first = cur_str;
		elem_to_heap.second = cur_node->weigth;
		HEAP.AddElementToHeap(elem_to_heap);
		HEAP.HeapSort(HEAP.m_heap_size);
	}
	


	for (int i = 0; i < 26; i++) {
		if (cur_node->next[i] != nullptr) {
			cur_str.push_back('a' + i);
			writeToHeap(cur_node->next[i],cur_str, HEAP);
			cur_str.pop_back();
		}
	}
}

void freeNodes(node* root,const bool isRoot) noexcept
{
	node* cur_node = root;
	if (cur_node == nullptr)
		return;
	for (int i = 0; i < 26; i++) 
	{
		if (cur_node->next[i] != nullptr) 
		{			
			freeNodes(cur_node->next[i],false);			
		} 
	}	
	if(!isRoot)
		delete cur_node;
}


