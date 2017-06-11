#pragma once


//入力クラス。
class Input
{
public:
	Input();
	~Input();
	void OnMouseMove( int x, int y );
	void OnMouseLButtonClick();
	static Input& GetInstance()
	{
		static Input input;
		return input;
	}
	bool IsMouseMove() const
	{
		return isMouseMove;
	}
	const D3DXVECTOR2& GetMouseMove() const
	{
		return mouseMove;
	}
	//更新。
	void Update();
	//マウスの左クリックが押されたか判定。
	bool IsMouseLButtonClick() const
	{
		return isMouseLButtonClick;
	}
private:
	D3DXVECTOR2 mousePos;
	D3DXVECTOR2 mouseMove;
	D3DXVECTOR2 mousePosTmp;
	bool isMouseMoveFirst;
	bool isMouseMoveTmp;
	bool isMouseMove;
	bool isMouseLButtonClickTmp;
	bool isMouseLButtonClick;
};

static inline Input& GetInput()
{
	return Input::GetInstance();
}

