#pragma once
#include "pch.h"
#include "Resource.h"
#include "shape.h"
#include "editor.h"
#include <string>

class MyEditor : public Editor
{
private:
	/*MyEditor() {}
	MyEditor(const MyEditor&);
	MyEditor& operator=(MyEditor&);*/
	Shape** pcshape = new Shape * [114];
	HWND handler;
	static MyEditor* p_instance;
	MyEditor() {};
	MyEditor(const MyEditor&);
	MyEditor& operator=(MyEditor&);
public:
	/*static MyEditor& getInstance()
	{
		static MyEditor instance;
		return instance;
	}*/
	static MyEditor* getInstance();

	void Start(Shape*);
	void RemoveShape(int);
	void UpdateWindow();
	std::string GetDetails();
	void OnLMBDown(HWND);
	void OnLMBUp(HWND);
	void OnMouseMove(HWND);
	void OnPaint(HWND, int);
	void OnInitMenuPopup(HWND, WPARAM);
	~MyEditor();
	long x1, y1, x2, y2;
};