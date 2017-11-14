#include "stdafx.h"
#include "SceneManager.h"

void ClearDeletedObject(std::list<std::shared_ptr<Object>>& objectlist ,ObjectType objtype)
{
	if (objtype == OBJECT_BULLET || objtype == OBJECT_ARROW)
		for (auto vectoriter = objectlist.begin(); vectoriter != objectlist.end(); vectoriter++)
			if ((*vectoriter).use_count() < 2) (*vectoriter).reset();

	//	������ ������Ʈ ����
	for (auto vectoriter = objectlist.begin(); vectoriter != objectlist.end();) {
		if (*vectoriter == nullptr)
			vectoriter = objectlist.erase(vectoriter);

		else if (vectoriter != objectlist.end())
			vectoriter++;
	}

}