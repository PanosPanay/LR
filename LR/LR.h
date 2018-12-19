#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
using namespace std;
//���²������Ը�������ӦҪ��
const int CANDIDATE_NUM = 10;								//ͬһ������ʽ�󲿵ĺ�ѡʽ�������ޣ�����ҵҪ��˴��޶�Ϊ5��
const int CANDIDATE_LENGTH = 10;							//ÿ����ѡʽ�ĳ������ޣ�����ĸ�����������ս���ͷ��ս�������˴���Ϊ10���ַ�
const int PRODUCTION_NUM = 20;								//�ķ�����ʽ���������ޣ���Ϊ20��
const int NONTERMINAL_NUM = 20;								//���ս���������ޣ���Ϊ20��
const int TERMINAL_NUM = 20;								//�ս���������ޣ�����$������Ϊ20��
const int STATE_NUM = 50;									//LR�������״̬�����ޣ���Ϊ50��
const int ITEM_NUM = 20;									//һ����Ŀ������Ŀ�������ޣ���Ϊ20��
const int ITEM_LENGTH = 50;									//��Ŀ��������
const int INPUT_BUFFER_LENGTH = 50;							//���뻺�������ȣ���Ϊ50��
const int STACK_LENGTH = 50;								//ջ�ĳ��ȣ���Ϊ50��

//����ʽ (������ͬ�󲿵Ĳ���ʽ���ϣ�eg.E->E+T|E-T|T)
class PRODUCTION {
public:
	char leftPart;											//����ʽ��
	int candidateNum = 0;									//��ѡʽ��Ŀ
	char formula[CANDIDATE_NUM][CANDIDATE_LENGTH] = { 0 };	//�洢��ѡʽ����,\0��Ϊÿ����ѡʽ�Ľ�����־
};

//���ս����
class NONTERMINAL {
public:
	char nonTerminal;										//���ս��
	char FIRST[TERMINAL_NUM] = { 0 };						//FIRST��
	char FOLLOW[TERMINAL_NUM] = { 0 };						//FOLLOW��
	int FIRSTNum = 0;										//FIRST��Ԫ�ظ���
	int FOLLOWNum = 0;										//FOLLOW��Ԫ�ظ���
};

class ITEM {
public:
	//(0,0)��ʾ�õ���E'->E
	int productionOrder;									//�õ����ǵڼ�������ʽ
	int candidateOrder;										//�õ��Ǹò���ʽ�ĵڼ�����ѡʽ
	string itemStr;											//��Ŀ����
};

//�ķ�
class GRAMMER {
public:
	PRODUCTION productionList[PRODUCTION_NUM];				//�ķ�����ʽ����
	NONTERMINAL nonTerminal[NONTERMINAL_NUM] = { 0 };		//���ս������
	char terminal[TERMINAL_NUM] = { 0 };					//�ս������
	int nonTerminalNum = 0;									//���ս����
	int terminalNum = 0;									//�ս����
	int productionNum = 0;									//��ͬ�󲿵Ĳ���ʽ��

	void Input_Grammer();									//���ı������ķ���ע���Զ���$������ս����ĩ��
	void Output_Grammer();									//����ķ�������̨
	void FIRST_Set();										//��FIRST��
	void FOLLOW_Set();										//��FOLLOW��
	void Output_First_Follow();								//���FIRST����FOLLOW��
	//���Ӷ��ַ�����FIRST���ĺ���
	string First_of_Str(string s);							//��һ���ַ�����FIRST��
	int isTerminal(char C);									//�ս���򷵻����ս�����е�λ�ã�0- �������ս���򷵻�-1
	int isNonTerminal(char C);								//���ս�������ڷ��ս�����е�λ�ã�0- �����ս���򷵻�-1
};

//LRԤ������������
class LR {
public:
	GRAMMER G;												//�ķ�
	char inputBufer[INPUT_BUFFER_LENGTH] = { 0 };			//���뻺����
	int forwardIp = 0;										//��ǰָ��
	int stateNum = 0;										//״̬��
	ITEM itemSetCollection[STATE_NUM][ITEM_NUM];			//LR��������Ŀ���淶�塣DFA���Խ�Ϸ�����õ�.(\0��Ϊһ����Ŀ���Ľ�����־)
	string actionTable[STATE_NUM][TERMINAL_NUM];			//LR�������action��
	string gotoTable[STATE_NUM][NONTERMINAL_NUM];			//LR������goto��

	LR();													//Ĭ�Ϲ��캯��
	int CMP_ItemSet(string *itemSet1, string *itemSet2);	//�Ƚ�2����Ŀ���Ƿ���ͬ����ͬ����1����ͬ����0
	void Closure(string *itemSet);							//������Ŀ���ıհ�
	void LR1_DFA();											//����ʶ���ķ����л�ǰ׺��LR(1) DFA,������LR(1)��Ŀ���淶�岢����LR(1)��������ΪDFA����Ŀ֮��Ĺ�ϵ�����ڷ������еõ�
	//void LR1_Analyze_Table();								//����LR(1)������
	void LR1_Analyze();										//LR(1)��������//�㷨4.3
	void Input();											//�����������ַ���w$�������뻺����,������ǰָ��ָ��w$�ĵ�һ������
};
