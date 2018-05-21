#pragma once

/*
* weight of each method
* 基于位置特征的位置价值：position value
* 凝聚步：quiet move
* 稳定子：stable piece, 40步之前几乎不可能有稳定子， 终局搜索再去判断即可
* 行动力：ability of action
* 潜在行动力：potential
* 棋子数：piece count
* 奇偶性：parity，终局搜索的时候去算
* 角位：corner
* 近角位：nearby corner
* 边（不含角位）：edge
* 棋子边界：boundary
* 可落子处：valid
* 无子但不可落子处：invalid
* 空位置：valid+invalid
* 基本设计：棋盘10*10，称为abstract board，传入的合法坐标取值范围应当在(1,1)到(8,8)之间，可落子区域称为solid board
*/

enum Result { WHITE_WIN, BLACK_WIN, DRAW, UNFINISHED };
const std::string Result_String[] = { "White side win.", "Black side win.", "It's a draw." };

//position evaluate
const short W1 = 1;
//stable pieces count evaluate
const short W2 = 1;

//the hyperparameter of MCTS. the greater it is, the more confidental to the result
//也许在初盘、中盘、终局应当取不同的值
const double _C = 1.95;

//weight evaluation, based on experience, used for position evaluate
const short position_value[10][10] = {
	{ -8,-8,-8,-8,-8,-8,-8,-8,-8,-8 },
	{ -8,20,-3,11, 8, 8,11,-3,20,-8 },
	{ -8,-3,-7,-4, 1, 1,-4,-7,-3,-8 },
	{ -8,11,-4, 2, 2, 2, 2,-4,11,-8 },
	{ -8, 8, 1, 2,-3,-3, 2, 1, 8,-8 },
	{ -8, 8, 1, 2,-3,-3, 2, 1, 8,-8 },
	{ -8,11,-4, 2, 2, 2, 2,-4,11,-8 },
	{ -8,-3,-7,-4, 1, 1,-4,-7,-3,-8 },
	{ -8,20,-3,11, 8, 8,11,-3,20,-8 },
	{ -8,-8,-8,-8,-8,-8,-8,-8,-8,-8 }
};
