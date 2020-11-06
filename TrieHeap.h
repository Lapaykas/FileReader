#pragma once
#include <iostream>

using WordStat = std::pair<std::string, int>;

//Класс мин-кучи
class MinHeap
{
public:
	WordStat* m_heap_array;
	int m_heap_size;
	int m_current_size;

	MinHeap(const int size_of_heap) noexcept;
	~MinHeap();

	int GetMinimumValue() noexcept;

	void AddElementToHeap(const WordStat& element) noexcept;

	void Heapify(const int size,const int pos) noexcept;

	void Swap(WordStat* x, WordStat* y) noexcept;

	void HeapSort(const int size) noexcept;
};

//Структура узла префиксного дерева
struct node {
	node* next[26];     //Массив указателей на следующие вершины
						//next[i] - указатель на следующую вершину, соответствующую символу ('a' + i)
						//(предполагается, что используются только строчные латинские буквы)

	int weigth;        //Количество строк, заканчивающихся в этой вершине.
	bool endOfWord;

	node()
	{
		for (int i = 0; i < 26; i++)
		{
			next[i] = nullptr;
		}

		weigth = 0;
		endOfWord = false;
	}
};

void add(node* root, const std::string& s)  noexcept;

void writeToHeap(node* cur_node, std::string& cur_str, MinHeap& HEAP) noexcept;

void freeNodes(node* root,const bool isRoot) noexcept;