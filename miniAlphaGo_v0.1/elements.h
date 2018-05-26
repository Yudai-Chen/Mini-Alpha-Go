#pragma once
#include <string>
#include "omp.h"
/*
* weight of each method
* 基于位置特征的位置价值：position value
* 凝聚步：quiet move
* 稳定子：stable piece, 28步之前几乎不可能有稳定子
* 行动力：mobility
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

//#define PVE_MODE	

#define AIVE_MODE

#define USE_LIBRARY
#ifdef USE_LIBRARY
//#define OUTPUT_MATCH
#endif

//#define LIBRARY_READER_TEST

//#define PRINT_DETAIL_SCORES

#define SOLID_THINK_TIME	//固定思考时间
#ifdef SOLID_THINK_TIME
const int think_time = 10;
//#define OUTPUT_SEARCH_TIME
#endif // SOLID_THINK_TIME

//#define SOLID_SEARCH_TIME	//固定搜索次数
#ifdef SOLID_SEARCH_TIME
const int search_time = 100000;
#define OUTPUT_THINK_TIME
#define SEARCH_PARALLELLY
#endif // SOLID_SEARCH_TIME

enum Result { WHITE_WIN, BLACK_WIN, DRAW, UNFINISHED };
const std::string Result_String[] = { "White side win.", "Black side win.", "It's a draw." };

const int pool_size = 4000;

const int max_library_number = 35;	//最大可导入开局库的数量

//the hyperparameter of MCTS. the greater it is, the more confidental to the result
//在初盘、中盘、终局应当取不同的值
const double _C = 1.4;

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

const double win_above_10 = 1;
const double win_above_5 = 0.9;
const double win_below_5 = 0.8;
const double draw = 0.5;

//#define OUTPUT_DETAIL_SCORE

//#define DEBUG_MODE_CHOICE_REASON

//#define DEBUG_MODE_POSITION_VALUE_EVALUATE

//#define DEBUG_MODE

//#define DEBUG_MODE_BESTCHILD

//#define DEBUG_MODE_RANDOM

//#define DEBUG_PRINT_WHEN_NODE_100

//#define DEBUG_MODE_NULLCHILID

