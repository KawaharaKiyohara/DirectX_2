// StructSample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>

int g_playerHp = 100;		//プレイヤーのHP
int g_playerMp = 200;		//プレイヤーのMP
int g_playerLv = 10;		//プレイヤーのレベル
int g_playerAtk = 20;		//プレイヤーの攻撃力。
int g_playerDef = 30;		//プレイヤーの防御力。

int main()
{
	std::cout << "プレイヤーのHP      " << g_playerHp << "\n";
	std::cout << "プレイヤーのMP      " << g_playerMp << "\n";
	std::cout << "プレイヤーのLv      " << g_playerLv << "\n";
	std::cout << "プレイヤーの攻撃力  " << g_playerAtk << "\n";
	std::cout << "プレイヤーの防御力  " << g_playerDef << "\n";
    return 0;
}

