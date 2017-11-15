#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <Windows.h>
#include <string>

#include <vector>
#include <memory>
#include <list>
#include "SceneManager.h"


#pragma comment(lib,"Winmm.lib")

#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

const int WIN_WIDTH  = 500;
const int WIN_HEIGHT = 800;

void ClearDeletedObject(std::list<std::shared_ptr<Object>>& objectlist, ObjectType objtype);