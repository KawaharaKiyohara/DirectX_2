// FunctionSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

//クラスの成績の入力と表示を行う関数。
//className クラス名
//numStudent 生徒の数。
void InputAndDispClassScore( const char* className, int numStudent)
{
	int classScore[256];
	for (int i = 0; i < numStudent; i++) {
		std::cout << className;
		std::cout << "の成績を入力してください。";
		std::cin >> classScore[i];
		std::cout << "\n";
	}
	//平均を求める。
	int totalScore = 0;
	for (int i = 0; i < numStudent; i++) {
		totalScore += classScore[i];
	}
	std::cout << className << "の平均点は" << totalScore / numStudent << "です\n\n";
}
int main()
{
	//Aクラスの成績入力と表示。
	InputAndDispClassScore("Aクラス", 3);

	//Bクラスの成績入力と表示。
	InputAndDispClassScore("Bクラス", 2);

	//Cクラスの平均点を求める。
	InputAndDispClassScore("Cクラス", 3);

	//Dクラスの平均点を求める。
	InputAndDispClassScore("Dクラス", 4);

    return 0;
}

