#include "stdafx.h"
#include "SceneManager.h"

void ClearDeletedObject(std::list<std::shared_ptr<Object>>& objectlist ,ObjectType objtype)
{
	////	������ ������Ʈ ����
	for (auto vectoriter = objectlist.begin(); vectoriter != objectlist.end();) {
		if (*vectoriter == nullptr)
			vectoriter = objectlist.erase(vectoriter);

		else if (vectoriter != objectlist.end())
			vectoriter++;
	}

}