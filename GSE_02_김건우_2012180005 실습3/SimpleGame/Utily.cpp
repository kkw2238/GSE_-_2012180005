#include <Windows.h>
#include "Utily.h"
#include "stdafx.h"
#include "Vector3.h"

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << "Vector3 : " << v.GetX() << " , " << v.GetY() << " , " << v.GetZ() << std::endl;
	return os;
}
