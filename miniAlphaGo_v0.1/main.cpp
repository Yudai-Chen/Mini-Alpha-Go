#include <WinSock2.h>  
#include <windows.h>
#include <ctime>

#include "MCTS.h"
#include "library_reader_factory.h"
#include "library_solver.h"
#include "json/json.h"  

#pragma comment(lib, "WS2_32.lib")  
#pragma comment(lib, "json/json_vc71_libmtd.lib")  

#ifdef DEBUG_MODE_POSITION_VALUE_EVALUATE
extern double maxQuality, minQuality;
#endif // DEBUG_MODE_POSITION_VALUE_EVALUATE

Coord star[4] = { Coord(2, 2), Coord(7, 7), Coord(2, 7), Coord(7, 2) };
Coord corner[4] = { Coord(1, 1), Coord(8, 8), Coord(1, 8), Coord(8, 1) };
Library_reader_factory libReaderFactory;
Library_solver libSolver;
bool isMatch;

//server和本地对坐标的定义方式不同，需要进行改动
void localtoServer(Coord &coord)
{
	short x = coord.first;
	short y = coord.second;

	coord.first = y - 1;
	coord.second = x - 1;
}
void servertoLocal(Coord &coord)
{
	short x = coord.first;
	short y = coord.second;

	coord.first = y + 1;
	coord.second = x + 1;
}

void loadLibraries()
{
	libReaderFactory.initLibrary("library/WTH_2016.wtb");
	libReaderFactory.initLibrary("library/WTH_2015.wtb");
	libReaderFactory.initLibrary("library/WTH_2014.wtb");
	libReaderFactory.initLibrary("library/WTH_2013.wtb");
	libReaderFactory.initLibrary("library/WTH_2012.wtb");
	libReaderFactory.initLibrary("library/WTH_2011.wtb");
	libReaderFactory.initLibrary("library/WTH_2010.wtb");
	libReaderFactory.initLibrary("library/WTH_2009.wtb");
	libReaderFactory.initLibrary("library/WTH_2008.wtb");
	libReaderFactory.initLibrary("library/WTH_2007.wtb");
	libReaderFactory.initLibrary("library/WTH_2006.wtb");
	libReaderFactory.initLibrary("library/WTH_2005.wtb");
	libReaderFactory.initLibrary("library/WTH_2004.wtb");
	libReaderFactory.initLibrary("library/WTH_2003.wtb");
	libReaderFactory.initLibrary("library/WTH_2002.wtb");
	libReaderFactory.initLibrary("library/WTH_2001.wtb");
	libReaderFactory.initLibrary("library/WTH_2000.wtb");
	libReaderFactory.initLibrary("library/WTH_1999.wtb");
	libReaderFactory.initLibrary("library/WTH_1998.wtb");
	libReaderFactory.initLibrary("library/WTH_1997.wtb");
	libReaderFactory.initLibrary("library/WTH_1996.wtb");
	libReaderFactory.initLibrary("library/WTH_1995.wtb");
	libReaderFactory.initLibrary("library/WTH_1994.wtb");
	libReaderFactory.initLibrary("library/WTH_1993.wtb");
	libReaderFactory.initLibrary("library/WTH_1992.wtb");
	libReaderFactory.initLibrary("library/WTH_1991.wtb");
	libReaderFactory.initLibrary("library/WTH_1990.wtb");
	libReaderFactory.initLibrary("library/WTH_1989.wtb");
	libReaderFactory.initLibrary("library/WTH_1988.wtb");
	libSolver.buildLibraryTree(libReaderFactory);
	libReaderFactory.release();
	isMatch = true;
}

#ifdef PVE_MODE
int main()
{
	std::srand((unsigned)time(NULL));
	short x, y;
#ifdef USE_LIBRARY
	loadLibraries();
#endif
	Board mainBoard;
	while (1)
	{
		mainBoard.printBoard();
		std::cout << "turn: " << mainBoard.whosTurn() << std::endl;
		std::cout << "black: " << mainBoard.getPieceCount(true) << ", white: " << mainBoard.getPieceCount(false) << std::endl;
		std::cout << "your valid step :" << std::endl;
		mainBoard.printValid();
		if (!mainBoard.whosTurn())
		{
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
		}
		else
		{
			MCTS miniAlphaGo(mainBoard, mainBoard.whosTurn(), _C);
			Coord move = miniAlphaGo.search();
			miniAlphaGo.release();
			std::cout << "miniAlphaGo: (" << move.first << ", " << move.second << ")" << std::endl;
			mainBoard.putPiece(move);
		}
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
#ifdef USE_LIBRARY
	libSolver.releaseLibraryTree();
#endif
	return 0;	
}
#endif

#ifdef AIVE_MODE
int main()
{
	std::srand((unsigned)time(NULL));
	Json::Value send_value, recv_value;
	Json::StyledWriter style_write;
	Json::Reader reader;

	char rec[100];
	std::string SendBuf;

	Result res;

	WSADATA wsaData;
	SOCKET SendSocket, ReceiveSocket;
	sockaddr_in SendAddr, RecvAddr;
	int Port = 6000;

	//初始化Socket  
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建Socket对象  
	SendSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//设置服务器地址  
	RecvAddr.sin_family = PF_INET;
	RecvAddr.sin_port = htons(4567);
	RecvAddr.sin_addr.s_addr = inet_addr("192.168.18.1");	//18.1

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建Socket对象  
	ReceiveSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//设置服务器地址  
	SendAddr.sin_family = PF_INET;
	SendAddr.sin_port = htons(6000);
	SendAddr.sin_addr.s_addr = inet_addr("192.168.18.1");
	bind(ReceiveSocket, (struct sockaddr *)&RecvAddr, sizeof(RecvAddr));


	Board mainBoard;
#ifdef USE_LIBRARY
	loadLibraries();
#endif
	memset(rec, 0, 100);
	while (connect(ReceiveSocket, (struct sockaddr *)&SendAddr, sizeof(SendAddr)) == -1);

	recv(ReceiveSocket, rec, 100, 0);

	reader.parse(rec, recv_value);
	if (recv_value["Black"].asInt() == 0)
	{
		memset(rec, 0, 100);
		recv(ReceiveSocket, rec, 100, 0);
		reader.parse(rec, recv_value);
		Coord move = Coord(recv_value["x"].asInt(), recv_value["y"].asInt());
		servertoLocal(move);
		mainBoard.putPiece(move);
#ifdef USE_LIBRARY
		libSolver.changeState(move, -1);
#endif
	}
	while (1)
	{
		if (mainBoard.shouldPass())
		{
			mainBoard.doPass();
			send_value["x"] = -1;
			send_value["y"] = -1;
			SendBuf = style_write.write(send_value);
			while (send(ReceiveSocket, SendBuf.c_str(), SendBuf.size(), 0) == -1);
		}
		else
		{
#ifdef USE_LIBRARY
			Coord move;
			if (isMatch)
			{
				move = libSolver.getBestMoveAndChangeState();
				if (move == Coord(-1, -1))
				{
					isMatch = false;
				}
				else
				{
#ifdef OUTPUT_MATCH
					std::cout << "match!" << std::endl;
#endif
					mainBoard.putPiece(move);
					localtoServer(move);
					send_value["x"] = move.first;
					send_value["y"] = move.second;
					SendBuf = style_write.write(send_value);
					while (send(ReceiveSocket, SendBuf.c_str(), SendBuf.size(), 0) == -1);
				}
			}
			if (!isMatch)
			{
				MCTS miniAlphaGo(mainBoard, mainBoard.whosTurn(), _C);
				move = miniAlphaGo.search();
				mainBoard.putPiece(move);
				localtoServer(move);
				send_value["x"] = move.first;
				send_value["y"] = move.second;
				SendBuf = style_write.write(send_value);
				while (send(ReceiveSocket, SendBuf.c_str(), SendBuf.size(), 0) == -1);
				miniAlphaGo.release();
			}			
#else
			MCTS miniAlphaGo(mainBoard, mainBoard.whosTurn(), _C);
			Coord move = miniAlphaGo.search();
			mainBoard.putPiece(move);
			localtoServer(move);
			send_value["x"] = move.first;
			send_value["y"] = move.second;
			SendBuf = style_write.write(send_value);
			while (send(ReceiveSocket, SendBuf.c_str(), SendBuf.size(), 0) == -1);
			miniAlphaGo.release();
#endif
		}
		res = mainBoard.isTerminal();
		if (res < UNFINISHED)
		{
			std::cout << Result_String[res] << std::endl;
			std::cout << "black: " << mainBoard.getPieceCount(true) << ", white: " << mainBoard.getPieceCount(false) << std::endl;
			break;
		}
		recv(ReceiveSocket, rec, 100, 0);
		reader.parse(rec, recv_value);
		if (recv_value["x"] == -1 && recv_value["y"] == -1)
		{
			mainBoard.doPass();
		}
		else
		{
			Coord move = Coord(recv_value["x"].asInt(), recv_value["y"].asInt());
			servertoLocal(move);
			mainBoard.putPiece(move);
#ifdef USE_LIBRARY
			if (isMatch)
			{
				if (libSolver.changeState(move, -1) != 0)
				{
					isMatch = false;
				}
#ifdef OUTPUT_MATCH
				else
					std::cout << "match!" << std::endl;
#endif
			}
#endif
		}
		res = mainBoard.isTerminal();
		if (res < UNFINISHED)
		{
			std::cout << Result_String[res] << std::endl;
			std::cout << "black: " << mainBoard.getPieceCount(true) << ", white: " << mainBoard.getPieceCount(false) << std::endl;
			break;
		}
	}
#ifdef USE_LIBRARY
	libSolver.releaseLibraryTree();
#endif
	return 0;
}
#endif

/*int main()
{

}	*/