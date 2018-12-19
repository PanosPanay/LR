#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
using namespace std;
//以下参数可以更改以适应要求
const int CANDIDATE_NUM = 10;								//同一个产生式左部的候选式个数上限，按作业要求此处限定为5个
const int CANDIDATE_LENGTH = 10;							//每个候选式的长度上限（即字母个数，包括终结符和非终结符），此处设为10个字符
const int PRODUCTION_NUM = 20;								//文法产生式个数）上限，设为20个
const int NONTERMINAL_NUM = 20;								//非终结符个数上限，设为20个
const int TERMINAL_NUM = 20;								//终结符个数上限（包括$），设为20个
const int STATE_NUM = 50;									//LR分析表的状态数上限，设为50个
const int ITEM_NUM = 20;									//一个项目集的项目条数上限，设为20个
const int ITEM_LENGTH = 50;									//项目长度上限
const int INPUT_BUFFER_LENGTH = 50;							//输入缓冲区长度，设为50个
const int STACK_LENGTH = 50;								//栈的长度，设为50个

//产生式 (具有相同左部的产生式集合，eg.E->E+T|E-T|T)
class PRODUCTION {
public:
	char leftPart;											//产生式左部
	int candidateNum = 0;									//候选式数目
	char formula[CANDIDATE_NUM][CANDIDATE_LENGTH] = { 0 };	//存储候选式集合,\0作为每个候选式的结束标志
};

//非终结符类
class NONTERMINAL {
public:
	char nonTerminal;										//非终结符
	char FIRST[TERMINAL_NUM] = { 0 };						//FIRST集
	char FOLLOW[TERMINAL_NUM] = { 0 };						//FOLLOW集
	int FIRSTNum = 0;										//FIRST集元素个数
	int FOLLOWNum = 0;										//FOLLOW集元素个数
};

class ITEM {
public:
	//(0,0)表示用的是E'->E
	int productionOrder;									//用的是是第几个产生式
	int candidateOrder;										//用的是该产生式的第几个候选式
	string itemStr;											//项目内容
};

//文法
class GRAMMER {
public:
	PRODUCTION productionList[PRODUCTION_NUM];				//文法产生式集合
	NONTERMINAL nonTerminal[NONTERMINAL_NUM] = { 0 };		//非终结符集合
	char terminal[TERMINAL_NUM] = { 0 };					//终结符集合
	int nonTerminalNum = 0;									//非终结符数
	int terminalNum = 0;									//终结符数
	int productionNum = 0;									//不同左部的产生式数

	void Input_Grammer();									//从文本读入文法（注：自动将$加入非终结符表末）
	void Output_Grammer();									//输出文法到控制台
	void FIRST_Set();										//求FIRST集
	void FOLLOW_Set();										//求FOLLOW集
	void Output_First_Follow();								//输出FIRST集和FOLLOW集
	//增加对字符串求FIRST集的函数
	string First_of_Str(string s);							//求一个字符串的FIRST集
	int isTerminal(char C);									//终结符则返回在终结符表中的位置（0- ），非终结符则返回-1
	int isNonTerminal(char C);								//非终结符返回在非终结符表中的位置（0- ），终结符则返回-1
};

//LR预测分析程序的类
class LR {
public:
	GRAMMER G;												//文法
	char inputBufer[INPUT_BUFFER_LENGTH] = { 0 };			//输入缓冲区
	int forwardIp = 0;										//向前指针
	int stateNum = 0;										//状态数
	ITEM itemSetCollection[STATE_NUM][ITEM_NUM];			//LR分析的项目集规范族。DFA可以结合分析表得到.(\0作为一个项目集的结束标志)
	string actionTable[STATE_NUM][TERMINAL_NUM];			//LR分析表的action表
	string gotoTable[STATE_NUM][NONTERMINAL_NUM];			//LR分析的goto表

	LR();													//默认构造函数
	int CMP_ItemSet(string *itemSet1, string *itemSet2);	//比较2个项目集是否相同，相同返回1，不同返回0
	void Closure(string *itemSet);							//构造项目集的闭包
	void LR1_DFA();											//构造识别文法所有活前缀的LR(1) DFA,即构造LR(1)项目集规范族并构造LR(1)分析表。因为DFA各项目之间的关系即可在分析表中得到
	//void LR1_Analyze_Table();								//构造LR(1)分析表
	void LR1_Analyze();										//LR(1)分析程序//算法4.3
	void Input();											//将待分析的字符串w$放入输入缓冲区,并置向前指针指向w$的第一个符号
};
