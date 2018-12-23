#include "LR.h"

//ITEM项目
//------------------------------------------------------------------------------------------------
//比较项目i与本项目是否相同，相同返回1，不同返回0
int ITEM::CMP(ITEM a_item)
{
	if (productionOrder == a_item.productionOrder
		&& candidateOrder == a_item.candidateOrder
		&& dotPos == a_item.dotPos
		&& lookaheadChCnt == a_item.lookaheadChCnt)
	{
		int i;
		for (i = 0; i < lookaheadChCnt && lookaheadCh[i] == a_item.lookaheadCh[i]; ++i)
		{
		}
		if (i == lookaheadChCnt)//相同
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

//返回值：-1表示该终结符不存在向前符号集中，0..表示该终结符在向前看符号集中的位置
int ITEM::Lookahead_Exist(char terminal)
{
	int i;
	for (i = 0; i < lookaheadChCnt && lookaheadCh[i] != terminal; ++i)
	{
	}
	if (i == lookaheadChCnt)//说明不存在
	{
		return -1;
	}
	else//存在，返回在向前看符号表中的序号
	{
		return i;
	}
}


//ITEMSET项目集
//------------------------------------------------------------------------------------------------
//输入：产生式序号，候选式序号，.位置。
//输出：-1表示项目不存在，0..表示项目在该项目集中的位置序号，只看所用表达式和.的位置，不看向前符号
int ITEMSET::Item_Exist(int pOrder, int cOrder, int dPos)
{
	int i;
	int  existFlag = 0;
	for (i = 0; i < itemCnt && existFlag == 0; ++i)
	{
		if (pOrder == itemSet[i].productionOrder
			&& cOrder == itemSet[i].candidateOrder
			&& dPos == itemSet[i].dotPos)
		{
			existFlag = 1;
		}
	}
	if (existFlag == 1)//存在
		return (i - 1);
	else//不存在
		return -1;
}


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

//求一个字符串的FIRST集,fisrCnt用来返回first集元素个数，函数返回string是first集
string GRAMMER::First_of_Str(string s, int & firstCnt)
{
	firstCnt = 0;
	string firstCh;
	int lastBlankFlag = 1;//表示前一个字符为空，所以求first集要继续看下一个字符
	for (int i = 0; i < s.length() && lastBlankFlag == 1; ++i)
	{
		lastBlankFlag = 0;
		//不会出现空字符的情况
		int terminalPos = isTerminal(s[i]);
		int nonTerminalPos = isNonTerminal(s[i]);
		if (terminalPos > -1)//是终结符，则first集就直接填入该元素，不用继续循环
		{
			firstCh[firstCnt] = terminal[terminalPos];
			++firstCnt;
		}
		else//是非终结符
		{
			for (int j = 0; j < nonTerminal[nonTerminalNum].FIRSTNum; ++j)
			{
				if (nonTerminal[nonTerminalNum].FIRST[j] == '~')
				{
					if (j != nonTerminal[nonTerminalNum].FIRSTNum - 1)//不是最后一个字符的first集含空
					{
						lastBlankFlag = 1;
					}
					else//最后一个字符的first集含空
					{
						firstCh[firstCnt] = '~';
						++firstCnt;
					}
				}
				else
				{
					firstCh[firstCnt] = nonTerminal[nonTerminalNum].FIRST[j];
					++firstCnt;
				}
			}
		}
	}
	return firstCh;
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

//拓广文法，只是将S'->S加到文法产生式集合末尾，其他一律不变！
void GRAMMER::ExtendGrammer()
{
	if (productionNum != 0)//文法不为空
	{
		productionList[productionNum].leftPart = '\0';//因为左部只能表示单个字符，所以无法存入S'，用'\0'代替
		productionList[productionNum].candidateNum = 1;
		productionList[productionNum].formula[0][0] = nonTerminal[0].nonTerminal;
		productionList[productionNum].formula[0][1] = '\0';
	}
}



//LR分析
//------------------------------------------------------------------------------------------------
//构造项目集的闭包
void LR::Closure(ITEMSET* the_ItemSet)//输入：项目集itemSet
{
	int changeFlag = 1;//存在改动
	while (changeFlag == 1)
	{
		changeFlag = 0;
		//遍历项目集中的每一个项目[A->a.Bw,a/b/..]
		for (int i = 0; i < the_ItemSet->itemCnt; ++i)//当前遍历的item位置为i
		{
			//项目A->a.Bw,a/b/..
			int theProduction = the_ItemSet->itemSet[i].productionOrder;
			int theCandidate = the_ItemSet->itemSet[i].candidateOrder;
			int theDotPos = the_ItemSet->itemSet[i].dotPos;
			if (G.productionList[theProduction].formula[theCandidate][theDotPos] != '\0')//不是规约项目
			{
				//B在非终结符表中的位置
				int leftPos = G.isNonTerminal(G.productionList[theProduction].formula[theCandidate][theDotPos]);
				//B是非终结符
				if (leftPos > -1)
				{
					//遍历文法G中B的每个候选式B->???
					for (int j = 0; j < G.productionList[leftPos].candidateNum; ++j)
					{
						//如果[B->.???]不存在，则加入
						int itemPos = the_ItemSet->Item_Exist(leftPos, j, 0);//itemPos代表要增加/修改的项目序号
						if (itemPos == -1)//该项目不存在，加入
						{
							changeFlag = 1;
							the_ItemSet->itemSet[the_ItemSet->itemCnt].productionOrder = leftPos;
							the_ItemSet->itemSet[the_ItemSet->itemCnt].candidateOrder = j;
							the_ItemSet->itemSet[the_ItemSet->itemCnt].dotPos = 0;
							the_ItemSet->itemSet[the_ItemSet->itemCnt].lookaheadChCnt = 0;
							itemPos = the_ItemSet->itemCnt;
							++(the_ItemSet->itemCnt);//项目集项目数+1
						}
						//遍历FIRST[wa/wb/...]中的每个终结符t，如果[B->.???,t]不在项目中，则把t加入[B->.???]的lookahead中	
						//求FIRST集
						string remainderStr;
						//','左边（即产生式）的剩余部分
						for (int k = theDotPos + 1; G.productionList[theProduction].formula[theCandidate][k] != '\0'; ++k)
						{
							remainderStr += G.productionList[theProduction].formula[theCandidate][k];
						}
						int firstCnt = 0;//fisrt集中非终结符个数
						string firstCh;//first集
						//对每一个小尾巴（lookahead）
						for (int k = 0; k < the_ItemSet->itemSet[i].lookaheadChCnt; ++k)
						{
							string temp_remainderStr = remainderStr;
							temp_remainderStr += the_ItemSet->itemSet[i].lookaheadCh[k];
							firstCh = G.First_of_Str(temp_remainderStr, firstCnt);
							for (int l = 0; l < firstCnt; ++l)
							{
								int lookaheadPos = the_ItemSet->itemSet[itemPos].Lookahead_Exist(firstCh[l]);
								if (lookaheadPos == -1)//该first元素不存在lookahead集合中,则加入
								{
									changeFlag = 1;
									lookaheadPos = the_ItemSet->itemSet[itemPos].lookaheadChCnt;
									the_ItemSet->itemSet[itemPos].lookaheadCh[lookaheadPos] = firstCh[l];
									++(the_ItemSet->itemSet[itemPos].lookaheadChCnt);//该项目小尾巴数+1
								}
							}
						}
						
					}
				}
			}
		}
	}
}

//构造识别文法所有活前缀的LR(1) DFA,即构造LR(1)项目集规范族并构造LR(1)分析表。因为DFA各项目之间的关系即可在分析表中得到
void LR::LR1_DFA()
{
	//对分析表初始化
	//action分析表初始化
	for (int i = 0; i < STATE_NUM; ++i)
	{
		for (int j = 0; j < TERMINAL_NUM; ++j)
		{
			actionTable[i][j] = "\0";
		}
	}
	//goto分析表初始化
	for (int i = 0; i < STATE_NUM; ++i)
	{
		for (int j = 0; j < NONTERMINAL_NUM; ++j)
		{
			gotoTable[i][j] = -1;
		}
	}

	//拓广文法
	//stateNum=0;
	itemSetCollection[stateNum].itemSet[0].productionOrder = G.productionNum;//拓广文法S'->S这一产生式存储在文法产生式集合最后的位置
	itemSetCollection[stateNum].itemSet[0].candidateOrder = 0;//E'->E,拓广文法的候选式序号设为-1
	//[S'->.S,$]
	itemSetCollection[stateNum].itemSet[0].dotPos = 0;
	itemSetCollection[stateNum].itemSet[0].lookaheadCh[0] = '$';
	itemSetCollection[stateNum].itemSet[0].lookaheadChCnt = 1;
	stateNum = 1;
	itemSetCollection[0].itemCnt = 1;
	//closure([S'->.S,$])
	Closure(&itemSetCollection[0]);

	int changeFlag = 1;//有更新
	while (changeFlag == 1)
	{
		changeFlag = 0;
		ITEMSET newItemSet;
		//遍历每一个项目集I,即每一个状态
		for (int i = 0; i < stateNum; ++i)
		{
			//遍历每一个文法符号x（包括终结符和非终结符）
			//先遍历非终结符
			for (int j = 0; j < G.nonTerminalNum; ++j)
			{	//每一个文法符号都可能产生一个新的项目集
				newItemSet.itemCnt = 0;

				char x = G.nonTerminal[j].nonTerminal;
				int xExistFlag = 0;//=0表示go（I,X)=空，即该项目集不存在此转换
				
				//遍历该项目集的每一个项目
				for (int k = 0; k < itemSetCollection[i].itemCnt; ++k)
				{
					int currentDotPos = itemSetCollection[i].itemSet[k].dotPos;
					int currentProductionOrder = itemSetCollection[i].itemSet[k].productionOrder;
					int currentCandidateOrder = itemSetCollection[i].itemSet[k].candidateOrder;
					int currentLookaheadCnt = itemSetCollection[i].itemSet[k].lookaheadChCnt;
					//A->b.xw,..
					if (G.productionList[currentProductionOrder].formula[currentCandidateOrder][currentDotPos] == x)
					{
						xExistFlag = 1;
						newItemSet.itemSet[newItemSet.itemCnt].productionOrder = currentProductionOrder;
						newItemSet.itemSet[newItemSet.itemCnt].candidateOrder = currentCandidateOrder;
						newItemSet.itemSet[newItemSet.itemCnt].dotPos = currentDotPos + 1;//.右移一位
						newItemSet.itemSet[newItemSet.itemCnt].lookaheadChCnt = currentLookaheadCnt;
						for (int l = 0; l < currentLookaheadCnt; ++l)
						{
							newItemSet.itemSet[newItemSet.itemCnt].lookaheadCh[l] = itemSetCollection[i].itemSet[k].lookaheadCh[l];
						}
						++newItemSet.itemCnt;
					}
				}
				if (xExistFlag == 1)//go（I, X) != 空
				{
					Closure(&newItemSet);
					int newItemExisted = ItemSet_Exist(newItemSet);
					if (newItemExisted == -1)//不存在该项目集，则加入,并在分析表中填入状态转移序号
					{
						itemSetCollection[stateNum] = newItemSet;//是否要自己重载等号运算符？
						gotoTable[i][j] = stateNum;
						++stateNum;
					}
					else//该项目集已存在
					{
						gotoTable[i][j] = newItemExisted;
					}
				}
			}
			//再遍历终结符
			for (int j = 0; j < G.terminalNum; ++j)
			{	//每一个文法符号都可能产生一个新的项目集
				newItemSet.itemCnt = 0;

				char x = G.terminal[j];
				int xExistFlag = 0;//=0表示go（I,X)=空，即该项目集不存在此转换

				//遍历该项目集的每一个项目
				for (int k = 0; k < itemSetCollection[i].itemCnt; ++k)
				{
					int currentDotPos = itemSetCollection[i].itemSet[k].dotPos;
					int currentProductionOrder = itemSetCollection[i].itemSet[k].productionOrder;
					int currentCandidateOrder = itemSetCollection[i].itemSet[k].candidateOrder;
					int currentLookaheadCnt = itemSetCollection[i].itemSet[k].lookaheadChCnt;
					//A->b.xw,..
					if (G.productionList[currentProductionOrder].formula[currentCandidateOrder][currentDotPos] == x)
					{
						xExistFlag = 1;
						newItemSet.itemSet[newItemSet.itemCnt].productionOrder = currentProductionOrder;
						newItemSet.itemSet[newItemSet.itemCnt].candidateOrder = currentCandidateOrder;
						newItemSet.itemSet[newItemSet.itemCnt].dotPos = currentDotPos + 1;//.右移一位
						newItemSet.itemSet[newItemSet.itemCnt].lookaheadChCnt = currentLookaheadCnt;
						for (int l = 0; l < currentLookaheadCnt; ++l)
						{
							newItemSet.itemSet[newItemSet.itemCnt].lookaheadCh[l] = itemSetCollection[i].itemSet[k].lookaheadCh[l];
						}
						++newItemSet.itemCnt;
					}
				}
				if (xExistFlag == 1)//go（I, X) != 空
				{
					Closure(&newItemSet);
					int newItemExisted = ItemSet_Exist(newItemSet);
					if (newItemExisted == -1)//不存在该项目集，则加入,并在分析表中填入状态转移序号
					{
						itemSetCollection[stateNum] = newItemSet;//是否要自己重载等号运算符？
						actionTable[i][j] = "S";
						actionTable[i][j] += to_string(stateNum);
						++stateNum;
					}
					else//该项目集已存在
					{
						actionTable[i][j] = "S";
						actionTable[i][j] += to_string(newItemExisted);
					}
				}
			}
		}
	}
}

//构造LR(1)分析表，填充规约和接收项，转移表项在LR1_DFA（）中填写
void LR::LR1_Analyze_Table()
{
	for (int i = 0; i < stateNum; ++i)
	{
		for (int j = 0; j < itemSetCollection[i].itemCnt; ++j)
		{
			int currentProductionOrder = itemSetCollection[i].itemSet[j].productionOrder;
			int currentCandidateOrder = itemSetCollection[i].itemSet[j].candidateOrder;
			int currentDotPos = itemSetCollection[i].itemSet[j].dotPos;
			//是规约项
			if (G.productionList[currentProductionOrder].formula[currentCandidateOrder][currentDotPos] == '\0')
			{
				string R;
				if (currentProductionOrder == G.productionNum)//是S'->S接受项
				{
					R = "ACC";
				}
				else
				{
					R = "R|";
					R += to_string(currentProductionOrder) + "|";
					R += to_string(currentCandidateOrder) + "|";
					R += " ";
					R += G.nonTerminal[currentProductionOrder].nonTerminal;
					R += "->";
					for (int l = 0; l < currentDotPos; ++l)
					{
						R += G.productionList[currentProductionOrder].formula[currentCandidateOrder][l];
					}
					R += "|";
				}
				
				//对每个小尾巴对应的表项填入规约式
				for (int k = 0; k < itemSetCollection[i].itemSet[j].lookaheadChCnt; ++k)
				{
					int terminalOrder = G.isTerminal(itemSetCollection[i].itemSet[j].lookaheadCh[k]);
					actionTable[i][terminalOrder] = R;
				}
			}
		}
	}
}

//LR(1)分析程序//算法4.3
void LR::LR1_Analyze()
{
	G.Input_Grammer();//输入文法
	cout << endl << "您输入的文法如下：" << endl;
	G.Output_Grammer();//输出文法
	G.FIRST_Set();//求FIRST集
	G.FOLLOW_Set();//求FOLLOW集
	G.Output_First_Follow();//输出FIRST集和FOLLOW集
	G.ExtendGrammer();//拓展文法
	
	LR1_DFA();
	LR1_Analyze_Table();
	cout << endl << "该文法的LR(1)项目集规范族如下：" << endl;
	Output_LR1_ItemSetCollection();
	cout << endl << "该文法的LR(1)分析表如下：" << endl;
	OutPut_LR1_Analyze_Table();

	//LR分析程序
	cout << endl << "输入q退出,输入1进行LR(1)分析，输入2进行LR(1)翻译，其余键默认进行LR(1)分析" << endl;
	char inCh;
	cin >> inCh;
	while (inCh != 'q')
	{
		if (inCh == '2')
		{
			Translate();
		}
		else
		{
			//输入符号串及初始化
			Input();
			forwardIp = 0;
			//stack <int> stateStk;//状态栈
			//stack <char> symbleStk;//符号栈
			//stateStk.push(0);
			//symbleStk.push('-');
			int stateStk[100];//状态栈
			char symbleStk[100];//符号栈
			stateStk[0] = 0;
			symbleStk[0] = '-';
			int stkTop = 0;//栈顶指针

			cout << endl << "对该符号串";
			cout << "的分析动作如下表：(注：栈第一行代表状态栈，第二行代表符号栈)" << endl;
			cout << "步骤 ";//5位

			//for (int i = 0; i < 24; ++i)
			//{
			//	cout << " ";
			//}
			cout << "栈";//50位
			for (int i = 0; i < 48; ++i)
			{
				cout << " ";
			}
			cout << "输入";//30位
			for (int i = 0; i < 26 + 2; ++i)
			{
				cout << " ";
			}
			cout << "分析动作" << endl;

			//开始分析
			int goOnFlag = 1;
			for (int i = 0; goOnFlag == 1; ++i)
			{
				//状态
				cout << i;
				if (i < 10)
				{
					cout << "    ";
				}
				else
					cout << "   ";

				//状态栈
				int j;
				int len1 = 0;
				for (j = 0; j <= stkTop; ++j)
				{
					if (stateStk[j] >= 10)
						len1 += 2;
					else
						len1 += 1;
					cout << stateStk[j] << " ";
					++len1;
				}
				for (j = len1 + 1; j < 50; ++j)
				{
					cout << " ";
				}
				int oldStkTop = stkTop;
				int oldStateStk[100];
				char oldSymbleStk[100];
				for (j = 0; j <= stkTop; ++j)
				{
					oldStateStk[j] = stateStk[j];
					oldSymbleStk[j] = symbleStk[j];
				}

				//输入
				for (j = 0; inputBufer[j] != '\0'; ++j)
				{
					if (j < forwardIp)
					{
						cout << " ";
					}
					else
					{
						cout << inputBufer[j];
					}
				}
				for (; j < 30; ++j)
				{
					cout << " ";
				}

				//分析动作
				int inputTerminalPos = G.isTerminal(inputBufer[forwardIp]);
				if (actionTable[stateStk[stkTop]][inputTerminalPos][0] == 'S')
				{
					string tempStr = actionTable[stateStk[stkTop]][inputTerminalPos];
					tempStr.erase(0, 1);
					cout << "Shift " << tempStr << endl;

					++stkTop;
					stateStk[stkTop] = stoi(tempStr);
					symbleStk[stkTop] = inputBufer[forwardIp];
					++forwardIp;
				}
				else if (actionTable[stateStk[stkTop]][inputTerminalPos][0] == 'R')//R|productionOrder|candidateOrder| S->ABC|
				{
					//分词
					char actionCh[50];
					strcpy_s(actionCh, actionTable[stateStk[stkTop]][inputTerminalPos].c_str());
					char *nextToken = NULL;
					const char* split = "|";
					char * dataPart = strtok_s(actionCh, split, &nextToken);
					string thePart = dataPart;//分出'R'
					dataPart = strtok_s(NULL, split, &nextToken);
					thePart = dataPart;//产生式左部序号
					int leftOrder = stoi(thePart);
					dataPart = strtok_s(NULL, split, &nextToken);
					thePart = dataPart;//候选式序号
					dataPart = strtok_s(NULL, split, &nextToken);
					thePart = dataPart;//" 规约产生式"

					stkTop = stkTop - thePart.length() + 4 + 1;//去掉空格和A->共4个符号
					symbleStk[stkTop] = thePart[1];
					stateStk[stkTop] = gotoTable[stateStk[stkTop - 1]][leftOrder];

					cout << "Reduce by" << thePart << endl;
				}
				else if (actionTable[stateStk[stkTop]][inputTerminalPos] == "ACC")
				{
					goOnFlag = 0;
					cout << "ACC" << endl;
				}
				else//错误
				{
					cout << endl << "错误！" << endl;
					goOnFlag = 0;
				}

				//符号栈
				cout << "     ";
				for (j = 0; j <= oldStkTop; ++j)
				{
					cout << oldSymbleStk[j];
					if (oldStateStk[j] >= 10)
						cout << "  ";
					else
						cout << " ";
				}
				cout << endl;
			}
		}
		
		cout << endl << "输入q退出,输入1进行LR(1)分析，输入2进行LR(1)翻译，其余键默认进行LR(1)分析" << endl;
		cin >> inCh;
	}
}

//将待分析的字符串w$放入输入缓冲区,并置向前指针指向w$的第一个符号
void LR::Input()
{
	char C;
	cout << endl << "请输入您想要分析的符号串（以$作为输入结束标志，数字只识别正整数）:" << endl;
	cin >> C;
	int i = 0;
	numCnt = 0;
	while (C != '$')
	{
		//判断是否为数字
		if (C<'0' || C>'9')//不是数字
		{
			inputBufer[i] = C;
			++i;
			cin >> C;
		}
		else//是数字
		{
			inputBufer[i] = 'n';
			++i;
			int number = 0;
			while (C >= '0' && C <= '9')
			{
				number = number * 10 + C - '0';
				cin >> C;
			}
			inputNum[numCnt] = number;
			++numCnt;
		}
	}
	inputBufer[i] = '$';
	inputBufer[i+1] = '\0';
	forwardIp = 0;//向前指针指向w$的第一个符号
}

//判断项目集是否已经存在，不存在返回-1，存在返回在项目集规范族中的序号0..(即状态序号)
int LR::ItemSet_Exist(ITEMSET newItemSet)
{
	int existFlag = 0;
	//遍历已存在的每一个项目集
	int i;
	for (i = 0; i < stateNum && existFlag == 0; ++i)
	{
		existFlag = 1;//假设存在,即项目相同
		if (newItemSet.itemCnt == itemSetCollection[i].itemCnt)//若项目数相同，继续比较项目集具体内容
		{
			//比较每一个项目是否一样
			existFlag = 1;
			for (int j = 0; j < newItemSet.itemCnt && existFlag == 1; ++j)
			{
				int sameItemPos = itemSetCollection[i].Item_Exist(newItemSet.itemSet[j].productionOrder,
					newItemSet.itemSet[j].candidateOrder, newItemSet.itemSet[j].dotPos);
				//若项目存在，且小尾巴数目相同，继续比较小尾巴
				if (sameItemPos > -1
					&& newItemSet.itemSet[j].lookaheadChCnt == itemSetCollection[i].itemSet[sameItemPos].lookaheadChCnt)
				{
					//比较每一个小尾巴是否一样
					existFlag = 1;
					for (int k = 0; k < newItemSet.itemSet[j].lookaheadChCnt && existFlag == 1; ++k)
					{
						char currenLookahead = newItemSet.itemSet[j].lookaheadCh[k];
						int sameLookaheadPos = itemSetCollection[i].itemSet[sameItemPos].Lookahead_Exist(currenLookahead);
						if (sameLookaheadPos == -1)//小尾巴不存在
						{
							existFlag = 0;
						}
						else
							existFlag = 1;
					}
					
				}
				else
				{
					existFlag = 0;
				}
			}
		}	
		else
		{
			existFlag = 0;
		}
	}
	if (existFlag == 1)//存在
		return (i - 1);
	else
		return -1;
}

//输出项目集规范族
void LR::Output_LR1_ItemSetCollection()
{
	for (int i = 0; i < stateNum; ++i)
	{
		cout << "I" << i << ": ";
		for (int j = 0; j < itemSetCollection[i].itemCnt; ++j)
		{
			cout << "[";
			int currentProductionOrder = itemSetCollection[i].itemSet[j].productionOrder;
			int currentCandidateOrder = itemSetCollection[i].itemSet[j].candidateOrder;
			int currentDotPos = itemSetCollection[i].itemSet[j].dotPos;
			int currentLookaheadCnt = itemSetCollection[i].itemSet[j].lookaheadChCnt;
			if (currentProductionOrder == G.productionNum)//是S->S'
			{
				cout << G.nonTerminal[0].nonTerminal;
				cout << "'";
			}
			else
			{
				cout << G.nonTerminal[currentProductionOrder].nonTerminal;
			}
			cout << "->";
			int k;
			for (k = 0; G.productionList[currentProductionOrder].formula[currentCandidateOrder][k] != '\0'; ++k)
			{
				if (currentDotPos == k)
				{
					cout << ".";
				}
				cout << G.productionList[currentProductionOrder].formula[currentCandidateOrder][k];
			}
			if (currentDotPos == k)
			{
				cout << ".";
			}
			cout << ",";
			for (int k = 0; k < currentLookaheadCnt; ++k)
			{
				cout << itemSetCollection[i].itemSet[j].lookaheadCh[k];
				if (k != currentLookaheadCnt - 1)
				{
					cout << "|";
				}
			}
			cout << "] ";
		}
		cout << endl;
	}
}

//输出LR(1)分析表
void LR::OutPut_LR1_Analyze_Table()
{
	for (int i = 0; i < 6 + (G.terminalNum + G.nonTerminalNum) * 11; ++i)
		cout << "-";//输出行分割线
	cout << endl;

	//第一行 状态，action，goto
	cout << "状态 |";
	int i;
	for (i = 0; i < G.terminalNum / 2; ++i)
	{
		for (int j = 0; j < 11; ++j)//一个表项输出10位，加上‘|’11位
		{
			cout << " ";
		}
	}
	cout << "  action   ";
	for (++i; i < G.terminalNum; ++i)
	{
		int len;
		if (i == G.terminalNum - 1)
		{
			len = 10;
		}
		else
			len = 11;
		for (int j = 0; j < len; ++j)
		{
			cout << " ";
		}
	}
	cout << "|";
	//输出goto其实可以缩减表项长度位5...
	for (i = 0; i < G.nonTerminalNum / 2; ++i)
	{
		for (int j = 0; j < 11; ++j)//一个表项输出10位，加上‘|’11位
		{
			cout << " ";
		}
	}
	cout << "   goto   ";
	for (++i; i < G.nonTerminalNum; ++i)
	{
		int len =  11;
		for (int j = 0; j < len; ++j)
		{
			cout << " ";
		}
	}
	cout << "|";
	cout << endl;

	for (int i = 0; i < 6 + (G.terminalNum + G.nonTerminalNum) * 11; ++i)
		cout << "-";//输出行分割线
	cout << endl;

	//第二行 终结符和非终结符
	cout << "     |";
	for (i = 0; i < G.terminalNum; ++i)
	{
		cout << "    ";
		cout << G.terminal[i];
		cout << "     |";
	}
	for (i = 0; i < G.nonTerminalNum; ++i)
	{
		cout << "    ";
		cout << G.nonTerminal[i].nonTerminal;
		cout << "     ";
		//if (i != G.nonTerminalNum - 1)
		//{
			cout << "|";
		//}
	}
	cout << endl;

	for (int i = 0; i < 6 + (G.terminalNum + G.nonTerminalNum) * 11; ++i)
		cout << "-";//输出行分割线
	cout << endl;

	//输出剩余表项
	for (i = 0; i < stateNum; ++i)
	{
		if (i < 10)
			cout << "  ";
		else
			cout << " ";
		cout << i;
		cout << "  |";
		
		for (int j = 0; j < G.terminalNum; ++j)
		{
			if (actionTable[i][j] == "\0")
				cout << "          |";
			else if (actionTable[i][j] == "ACC")
			{
				cout << "   ACC    |";
			}
			else if (actionTable[i][j][0] == 'S')
			{
				cout << "    " << actionTable[i][j];
				if (actionTable[i][j].length() == 2)
					cout << "    ";
				else
					cout << "   ";
				cout << "|";
			}
			else if (actionTable[i][j][0] = 'R')
			{
				//分词
				char actionCh[50];
				strcpy_s(actionCh, actionTable[i][j].c_str());
				char *nextToken = NULL;
				const char* split = "|";
				char * dataPart = strtok_s(actionCh, split, &nextToken);
				string thePart = dataPart;//分出'R'
				dataPart = strtok_s(NULL, split, &nextToken);
				thePart = dataPart;//产生式左部序号
				dataPart = strtok_s(NULL, split, &nextToken);
				thePart = dataPart;//候选式序号
				dataPart = strtok_s(NULL, split, &nextToken);
				thePart = dataPart;

				cout << " ";
				cout << "R";
				cout << thePart;
				//cout << actionTable[i][j];
				for (int k = thePart.length() + 1 ; k < 10-1; ++k)
				{
					cout << " ";
				}
				cout << "|";
			}
		}
		for (int j = 0; j < G.nonTerminalNum; ++j)
		{
			if (gotoTable[i][j] == -1)
			{
				cout << "          |";
			}
			else
			{
				cout << "    ";
				cout << gotoTable[i][j];
				if (gotoTable[i][j] >= 10)
					cout << "    ";
				else
					cout << "     ";
				cout << "|";
			}
		}
		cout << endl;
		for (int i = 0; i < 6 + (G.terminalNum + G.nonTerminalNum) * 11; ++i)
			cout << "-";//输出行分割线
		cout << endl;
	}
}

//翻译程序
void LR::Translate()
{
	//输入符号串及初始化
	Input();
	forwardIp = 0;
	int stateStk[100];//状态栈
	string valStk[100];//value栈
	stateStk[0] = 0;
	valStk[0] = "-";
	int stkTop = 0;//栈顶指针

	cout << endl << "对该符号串";
	cout << "的分析动作如下表：(注：栈第一行代表状态栈，第二行代表value栈)" << endl;
	cout << "步骤 ";//5位

	//for (int i = 0; i < 24; ++i)
	//{
	//	cout << " ";
	//}
	cout << "栈";//50位
	for (int i = 0; i < 48; ++i)
	{
		cout << " ";
	}
	cout << "输入";//30位
	for (int i = 0; i < 26 + 2; ++i)
	{
		cout << " ";
	}
	cout << "分析及翻译动作" << endl;

	//开始分析
	int theNumCnt = 0;
	int goOnFlag = 1;
	for (int i = 0; goOnFlag == 1; ++i)
	{
		//状态
		cout << i;
		if (i < 10)
		{
			cout << "    ";
		}
		else
			cout << "   ";

		//value栈
		int j;
		int len1 = 0;
		for (j = 0; j <= stkTop; ++j)
		{
			int len2;
			if (integerBits(stateStk[j]) >= valStk[j].length())
			{
				len1 += integerBits(stateStk[j]);
				len2 = integerBits(stateStk[j]);
			}
			else
			{
				len1 += valStk[j].length();
				len2 = valStk[j].length();
			}
			cout << stateStk[j];
			for (int k = integerBits(stateStk[j]); k < len2; ++k)
				cout << " ";
			cout << " ";
			++len1;
		}
		for (j = len1 + 1; j < 50; ++j)
		{
			cout << " ";
		}
		int oldStkTop = stkTop;
		int oldStateStk[100];
		string oldvalStk[100];
		for (j = 0; j <= stkTop; ++j)
		{
			oldStateStk[j] = stateStk[j];
			oldvalStk[j] = valStk[j];
		}

		//输入
		for (j = 0; inputBufer[j] != '\0'; ++j)
		{
			if (j < forwardIp)
			{
				cout << " ";
			}
			else
			{
				cout << inputBufer[j];
			}
		}
		for (; j < 30; ++j)
		{
			cout << " ";
		}

		//分析动作
		int inputTerminalPos = G.isTerminal(inputBufer[forwardIp]);
		if (actionTable[stateStk[stkTop]][inputTerminalPos][0] == 'S')//移进
		{
			string tempStr = actionTable[stateStk[stkTop]][inputTerminalPos];
			tempStr.erase(0, 1);
			//cout << "Shift " << tempStr;

			++stkTop;
			stateStk[stkTop] = stoi(tempStr);
			if (inputBufer[forwardIp] == 'n')
			{
				valStk[stkTop] = to_string(inputNum[theNumCnt]);
				cout << "val[newtop]=" << valStk[stkTop] << ",";
				++theNumCnt;
			}
			else
			{
				valStk[stkTop] = " ";
			}
			++forwardIp;

			cout << "Shift " << tempStr << endl;
		}
		//规约
		else if (actionTable[stateStk[stkTop]][inputTerminalPos][0] == 'R')//R|productionOrder|candidateOrder| S->ABC|
		{
			//分词
			char actionCh[50];
			strcpy_s(actionCh, actionTable[stateStk[stkTop]][inputTerminalPos].c_str());
			char *nextToken = NULL;
			const char* split = "|";
			char * dataPart = strtok_s(actionCh, split, &nextToken);
			string thePart = dataPart;//分出'R'
			dataPart = strtok_s(NULL, split, &nextToken);
			thePart = dataPart;//产生式左部序号
			int leftOrder = stoi(thePart);
			dataPart = strtok_s(NULL, split, &nextToken);
			thePart = dataPart;//候选式序号
			dataPart = strtok_s(NULL, split, &nextToken);
			thePart = dataPart;//" 规约产生式"

			int newValue;
			if (thePart[5] == '+')
			{
				newValue = stoi(valStk[stkTop - 2]) + stoi(valStk[stkTop]);
				cout << "val[newtop]=val[top-2]+val[top]=" << newValue << " , ";
				stkTop = stkTop - thePart.length() + 4 + 1;//去掉空格和A->共4个符号
				valStk[stkTop] = to_string(newValue);
			}
			else if (thePart[5] == '-')
			{
				newValue = stoi(valStk[stkTop - 2]) - stoi(valStk[stkTop]);
				cout << "val[newtop]=val[top-2]-val[top]=" << newValue << " , ";
				stkTop = stkTop - thePart.length() + 4 + 1;//去掉空格和A->共4个符号
				valStk[stkTop] = to_string(newValue);
			}
			else if (thePart[5] == '*')
			{
				newValue = stoi(valStk[stkTop - 2]) * stoi(valStk[stkTop]);
				cout << "val[newtop]=val[top-2]*val[top]=" << newValue << " , ";
				stkTop = stkTop - thePart.length() + 4 + 1;//去掉空格和A->共4个符号
				valStk[stkTop] = to_string(newValue);
			}
			else if (thePart[5] == '/')
			{
				if (stoi(valStk[stkTop]) == 0)
				{
					cout << endl << "错误，除数不能为0！" << endl;
				}
				newValue = stoi(valStk[stkTop - 2]) / stoi(valStk[stkTop]);
				cout << "val[newtop]=val[top-2]/val[top]=" << newValue << " , ";
				stkTop = stkTop - thePart.length() + 4 + 1;//去掉空格和A->共4个符号
				valStk[stkTop] = to_string(newValue);
			}
			else if (thePart[4] == '(')
			{
				newValue = stoi(valStk[stkTop - 1]);
				cout << "val[newtop]=val[top-1]=" << newValue << " , ";
				stkTop = stkTop - thePart.length() + 4 + 1;//去掉空格和A->共4个符号
				valStk[stkTop] = to_string(newValue);
			}
			else
			{
				stkTop = stkTop - thePart.length() + 4 + 1;//去掉空格和A->共4个符号
				//valStk[stkTop] = " ";
			}

			//stkTop = stkTop - thePart.length() + 4 + 1;//去掉空格和A->共4个符号
			//valStk[stkTop] = to_string(newValue);
			stateStk[stkTop] = gotoTable[stateStk[stkTop - 1]][leftOrder];

			cout << "Reduce by" << thePart << endl;
		}
		else if (actionTable[stateStk[stkTop]][inputTerminalPos] == "ACC")
		{
			goOnFlag = 0;
			cout << "print(val[top]=" << valStk[stkTop] << ") , ";
			cout << "ACC" << endl;
		}
		else//错误
		{
			cout << endl << "错误！" << endl;
			goOnFlag = 0;
		}

		//value栈
		cout << "     ";
		len1 = 0;
		for (j = 0; j <= oldStkTop; ++j)
		{
			cout << oldvalStk[j];
			if (integerBits(stateStk[j]) >= valStk[j].length())
				len1 = integerBits(stateStk[j]);
			else
				len1 = valStk[j].length();
			for (int k = valStk[j].length(); k < len1; ++k)
				cout << " ";
			cout << " ";
		}
		cout << endl;
	}
}

//求整数位数
int LR::integerBits(int number)
{
	int sizeTable[9] = { 9,99,999,9999,99999,999999,9999999,99999999,999999999 };
	for (int i = 0; i < 9; ++i)
	{
		if (number <= sizeTable[i])
			return (i + 1);
	}
	return 10;
}

