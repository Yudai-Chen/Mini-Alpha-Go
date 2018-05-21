#pragma once

/*
* weight of each method
* ����λ��������λ�ü�ֵ��position value
* ���۲���quiet move
* �ȶ��ӣ�stable piece, 40��֮ǰ�������������ȶ��ӣ� �վ�������ȥ�жϼ���
* �ж�����ability of action
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

enum Result { WHITE_WIN, BLACK_WIN, DRAW, UNFINISHED };
const std::string Result_String[] = { "White side win.", "Black side win.", "It's a draw." };

//position evaluate
const short W1 = 1;
//stable pieces count evaluate
const short W2 = 1;

//the hyperparameter of MCTS. the greater it is, the more confidental to the result
//Ҳ���ڳ��̡����̡��վ�Ӧ��ȡ��ͬ��ֵ
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
