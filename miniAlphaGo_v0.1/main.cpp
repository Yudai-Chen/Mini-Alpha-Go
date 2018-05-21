#include "board.h"

Board mainBoard;

int main()
{
	short x, y;
	while (1)
	{
		mainBoard.printBoard();
		std::cout << "turn: " << mainBoard.whosTurn() << std::endl;
		std::cout << "black: " << mainBoard.getPieceCount(true) << ", white: " << mainBoard.getPieceCount(false) << std::endl;
		mainBoard.printBoundary();
		std::cout << "your valid step :" << std::endl;
		mainBoard.printValid();
		while (1)
		{
			std::cin >> x >> y;
			if (!mainBoard.isCertainStepValid(Coord(x, y)))		//这一步有耗时，AI下棋的时候不需要进行
			{
				std::cout << "You cannot put piece at this position." << std::endl;
			}
			else
				break;
		}
		mainBoard.putPiece(Coord(x, y));
		Result res = mainBoard.isTerminal();
		if (res == UNFINISHED)
		{
			if (mainBoard.shouldPass())
			{
				std::cout << "You must pass." << std::endl;
				getchar();
				mainBoard.doPass();
			}
			continue;
		}
		std::cout << Result_String[res] << std::endl;
		std::cout << "black: " << mainBoard.getPieceCount(true) << ", white: " << mainBoard.getPieceCount(false) << std::endl;
		break;
	}
	return 0;
}