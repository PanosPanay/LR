#include "LR.h"

//文法
//------------------------------------------------------------------------------------------------
//从文件读入文法（注：自动将$加入非终结符表末）
void GRAMMER::Input_Grammer()
{
	cout << "题目给定文法存储于G.txt." << endl;
	cout << "请输入文法所在文件名（eg: G.txt)：";
	string fileName;
	cin >> fileName;
	ifstream GFile;
	GFile.open(fileName);
	GFile >> noskipws;//不跳过空格和回车
	char C;//读入的字符
	int state = 0;
	int errorFlag = 0;
	int theProduction = 0;//指第几个产生式
	int theCandidate = 0;//指第几个候选式
	int candidatePos = 0;//指候选式中的位置
	for (; GFile.peek() != EOF && errorFlag == 0;)
	{
		//GFile >> C;//读入字符
		//cout << C << endl;///////////////////////

		switch (state)
		{
		case 0://初始，等待读入产生式左部
			GFile >> C;
			if (C == ' ' || C == '\t')//跳过空格和制表符
				state = 0;
			else if (C >= 'A' && C <= 'Z')//产生式左部应为大写字母
			{
				state = 1;
				int flag = 0;
				int i;
				for (i = 0; i < productionNum && flag == 0; ++i)
				{
					if (C == productionList[i].leftPart)//左部已存在
					{
						flag = 1;
					}
				}
				if (flag == 1)//说明左部已存在
				{
					theProduction = i - 1;
				}
				else//左部不存在
				{
					theProduction = productionNum;
					productionList[theProduction].leftPart = C;
					++productionNum;//不同左部的产生式数目+1

					//加入非终结符表
					nonTerminal[nonTerminalNum].nonTerminal = C;
					++nonTerminalNum;
				}
			}
			else
			{
				errorFlag = 1;
				cout << "产生式左部为非终结符，应该是一个大写字母" << endl;
			}
			break;
		case 1://输入-  （->）
			GFile >> C;
			if (C == ' ' || C == '\t')
				state = 1;
			else if (C == '-')
				state = 2;
			else
			{
				errorFlag = 1;
				cout << "文法中'->'之间不能有空格或其他字符" << endl;
			}
			break;
		case 2://输入了->
			GFile >> C;
			if (C == '>')
			{
				state = 3;
			}
			else
			{
				errorFlag = 1;
				cout << "文法中'->'之间不能有空格或其他字符" << endl;
			}
			break;
		case 3:
			GFile >> C;

			if (C == ' ' || C == '\t')
				state = 3;
			else if (C != '\n' && C != '|')//读入候选式第一个字符
			{
				state = 4;
				theCandidate = productionList[theProduction].candidateNum;
				candidatePos = 0;//输入该候选式的第一个字符
				productionList[theProduction].formula[theCandidate][candidatePos] = C;
				++(productionList[theProduction].candidateNum);//该产生式的候选式数目+1

				//如果读入的是终结符，看是否已经出现在终结符表，没有的话就加入
				if ((C > 'Z' || C < 'A') && C != '~')//即不是非终结符
				{
					int i;
					int tFlag = 0;
					for (i = 0; i < terminalNum && tFlag == 0; ++i)
					{
						if (C == terminal[i])//已存在
						{
							tFlag = 1;
						}
					}
					if (tFlag == 0)//不存在
					{
						terminal[terminalNum] = C;
						++terminalNum;
					}
				}
			}
			else
			{
				errorFlag = 1;
				cout << "请输入正确的候选式" << endl;
			}
			break;
		case 4:
			GFile >> C;

			if (C == ' ' || C == '\t')//空格
			{
				state = 4;//忽略空格
			}
			else if (C == '|')
			{
				state = 3;//开始接收下一个候选式
				++candidatePos;
				productionList[theProduction].formula[theCandidate][candidatePos] = '\0';
			}
			else if (C == '\n')
			{
				state = 0;//输入下一组产生式
				++candidatePos;
				productionList[theProduction].formula[theCandidate][candidatePos] = '\0';
			}
			else//接收其他字符作为产生式右部
			{
				++candidatePos;//字符在候选式中的位置+1
				productionList[theProduction].formula[theCandidate][candidatePos] = C;

				//如果读入的是终结符，看是否已经出现在终结符表，没有的话就加入
				if ((C > 'Z' || C < 'A') && C != '~')//即不是非终结符,且不为空‘~’
				{
					int i;
					int tFlag = 0;
					for (i = 0; i < terminalNum && tFlag == 0; i++)
					{
						if (C == terminal[i])//已存在
						{
							tFlag = 1;
						}
					}
					if (tFlag == 0)//不存在
					{
						terminal[terminalNum] = C;
						terminalNum++;
					}
				}
			}
			break;
		default:break;
		}
	}
	GFile.close();
	//在非终结符表末尾存入$
	terminal[terminalNum] = '$';
	++terminalNum;
}

//输出文法
void GRAMMER::Output_Grammer()
{
	//cout << endl << "文法如下：" << endl;
	for (int i = 0; i < productionNum; ++i)
	{
		cout << productionList[i].leftPart << " -> ";
		for (int j = 0; j < productionList[i].candidateNum; ++j)
		{
			if (j > 0)
				cout << " | ";
			for (int k = 0; productionList[i].formula[j][k] != '\0'; ++k)
			{
				cout << productionList[i].formula[j][k];
			}
		}
		cout << endl;
	}
}

//求FIRST集
void GRAMMER::FIRST_Set()
{
	int changeFlag = 1;//有改动，FIRST集还没算完，可以继续循环
	while (changeFlag == 1)
	{
		changeFlag = 0;
		for (int i = 0; i < nonTerminalNum; ++i)//为每一个非终结符求（扩充）FOLLOW集
		{
			for (int j = 0; j < productionList[i].candidateNum; ++j)//遍历该非终结符的每一个候选式
			{
				int lastBlank = 1;//只有上一个字符能推出空，才需要继续遍历看下一个字符的FIRST集
				for (int k = 0; productionList[i].formula[j][k] != '\0' && lastBlank == 1; ++k)//求此候选式的first集加入该非终结符的first集
				{
					lastBlank = 0;
					int tPos = isTerminal(productionList[i].formula[j][k]);//若为终结符，返回在终结符表中的位置
					int nPos = isNonTerminal(productionList[i].formula[j][k]);//若为非终结符，返回在非终结符表中的位置
					if (tPos >= 0 || productionList[i].formula[j][k] == '~')//是终结符或~，直接加入first集
					{
						//if(productionList[i].formula[j][k]!='$')//不需要判断，产生式不会出现$
						int fillPos = nonTerminal[i].FIRSTNum;
						int existFlag = 0;
						for (int l = 0; l < fillPos && existFlag == 0; ++l)
						{
							if (nonTerminal[i].FIRST[l] == productionList[i].formula[j][k])
							{
								existFlag = 1;
							}
						}
						if (existFlag == 0)//原本first集不含有该终结符或~,加入
						{
							nonTerminal[i].FIRST[fillPos] = productionList[i].formula[j][k];
							++(nonTerminal[i].FIRSTNum);
							changeFlag = 1;//有改动
						}
						//lastBlank = 0;//不用再看后面的字符了
					}
					else if (nPos >= 0)//是非终结符
					{
						for (int l = 0; l < nonTerminal[nPos].FIRSTNum; ++l)//遍历该非终结符的每一个first集元素，看是否已存在，不存在就加入
						{
							int fillPos = nonTerminal[i].FIRSTNum;
							if (nonTerminal[nPos].FIRST[l] != '~')
							{
								int existFlag = 0;
								for (int m = 0; m < fillPos&&existFlag == 0; ++m)
								{
									if (nonTerminal[nPos].FIRST[l] == nonTerminal[i].FIRST[m])
									{
										existFlag = 1;
									}
								}
								if (existFlag == 0)//原本first集不含有该终结符,加入
								{
									nonTerminal[i].FIRST[fillPos] = nonTerminal[nPos].FIRST[l];
									++(nonTerminal[i].FIRSTNum);
									changeFlag = 1;//有改动
								}
							}
							else//如果是~
							{
								if (productionList[i].formula[j][k + 1] == '\0')//是候选式最后一个符号能推出空，即能推出空
								{
									int existFlag = 0;
									for (int m = 0; m < fillPos&&existFlag == 0; ++m)
									{
										if (nonTerminal[i].FIRST[m] == '~')
										{
											existFlag = 1;
										}
									}
									if (existFlag == 0)//原本first集不含有~,加入
									{
										nonTerminal[i].FIRST[fillPos] = '~';
										++(nonTerminal[i].FIRSTNum);
										changeFlag = 1;//有改动
									}
								}
								else//可以继续看后面的字符
								{
									lastBlank = 1;
								}
							}
						}
					}
				}
			}
		}
	}
}

//求FOLLOW集
void GRAMMER::FOLLOW_Set()
{
	//对文法开始符号（默认为第一个产生式左部），置$于其FOLLOW集
	nonTerminal[0].FOLLOW[0] = '$';
	++(nonTerminal[0].FOLLOWNum);
	int changeFlag = 1;//有改动，说明FOLLOW集还没求完，继续循环使用下面规则，直到每个集合不再增大（即changeFlag==0）为止
	while (changeFlag == 1)
	{
		changeFlag = 0;
		//i、j两个循环遍历每一个候选式
		for (int i = 0; i < productionNum; ++i)
		{
			for (int j = 0; j < productionList[i].candidateNum; ++j)
			{
				//遍历候选式的每个字符，找非终结符，运用规则，扩充FOLLOW集
				for (int k = 0; productionList[i].formula[j][k] != '\0'; ++k)
				{
					int nPos = isNonTerminal(productionList[i].formula[j][k]);//返回在非终结符表中的位置
					if (nPos >= 0)//如果是非终结符
					{
						if (productionList[i].formula[j][k + 1] == '\0')//即A->aB 的形式，则把FOLLOW(A)中的所有元素加入到FOLLOW（B）中
						{
							for (int l = 0; l < nonTerminal[i].FOLLOWNum; ++l)//遍历FOLLOW(A)的所有FOLLOW集元素，与FOLLOW（B)中元素逐一比较，若不存在，就加入
							{
								int existFlag = 0;
								for (int m = 0; m < nonTerminal[nPos].FOLLOWNum && existFlag == 0; ++m)
								{
									if (nonTerminal[nPos].FOLLOW[m] == nonTerminal[i].FOLLOW[l])
									{
										existFlag = 1;
									}
								}
								if (existFlag == 0)//FOLLOW（B)不含该元素，则加入
								{
									int fillPos = nonTerminal[nPos].FOLLOWNum;
									nonTerminal[nPos].FOLLOW[fillPos] = nonTerminal[i].FOLLOW[l];
									++(nonTerminal[nPos].FOLLOWNum);
									changeFlag = 1;
								}
							}
						}
						else //A->aBw
						{
							//将w的first集的非空元素加入FOLLOW B，若w->~，则将A的Follow集元素加入FOLLOW B
							int lastBlank = 1;//只有上一个能推出空，才要看下一个元素的first集
							for (int l = k + 1; productionList[i].formula[j][l] != '\0' && lastBlank == 1; ++l)
							{
								lastBlank = 0;
								//if(productionList[i].formula[j][l]=='~')//不可能出现A->aB~
								int tPos = isTerminal(productionList[i].formula[j][l]);//返回在终结符表中的位置
								if (tPos >= 0)//是终结符，即A->aBcw的形式，c加入FOLLOW B
								{
									int existFlag = 0;
									for (int m = 0; m < nonTerminal[nPos].FOLLOWNum && existFlag == 0; ++m)
									{
										if (nonTerminal[nPos].FOLLOW[m] == terminal[tPos])
										{
											existFlag = 1;
										}
									}
									if (existFlag == 0)//FOLLOW（B)不含该元素，则加入
									{
										int fillPos = nonTerminal[nPos].FOLLOWNum;
										nonTerminal[nPos].FOLLOW[fillPos] = terminal[tPos];
										++(nonTerminal[nPos].FOLLOWNum);
										changeFlag = 1;
									}
								}
								else//是非终结符，即A->aBCw的情况,将C的first集的非空元素加入FOLLOW B，若C->~，继续看下一个字母
								{
									int CPos = isNonTerminal(productionList[i].formula[j][l]);
									for (int m = 0; m < nonTerminal[CPos].FIRSTNum; ++m)
									{
										if (nonTerminal[CPos].FIRST[m] != '~')
										{
											int existFlag = 0;
											for (int n = 0; n < nonTerminal[nPos].FOLLOWNum && existFlag == 0; ++n)
											{
												if (nonTerminal[nPos].FOLLOW[n] == nonTerminal[CPos].FIRST[m])
												{
													existFlag = 1;
												}
											}
											if (existFlag == 0)//FOLLOW（B)不含该元素，则加入
											{
												int fillPos = nonTerminal[nPos].FOLLOWNum;
												nonTerminal[nPos].FOLLOW[fillPos] = nonTerminal[CPos].FIRST[m];
												++(nonTerminal[nPos].FOLLOWNum);
												changeFlag = 1;
											}
										}
										else//C的first集含~，如果C是该候选式最后一个字符，则把follow A元素加入follow B；否则继续可能w
										{
											if (productionList[i].formula[j][l + 1] == '\0')//如果C是该候选式最后一个字符，则把follow A元素加入follow B
											{
												for (int n = 0; n < nonTerminal[i].FOLLOWNum; ++n)//遍历FOLLOW(A)的所有FOLLOW集元素，与FOLLOW（B)中元素逐一比较，若不存在，就加入
												{
													int existFlag = 0;
													for (int o = 0; o < nonTerminal[nPos].FOLLOWNum && existFlag == 0; ++o)
													{
														if (nonTerminal[nPos].FOLLOW[o] == nonTerminal[i].FOLLOW[n])
														{
															existFlag = 1;
														}
													}
													if (existFlag == 0)//FOLLOW（B)不含该元素，则加入
													{
														int fillPos = nonTerminal[nPos].FOLLOWNum;
														nonTerminal[nPos].FOLLOW[fillPos] = nonTerminal[i].FOLLOW[n];
														++(nonTerminal[nPos].FOLLOWNum);
														changeFlag = 1;
													}
												}
											}
											else//继续看后面的字符
											{
												lastBlank = 1;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//输出FIRST集和FOLLOW集
void GRAMMER::Output_First_Follow()//输出FIRST集和FOLLOW集
{
	cout << endl << "FIRST集和FOLLOW集如下表： " << endl;
	cout << "-----------------------------------------------" << endl;
	//cout << "\tFIRST\t\t\tFOLLOW" << endl;
	cout << "\tFIRST";
	for (int i = 5; i < 20; ++i)
		cout << " ";
	cout << "FOLLOW" << endl;

	cout << "-----------------------------------------------" << endl;
	for (int i = 0; i < nonTerminalNum; ++i)
	{
		cout << nonTerminal[i].nonTerminal << "\t";
		int j;
		for (j = 0; j < nonTerminal[i].FIRSTNum; ++j)
		{
			cout << nonTerminal[i].FIRST[j] << " ";
		}
		for (j = 2 * j; j < 20; ++j)
			cout << " ";
		//cout << "\t\t\t";
		for (int j = 0; j < nonTerminal[i].FOLLOWNum; ++j)
		{
			cout << nonTerminal[i].FOLLOW[j] << " ";
		}
		cout << endl;
		cout << "-----------------------------------------------" << endl;
	}
}

//求一个字符串的FIRST集
string GRAMMER::First_of_Str(string s)
{
	//未完成！
	//末尾加'\0'
	return string();
}

//判定是否是终结符，终结符则返回在终结符表中的位置（0- ），非终结符则返回-1
int GRAMMER::isTerminal(char C)
{
	int i;
	int flag = 0;
	for (i = 0; i < terminalNum && flag == 0; ++i)
	{
		if (C == terminal[i])
		{
			flag = 1;
		}
	}
	if (flag == 1)
		return i - 1;//终结符
	else
		return -1;//非终结符
}

//判定是否是非终结符，非终结符返回在非终结符表中的位置（0- ），终结符则返回-1
int GRAMMER::isNonTerminal(char C)
{
	int i;
	int flag = 0;
	for (i = 0; i < nonTerminalNum && flag == 0; ++i)
	{
		if ((nonTerminal[i].nonTerminal) == C)
		{
			flag = 1;
		}
	}
	if (flag == 1)
		return i - 1;
	else
		return -1;
}



//LR分析
//------------------------------------------------------------------------------------------------
//默认构造函数
LR::LR()
{
	//将项目集的项目都初始化为结束标志"\0"
	for (int i = 0; i < STATE_NUM; ++i)
	{
		for (int j = 0; j < ITEM_NUM; ++j)
		{
			itemSetCollection[i][j].itemStr = "\0";
			itemSetCollection[i][j].productionOrder = -1;
			itemSetCollection[i][j].candidateOrder = -1;
		}
	}
}

//比较2个项目集是否相同，相同返回1，不同返回0，可以通过切割产生式和后缀
int LR::CMP_ItemSet(string * itemSet1, string * itemSet2)
{
	//未完成
	return 1;
}

void LR::Closure(string * itemSet)//itemSet为一个string数组,"\0"表终结
{
	int changeFlag = 1;//存在改动
	while (changeFlag == 1)
	{
		for (int i = 0; *(itemSet + i) != "\0"; ++i)//每个项目A->a.Bw,a|b...
		{
			int j = 0;
			while ((*(itemSet + i))[j] != '.')
			{
				++j;
			}
			++j;//到.之后一位字符
			/*if ((*(itemSet + i))[j] == ',')//规约，填分析表
			{
			}*/
			char leftP = (*(itemSet + i))[j];
			if (leftP != ',' && G.isNonTerminal(leftP) != -1)
			{
				int theProduction = G.isNonTerminal(leftP);
				for (int num = 0; num < G.productionList[theProduction].candidateNum; ++num)//文法G的每个产生式B->...
				{
					//如果该产生式不存在
					//未写
					int k = j;
					++k;
					string remainderS;
					int l = 0;
					while ((*(itemSet + i))[k] != ',')
					{
						remainderS[l] = (*(itemSet + i))[k];
						++l;
						++k;
					}
					while ((*(itemSet + i))[k] != '\0')
					{
						++k;
						remainderS[l] = (*(itemSet + i))[k];
						remainderS[l + 1] = '\0';
						++k;
						string firstSet = G.First_of_Str(remainderS);//first集

					}
				}
			}
		}
	}
}

//构造识别文法所有活前缀的LR(1) DFA,即构造LR(1)项目集规范族并构造LR(1)分析表。因为DFA各项目之间的关系即可在分析表中得到
void LR::LR1_DFA()
{

}

//构造LR(1)分析表
/*void LR::LR1_Analyze_Table()
{
}*/

//LR(1)分析程序//算法4.3
void LR::LR1_Analyze()
{
	G.Input_Grammer();//输入文法
	cout << endl << "您输入的文法如下：" << endl;
	G.Output_Grammer();//输出文法
	G.FIRST_Set();//求FIRST集
	G.FOLLOW_Set();//求FOLLOW集
	G.Output_First_Follow();//输出FIRST集和FOLLOW集
	
}

//将待分析的字符串w$放入输入缓冲区,并置向前指针指向w$的第一个符号
void LR::Input()
{
	char C;
	cout << endl << "请输入您想要分析的符号串（以$作为输入结束标志）:" << endl;
	cin >> C;
	int i = 0;
	while (C != '$')
	{
		inputBufer[i] = C;
		++i;
		cin >> C;
	}
	inputBufer[i] = '$';
	forwardIp = 0;//向前指针指向w$的第一个符号
}
