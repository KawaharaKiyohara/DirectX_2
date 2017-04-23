#include "stdafx.h"
#include "Input.h"


Input::Input()
{
	mousePos.x = 0.0f;
	mousePos.y = 0.0f;
	mouseMove.x = 0.0f;
	mouseMove.y = 0.0f;
	mousePosTmp.x = 0.0f;
	mousePosTmp.y = 0.0f;
	isMouseMoveFirst = true;
	isMouseMove = false;
	isMouseMoveTmp = false;
	isMouseLButtonClickTmp = false;
	isMouseLButtonClick = false;
}


Input::~Input()
{
}

void Input::OnMouseMove(int x, int y)
{
	mousePosTmp.x = x;
	mousePosTmp.y = y;
	if (!isMouseMoveFirst) {
		isMouseMoveTmp = true;
	}
	
	isMouseMoveFirst = false;
}
void Input::OnMouseLButtonClick()
{
	isMouseLButtonClickTmp = true;
}
//çXêVÅB
void Input::Update()
{
	if (isMouseMove && !isMouseMoveTmp) {
		RECT rc;
		if (GetActiveWindow() == g_hwnd) {
			GetClientRect(g_hwnd, &rc);
			mousePos.x = rc.right / 2.0f;
			mousePos.y = rc.bottom / 2.0f;
			SetCursorPos(mousePos.x, mousePos.y);
			isMouseMoveFirst = true;
		}
	}
	mouseMove.x = 0.0f;
	mouseMove.y = 0.0f;
	isMouseMove = false;
	isMouseLButtonClick = isMouseLButtonClickTmp;
	isMouseLButtonClickTmp = false;
	if (isMouseMoveTmp) {
		mouseMove = mousePosTmp - mousePos;
		mousePos = mousePosTmp;
		isMouseMove = isMouseMoveTmp;
		isMouseMoveTmp = false;
	}
}