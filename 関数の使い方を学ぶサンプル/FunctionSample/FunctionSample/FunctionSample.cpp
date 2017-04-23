// FunctionSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

int main()
{
	int score[6];
	for (int i = 0; i < 6; i++) {
		std::cout << "‚ ‚È‚½‚Ì¬Ñ‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢B";
		std::cin >> score[i] ;
		std::cout << "\n";
	}
	//•½‹Ï‚ð‹‚ß‚éB
	int totalScore = 0;
	for (int i = 0; i < 6; i++) {
		totalScore += score[i];
	}
	std::cout << "•½‹Ï“_‚Í" << totalScore / 6 << "‚Å‚·\n";
    return 0;
}

