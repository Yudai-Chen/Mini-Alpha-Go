#pragma once
#include <string>
#include "omp.h"
/*
* weight of each method
* ����λ��������λ�ü�ֵ��position value
* ���۲���quiet move
* �ȶ��ӣ�stable piece, 28��֮ǰ�������������ȶ���
* �ж�����mobility
* Ǳ���ж�����potential
* ��������piece count
* ��ż�ԣ�parity���վ�������ʱ��ȥ��
* ��λ��corner
* ����λ��nearby corner
* �ߣ�������λ����edge
* ���ӱ߽磺boundary
* �����Ӵ���valid
* ���ӵ��������Ӵ���invalid
* ��λ�ã�valid+invalid
* ������ƣ�����10*10����Ϊabstract board������ĺϷ�����ȡֵ��ΧӦ����(1,1)��(8,8)֮�䣬�����������Ϊsolid board
*/

//#define PVE_MODE	

#define AIVE_MODE

#define USE_LIBRARY
#ifdef USE_LIBRARY
//#define OUTPUT_MATCH
#endif

//#define LIBRARY_READER_TEST

//#define PRINT_DETAIL_SCORES

#define SOLID_THINK_TIME	//�̶�˼��ʱ��
#ifdef SOLID_THINK_TIME
const int think_time = 10;
//#define OUTPUT_SEARCH_TIME
#endif // SOLID_THINK_TIME

//#define SOLID_SEARCH_TIME	//�̶���������
#ifdef SOLID_SEARCH_TIME
const int search_time = 100000;
#define OUTPUT_THINK_TIME
#define SEARCH_PARALLELLY
#endif // SOLID_SEARCH_TIME

enum Result { WHITE_WIN, BLACK_WIN, DRAW, UNFINISHED };
const std::string Result_String[] = { "White side win.", "Black side win.", "It's a draw." };

const int pool_size = 4000;

const int max_library_number = 35;	//���ɵ��뿪�ֿ������

//the hyperparameter of MCTS. the greater it is, the more confidental to the result
//�ڳ��̡����̡��վ�Ӧ��ȡ��ͬ��ֵ
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

