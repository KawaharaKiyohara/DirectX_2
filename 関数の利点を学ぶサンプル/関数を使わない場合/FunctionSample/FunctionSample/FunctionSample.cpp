// FunctionSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

int main()
{
	//Aクラスの平均点を求める。
	int AclassScore[3];
	for (int i = 0; i < 3; i++) {
		std::cout << "Aクラスの成績を入力してください。";
		std::cin >> AclassScore[i] ;
		std::cout << "\n";
	}
	//平均を求める。
	int totalScore = 0;
	for (int i = 0; i < 3; i++) {
		totalScore += AclassScore[i];
	}
	std::cout << "Aクラスの平均点は" << totalScore / 3 << "です\n\n";


	//Bクラスの平均点を求める。
	totalScore = 0;
	int BClassscore[2];
	for (int i = 0; i < 2; i++) {
		std::cout << "Bクラスの成績を入力してください。";
		std::cin >> BClassscore[i];
		std::cout << "\n";
	}
	//平均を求める。
	totalScore = 0;
	for (int i = 0; i < 2; i++) {
		totalScore += BClassscore[i];
	}
	std::cout << "Bクラスの平均点は" << totalScore / 2 << "です\n\n";

	//Cクラスの平均点を求める。
	int CClassscore[3];
	for (int i = 0; i < 3; i++) {
		std::cout << "Cクラスの成績を入力してください。";
		std::cin >> CClassscore[i];
		std::cout << "\n";
	}
	//平均を求める。
	totalScore = 0;
	for (int i = 0; i <3; i++) {
		totalScore += CClassscore[i];
	}
	std::cout << "Cクラスの平均点は" << totalScore / 3 << "です\n\n";

	//Dクラスの平均点を求める。
	int DClassscore[4];
	for (int i = 0; i < 4; i++) {
		std::cout << "Dクラスの国語の成績を入力してください。";
		std::cin >> DClassscore[i];
		std::cout << "\n";
	}
	//平均を求める。
	totalScore = 0;
	for (int i = 0; i < 4; i++) {
		totalScore += DClassscore[i];
	}
	std::cout << "Dクラスの平均点は" << totalScore / 4 << "です\n\n";
    return 0;
}

