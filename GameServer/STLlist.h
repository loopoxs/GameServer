
#include <iostream>
#include <list>
#include <algorithm>  

#include <stdlib.h>
#include <string.h>


typedef struct info_s
{
	int nNumber;	//uid
	int posX, posY;
	unsigned short int port;

	bool operator==(struct info_s b) const
	{
		return this->nNumber == b.nNumber;
	}
	bool operator!=(struct info_s b) const
	{
		return this->nNumber != b.nNumber;
	}

	bool operator>=(struct info_s b) const
	{
		return this->nNumber >= b.nNumber;
	}

	bool operator<=(struct info_s b) const
	{
		return this->nNumber <= b.nNumber;
	}

	bool operator>(struct info_s b) const
	{
		return this->nNumber > b.nNumber;
	}

	bool operator<(struct info_s b) const
	{
		return this->posY < b.posY;
	}

}info_t;

typedef std::list< info_t > list_t;


class STLlist
{
public:

	void append(list_t & List, info_t & info);

	void for_each(list_t & List);

	void del_end_info(list_t & List);

	void for_each_delete(list_t & List);

	int insert_one(list_t & List, info_t & info, int iPlace);

	void delete_one(list_t & List, info_t info);

	void find_one(list_t & List, info_t info);

	void Sort(list_t & List);

private:

	

};

