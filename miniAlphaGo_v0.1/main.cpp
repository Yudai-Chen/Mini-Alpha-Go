#include <WinSock2.h>  
#include <windows.h>
#include <ctime>

#include "MCTS.h"
#include "json/json.h"  

#pragma comment(lib, "WS2_32.lib")  
#pragma comment(lib, "json/json_vc71_libmtd.lib")  

Coord star[4] = { Coord(2, 2), Coord(7, 7), Coord(2, 7), Coord(7, 2) };
Coord corner[4] = { Coord(1, 1), Coord(8, 8), Coord(1, 8), Coord(8, 1) };

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

#ifdef PVE_MODE
int main()
{
	std::srand((unsigned)time(NULL));
	short x, y;
	Board mainBoard;
	while (1)
	{
		mainBoard.printBoard();
		std::cout << "turn: " << mainBoard.whosTurn() << std::endl;
		std::cout << "black: " << mainBoard.getPieceCount(true) << ", white: " << mainBoard.getPieceCount(false) << std::endl;
		std::cout << "your valid step :" << std::endl;
		mainBoard.printValid();
		if (mainBoard.whosTurn())
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

	while (1)
	{		
		Board mainBoard;
		memset(rec, 0, 100);
		if (connect(ReceiveSocket, (struct sockaddr *)&SendAddr, sizeof(SendAddr)) == -1)
			continue;
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
				MCTS miniAlphaGo(mainBoard, mainBoard.whosTurn(), _C);
				Coord move = miniAlphaGo.search();			
				mainBoard.putPiece(move);
				localtoServer(move);
				send_value["x"] = move.first;
				send_value["y"] = move.second;
				SendBuf = style_write.write(send_value);
				while (send(ReceiveSocket, SendBuf.c_str(), SendBuf.size(), 0) == -1);
				miniAlphaGo.release();
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
			}
			res = mainBoard.isTerminal();
			if (res < UNFINISHED)
			{
				std::cout << Result_String[res] << std::endl;
				std::cout << "black: " << mainBoard.getPieceCount(true) << ", white: " << mainBoard.getPieceCount(false) << std::endl;
				break;
			}
		}

	}
}
#endif