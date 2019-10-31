#pragma once
#include "stdafx.h"
#include "STLlist.h"

void STLlist::append(list_t &List, info_t &info)
{
	std::cout << "***append****" << std::endl;
	List.push_back(info);
}

void STLlist::for_each(list_t &List)
{
	std::cout << "***for_each****" << std::endl;
	list_t::iterator iter;
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		std::cout << iter->nNumber<<","<< iter ->posX<<","<< iter ->posY << std::endl;
	}
}

void STLlist::del_end_info(list_t &List)
{
	std::cout << "***del_end_info****" << std::endl;
	if (!List.empty())
	{
		List.pop_back();
	}
}

void STLlist::for_each_delete(list_t &List)
{
	list_t::iterator iter;
	for (iter = List.begin(); iter != List.end();)
	{
		std::cout << "delete before  iter->number:" << iter->nNumber << std::endl;
		iter = List.erase(iter);
		std::cout << "delete  after iter->number:" << iter->nNumber << std::endl;
	}

}


int STLlist::insert_one(list_t &List, info_t &info, int iPlace)
{
	int i = 0;

	std::cout << "insert_one" << std::endl;
	if (iPlace < 0)
	{
		std::cout << "insert_one  param error" << std::endl;
		return -1;
	}

	list_t::iterator iter = List.begin();

	while (iter != List.end())
	{
		//std::cout<<" dump "<< (*iVector)<<std::endl;  
		if (i == iPlace)
		{
			iter = List.insert(iter, info); //此时insert的返回值是迭代器，插入成功后iVector指向插入的位置  
			std::cout << " insert_one   after List point " << iter->nNumber << std::endl;
			return 0;
		}

		i++;
		++iter;
	}


	iter = List.insert(List.end(), info);

	return 0;
}



void STLlist::delete_one(list_t &List, info_t info)
{
	List.remove(info);
}
void STLlist::find_one(list_t &List, info_t info)
{

	std::cout << "find_one" << std::endl;

	list_t::iterator iter;

	iter = std::find(List.begin(), List.end(), info);

	if (iter != List.end())
	{
		std::cout << "find it" <<","<<info.posX<< std::endl;
	}
	else
	{
		std::cout << "not find it" << std::endl;
	}

}

void STLlist::Sort(list_t & List)
{
	std::cout << "Sort it" << std::endl;
	List.sort();
	for_each(List);
}
