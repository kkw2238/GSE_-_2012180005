/*
	Copyright 2017 Lee Taek Hee (Korea Polytech University)
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the What The Hell License. Do it plz.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"

#include "SceneManager.h"
#include "Renderer.h"

SceneManager* g_objects = NULL;

bool g_LButtonDown = false;
bool g_RButtonDown = false;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	float ElpsedTime = 1.0f / 60.0f;
	// Initialize Objects
	
	// Renderer Test
	g_objects->Run();
	/*
		DrawSolidRect( x , y , z , float size , float r , g , b , a ) 
	*/

	glutSwapBuffers();
}

// Idle , RenderScene에서 업데이트를 하게 만든다.
void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	std::mt19937 engine((unsigned int)time(NULL));

	std::uniform_int_distribution<int> ui(-100, 100);
	std::uniform_real_distribution<float> uf(0.0f, 1.0f);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Vector3 vecPos = Vector3((float)x - 250.f, 250.f - (float)y, 0.0f);
		Vector3 vecDirection = Vector3(ui(engine), ui(engine), ui(engine));
		Vector4 vecColor = Vector4(uf(engine), uf(engine), uf(engine), uf(engine));
	
		g_objects->CreateNewObject(vecPos, OBJECT_CHARACTER);
	}
}

void KeyInput(unsigned char key, int x, int y)
{
	//RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	//RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer
	g_objects = new SceneManager();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop();

	delete g_objects;

    return 0;
}

