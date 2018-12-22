#include "LR.h"

//ITEM��Ŀ
//------------------------------------------------------------------------------------------------
//�Ƚ���Ŀi�뱾��Ŀ�Ƿ���ͬ����ͬ����1����ͬ����0
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
		if (i == lookaheadChCnt)//��ͬ
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

//����ֵ��-1��ʾ���ս����������ǰ���ż��У�0..��ʾ���ս������ǰ�����ż��е�λ��
int ITEM::Lookahead_Exist(char terminal)
{
	int i;
	for (i = 0; i < lookaheadChCnt && lookaheadCh[i] != terminal; ++i)
	{
	}
	if (i == lookaheadChCnt)//˵��������
	{
		return -1;
	}
	else//���ڣ���������ǰ�����ű��е����
	{
		return i;
	}
}


//ITEMSET��Ŀ��
//------------------------------------------------------------------------------------------------
//���룺����ʽ��ţ���ѡʽ��ţ�.λ�á�
//�����-1��ʾ��Ŀ�����ڣ�0..��ʾ��Ŀ�ڸ���Ŀ���е�λ����ţ�ֻ�����ñ��ʽ��.��λ�ã�������ǰ����
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
	if (existFlag == 1)//����
		return (i - 1);
	else//������
		return -1;
}


//�ķ�
//------------------------------------------------------------------------------------------------
//���ļ������ķ���ע���Զ���$������ս����ĩ��
void GRAMMER::Input_Grammer()
{
	cout << "��Ŀ�����ķ��洢��G.txt." << endl;
	cout << "�������ķ������ļ�����eg: G.txt)��";
	string fileName;
	cin >> fileName;
	ifstream GFile;
	GFile.open(fileName);
	GFile >> noskipws;//�������ո�ͻس�
	char C;//������ַ�
	int state = 0;
	int errorFlag = 0;
	int theProduction = 0;//ָ�ڼ�������ʽ
	int theCandidate = 0;//ָ�ڼ�����ѡʽ
	int candidatePos = 0;//ָ��ѡʽ�е�λ��
	for (; GFile.peek() != EOF && errorFlag == 0;)
	{
		//GFile >> C;//�����ַ�
		//cout << C << endl;///////////////////////

		switch (state)
		{
		case 0://��ʼ���ȴ��������ʽ��
			GFile >> C;
			if (C == ' ' || C == '\t')//�����ո���Ʊ��
				state = 0;
			else if (C >= 'A' && C <= 'Z')//����ʽ��ӦΪ��д��ĸ
			{
				state = 1;
				int flag = 0;
				int i;
				for (i = 0; i < productionNum && flag == 0; ++i)
				{
					if (C == productionList[i].leftPart)//���Ѵ���
					{
						flag = 1;
					}
				}
				if (flag == 1)//˵�����Ѵ���
				{
					theProduction = i - 1;
				}
				else//�󲿲�����
				{
					theProduction = productionNum;
					productionList[theProduction].leftPart = C;
					++productionNum;//��ͬ�󲿵Ĳ���ʽ��Ŀ+1

					//������ս����
					nonTerminal[nonTerminalNum].nonTerminal = C;
					++nonTerminalNum;
				}
			}
			else
			{
				errorFlag = 1;
				cout << "����ʽ��Ϊ���ս����Ӧ����һ����д��ĸ" << endl;
			}
			break;
		case 1://����-  ��->��
			GFile >> C;
			if (C == ' ' || C == '\t')
				state = 1;
			else if (C == '-')
				state = 2;
			else
			{
				errorFlag = 1;
				cout << "�ķ���'->'֮�䲻���пո�������ַ�" << endl;
			}
			break;
		case 2://������->
			GFile >> C;
			if (C == '>')
			{
				state = 3;
			}
			else
			{
				errorFlag = 1;
				cout << "�ķ���'->'֮�䲻���пո�������ַ�" << endl;
			}
			break;
		case 3:
			GFile >> C;

			if (C == ' ' || C == '\t')
				state = 3;
			else if (C != '\n' && C != '|')//�����ѡʽ��һ���ַ�
			{
				state = 4;
				theCandidate = productionList[theProduction].candidateNum;
				candidatePos = 0;//����ú�ѡʽ�ĵ�һ���ַ�
				productionList[theProduction].formula[theCandidate][candidatePos] = C;
				++(productionList[theProduction].candidateNum);//�ò���ʽ�ĺ�ѡʽ��Ŀ+1

				//�����������ս�������Ƿ��Ѿ��������ս����û�еĻ��ͼ���
				if ((C > 'Z' || C < 'A') && C != '~')//�����Ƿ��ս��
				{
					int i;
					int tFlag = 0;
					for (i = 0; i < terminalNum && tFlag == 0; ++i)
					{
						if (C == terminal[i])//�Ѵ���
						{
							tFlag = 1;
						}
					}
					if (tFlag == 0)//������
					{
						terminal[terminalNum] = C;
						++terminalNum;
					}
				}
			}
			else
			{
				errorFlag = 1;
				cout << "��������ȷ�ĺ�ѡʽ" << endl;
			}
			break;
		case 4:
			GFile >> C;

			if (C == ' ' || C == '\t')//�ո�
			{
				state = 4;//���Կո�
			}
			else if (C == '|')
			{
				state = 3;//��ʼ������һ����ѡʽ
				++candidatePos;
				productionList[theProduction].formula[theCandidate][candidatePos] = '\0';
			}
			else if (C == '\n')
			{
				state = 0;//������һ�����ʽ
				++candidatePos;
				productionList[theProduction].formula[theCandidate][candidatePos] = '\0';
			}
			else//���������ַ���Ϊ����ʽ�Ҳ�
			{
				++candidatePos;//�ַ��ں�ѡʽ�е�λ��+1
				productionList[theProduction].formula[theCandidate][candidatePos] = C;

				//�����������ս�������Ƿ��Ѿ��������ս����û�еĻ��ͼ���
				if ((C > 'Z' || C < 'A') && C != '~')//�����Ƿ��ս��,�Ҳ�Ϊ�ա�~��
				{
					int i;
					int tFlag = 0;
					for (i = 0; i < terminalNum && tFlag == 0; i++)
					{
						if (C == terminal[i])//�Ѵ���
						{
							tFlag = 1;
						}
					}
					if (tFlag == 0)//������
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
	//�ڷ��ս����ĩβ����$
	terminal[terminalNum] = '$';
	++terminalNum;
}

//����ķ�
void GRAMMER::Output_Grammer()
{
	//cout << endl << "�ķ����£�" << endl;
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

//��FIRST��
void GRAMMER::FIRST_Set()
{
	int changeFlag = 1;//�иĶ���FIRST����û���꣬���Լ���ѭ��
	while (changeFlag == 1)
	{
		changeFlag = 0;
		for (int i = 0; i < nonTerminalNum; ++i)//Ϊÿһ�����ս�������䣩FOLLOW��
		{
			for (int j = 0; j < productionList[i].candidateNum; ++j)//�����÷��ս����ÿһ����ѡʽ
			{
				int lastBlank = 1;//ֻ����һ���ַ����Ƴ��գ�����Ҫ������������һ���ַ���FIRST��
				for (int k = 0; productionList[i].formula[j][k] != '\0' && lastBlank == 1; ++k)//��˺�ѡʽ��first������÷��ս����first��
				{
					lastBlank = 0;
					int tPos = isTerminal(productionList[i].formula[j][k]);//��Ϊ�ս�����������ս�����е�λ��
					int nPos = isNonTerminal(productionList[i].formula[j][k]);//��Ϊ���ս���������ڷ��ս�����е�λ��
					if (tPos >= 0 || productionList[i].formula[j][k] == '~')//���ս����~��ֱ�Ӽ���first��
					{
						//if(productionList[i].formula[j][k]!='$')//����Ҫ�жϣ�����ʽ�������$
						int fillPos = nonTerminal[i].FIRSTNum;
						int existFlag = 0;
						for (int l = 0; l < fillPos && existFlag == 0; ++l)
						{
							if (nonTerminal[i].FIRST[l] == productionList[i].formula[j][k])
							{
								existFlag = 1;
							}
						}
						if (existFlag == 0)//ԭ��first�������и��ս����~,����
						{
							nonTerminal[i].FIRST[fillPos] = productionList[i].formula[j][k];
							++(nonTerminal[i].FIRSTNum);
							changeFlag = 1;//�иĶ�
						}
						//lastBlank = 0;//�����ٿ�������ַ���
					}
					else if (nPos >= 0)//�Ƿ��ս��
					{
						for (int l = 0; l < nonTerminal[nPos].FIRSTNum; ++l)//�����÷��ս����ÿһ��first��Ԫ�أ����Ƿ��Ѵ��ڣ������ھͼ���
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
								if (existFlag == 0)//ԭ��first�������и��ս��,����
								{
									nonTerminal[i].FIRST[fillPos] = nonTerminal[nPos].FIRST[l];
									++(nonTerminal[i].FIRSTNum);
									changeFlag = 1;//�иĶ�
								}
							}
							else//�����~
							{
								if (productionList[i].formula[j][k + 1] == '\0')//�Ǻ�ѡʽ���һ���������Ƴ��գ������Ƴ���
								{
									int existFlag = 0;
									for (int m = 0; m < fillPos&&existFlag == 0; ++m)
									{
										if (nonTerminal[i].FIRST[m] == '~')
										{
											existFlag = 1;
										}
									}
									if (existFlag == 0)//ԭ��first��������~,����
									{
										nonTerminal[i].FIRST[fillPos] = '~';
										++(nonTerminal[i].FIRSTNum);
										changeFlag = 1;//�иĶ�
									}
								}
								else//���Լ�����������ַ�
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

//��FOLLOW��
void GRAMMER::FOLLOW_Set()
{
	//���ķ���ʼ���ţ�Ĭ��Ϊ��һ������ʽ�󲿣�����$����FOLLOW��
	nonTerminal[0].FOLLOW[0] = '$';
	++(nonTerminal[0].FOLLOWNum);
	int changeFlag = 1;//�иĶ���˵��FOLLOW����û���꣬����ѭ��ʹ���������ֱ��ÿ�����ϲ������󣨼�changeFlag==0��Ϊֹ
	while (changeFlag == 1)
	{
		changeFlag = 0;
		//i��j����ѭ������ÿһ����ѡʽ
		for (int i = 0; i < productionNum; ++i)
		{
			for (int j = 0; j < productionList[i].candidateNum; ++j)
			{
				//������ѡʽ��ÿ���ַ����ҷ��ս�������ù�������FOLLOW��
				for (int k = 0; productionList[i].formula[j][k] != '\0'; ++k)
				{
					int nPos = isNonTerminal(productionList[i].formula[j][k]);//�����ڷ��ս�����е�λ��
					if (nPos >= 0)//����Ƿ��ս��
					{
						if (productionList[i].formula[j][k + 1] == '\0')//��A->aB ����ʽ�����FOLLOW(A)�е�����Ԫ�ؼ��뵽FOLLOW��B����
						{
							for (int l = 0; l < nonTerminal[i].FOLLOWNum; ++l)//����FOLLOW(A)������FOLLOW��Ԫ�أ���FOLLOW��B)��Ԫ����һ�Ƚϣ��������ڣ��ͼ���
							{
								int existFlag = 0;
								for (int m = 0; m < nonTerminal[nPos].FOLLOWNum && existFlag == 0; ++m)
								{
									if (nonTerminal[nPos].FOLLOW[m] == nonTerminal[i].FOLLOW[l])
									{
										existFlag = 1;
									}
								}
								if (existFlag == 0)//FOLLOW��B)������Ԫ�أ������
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
							//��w��first���ķǿ�Ԫ�ؼ���FOLLOW B����w->~����A��Follow��Ԫ�ؼ���FOLLOW B
							int lastBlank = 1;//ֻ����һ�����Ƴ��գ���Ҫ����һ��Ԫ�ص�first��
							for (int l = k + 1; productionList[i].formula[j][l] != '\0' && lastBlank == 1; ++l)
							{
								lastBlank = 0;
								//if(productionList[i].formula[j][l]=='~')//�����ܳ���A->aB~
								int tPos = isTerminal(productionList[i].formula[j][l]);//�������ս�����е�λ��
								if (tPos >= 0)//���ս������A->aBcw����ʽ��c����FOLLOW B
								{
									int existFlag = 0;
									for (int m = 0; m < nonTerminal[nPos].FOLLOWNum && existFlag == 0; ++m)
									{
										if (nonTerminal[nPos].FOLLOW[m] == terminal[tPos])
										{
											existFlag = 1;
										}
									}
									if (existFlag == 0)//FOLLOW��B)������Ԫ�أ������
									{
										int fillPos = nonTerminal[nPos].FOLLOWNum;
										nonTerminal[nPos].FOLLOW[fillPos] = terminal[tPos];
										++(nonTerminal[nPos].FOLLOWNum);
										changeFlag = 1;
									}
								}
								else//�Ƿ��ս������A->aBCw�����,��C��first���ķǿ�Ԫ�ؼ���FOLLOW B����C->~����������һ����ĸ
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
											if (existFlag == 0)//FOLLOW��B)������Ԫ�أ������
											{
												int fillPos = nonTerminal[nPos].FOLLOWNum;
												nonTerminal[nPos].FOLLOW[fillPos] = nonTerminal[CPos].FIRST[m];
												++(nonTerminal[nPos].FOLLOWNum);
												changeFlag = 1;
											}
										}
										else//C��first����~�����C�Ǹú�ѡʽ���һ���ַ������follow AԪ�ؼ���follow B�������������w
										{
											if (productionList[i].formula[j][l + 1] == '\0')//���C�Ǹú�ѡʽ���һ���ַ������follow AԪ�ؼ���follow B
											{
												for (int n = 0; n < nonTerminal[i].FOLLOWNum; ++n)//����FOLLOW(A)������FOLLOW��Ԫ�أ���FOLLOW��B)��Ԫ����һ�Ƚϣ��������ڣ��ͼ���
												{
													int existFlag = 0;
													for (int o = 0; o < nonTerminal[nPos].FOLLOWNum && existFlag == 0; ++o)
													{
														if (nonTerminal[nPos].FOLLOW[o] == nonTerminal[i].FOLLOW[n])
														{
															existFlag = 1;
														}
													}
													if (existFlag == 0)//FOLLOW��B)������Ԫ�أ������
													{
														int fillPos = nonTerminal[nPos].FOLLOWNum;
														nonTerminal[nPos].FOLLOW[fillPos] = nonTerminal[i].FOLLOW[n];
														++(nonTerminal[nPos].FOLLOWNum);
														changeFlag = 1;
													}
												}
											}
											else//������������ַ�
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

//���FIRST����FOLLOW��
void GRAMMER::Output_First_Follow()//���FIRST����FOLLOW��
{
	cout << endl << "FIRST����FOLLOW�����±� " << endl;
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

//��һ���ַ�����FIRST��,fisrCnt��������first��Ԫ�ظ�������������string��first��
string GRAMMER::First_of_Str(string s, int & firstCnt)
{
	firstCnt = 0;
	string firstCh;
	int lastBlankFlag = 1;//��ʾǰһ���ַ�Ϊ�գ�������first��Ҫ��������һ���ַ�
	for (int i = 0; i < s.length() && lastBlankFlag == 1; ++i)
	{
		lastBlankFlag = 0;
		//������ֿ��ַ������
		int terminalPos = isTerminal(s[i]);
		int nonTerminalPos = isNonTerminal(s[i]);
		if (terminalPos > -1)//���ս������first����ֱ�������Ԫ�أ����ü���ѭ��
		{
			firstCh[firstCnt] = terminal[terminalPos];
			++firstCnt;
		}
		else//�Ƿ��ս��
		{
			for (int j = 0; j < nonTerminal[nonTerminalNum].FIRSTNum; ++j)
			{
				if (nonTerminal[nonTerminalNum].FIRST[j] == '~')
				{
					if (j != nonTerminal[nonTerminalNum].FIRSTNum - 1)//�������һ���ַ���first������
					{
						lastBlankFlag = 1;
					}
					else//���һ���ַ���first������
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

//�ж��Ƿ����ս�����ս���򷵻����ս�����е�λ�ã�0- �������ս���򷵻�-1
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
		return i - 1;//�ս��
	else
		return -1;//���ս��
}

//�ж��Ƿ��Ƿ��ս�������ս�������ڷ��ս�����е�λ�ã�0- �����ս���򷵻�-1
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

//�ع��ķ���ֻ�ǽ�S'->S�ӵ��ķ�����ʽ����ĩβ������һ�ɲ��䣡
void GRAMMER::ExtendGrammer()
{
	if (productionNum != 0)//�ķ���Ϊ��
	{
		productionList[productionNum].leftPart = '\0';//��Ϊ��ֻ�ܱ�ʾ�����ַ��������޷�����S'����'\0'����
		productionList[productionNum].candidateNum = 1;
		productionList[productionNum].formula[0][0] = nonTerminal[0].nonTerminal;
		productionList[productionNum].formula[0][1] = '\0';
	}
}



//LR����
//------------------------------------------------------------------------------------------------
//������Ŀ���ıհ�
void LR::Closure(ITEMSET* the_ItemSet)//���룺��Ŀ��itemSet
{
	int changeFlag = 1;//���ڸĶ�
	while (changeFlag == 1)
	{
		changeFlag = 0;
		//������Ŀ���е�ÿһ����Ŀ[A->a.Bw,a/b/..]
		for (int i = 0; i < the_ItemSet->itemCnt; ++i)//��ǰ������itemλ��Ϊi
		{
			//��ĿA->a.Bw,a/b/..
			int theProduction = the_ItemSet->itemSet[i].productionOrder;
			int theCandidate = the_ItemSet->itemSet[i].candidateOrder;
			int theDotPos = the_ItemSet->itemSet[i].dotPos;
			if (G.productionList[theProduction].formula[theCandidate][theDotPos] != '\0')//���ǹ�Լ��Ŀ
			{
				//B�ڷ��ս�����е�λ��
				int leftPos = G.isNonTerminal(G.productionList[theProduction].formula[theCandidate][theDotPos]);
				//B�Ƿ��ս��
				if (leftPos > -1)
				{
					//�����ķ�G��B��ÿ����ѡʽB->???
					for (int j = 0; j < G.productionList[leftPos].candidateNum; ++j)
					{
						//���[B->.???]�����ڣ������
						int itemPos = the_ItemSet->Item_Exist(leftPos, j, 0);//itemPos����Ҫ����/�޸ĵ���Ŀ���
						if (itemPos == -1)//����Ŀ�����ڣ�����
						{
							changeFlag = 1;
							the_ItemSet->itemSet[the_ItemSet->itemCnt].productionOrder = leftPos;
							the_ItemSet->itemSet[the_ItemSet->itemCnt].candidateOrder = j;
							the_ItemSet->itemSet[the_ItemSet->itemCnt].dotPos = 0;
							the_ItemSet->itemSet[the_ItemSet->itemCnt].lookaheadChCnt = 0;
							itemPos = the_ItemSet->itemCnt;
							++(the_ItemSet->itemCnt);//��Ŀ����Ŀ��+1
						}
						//����FIRST[wa/wb/...]�е�ÿ���ս��t�����[B->.???,t]������Ŀ�У����t����[B->.???]��lookahead��	
						//��FIRST��
						string remainderStr;
						//','��ߣ�������ʽ����ʣ�ಿ��
						for (int k = theDotPos + 1; G.productionList[theProduction].formula[theCandidate][k] != '\0'; ++k)
						{
							remainderStr += G.productionList[theProduction].formula[theCandidate][k];
						}
						int firstCnt = 0;//fisrt���з��ս������
						string firstCh;//first��
						//��ÿһ��Сβ�ͣ�lookahead��
						for (int k = 0; k < the_ItemSet->itemSet[i].lookaheadChCnt; ++k)
						{
							string temp_remainderStr = remainderStr;
							temp_remainderStr += the_ItemSet->itemSet[i].lookaheadCh[k];
							firstCh = G.First_of_Str(temp_remainderStr, firstCnt);
							for (int l = 0; l < firstCnt; ++l)
							{
								int lookaheadPos = the_ItemSet->itemSet[itemPos].Lookahead_Exist(firstCh[l]);
								if (lookaheadPos == -1)//��firstԪ�ز�����lookahead������,�����
								{
									changeFlag = 1;
									lookaheadPos = the_ItemSet->itemSet[itemPos].lookaheadChCnt;
									the_ItemSet->itemSet[itemPos].lookaheadCh[lookaheadPos] = firstCh[l];
									++(the_ItemSet->itemSet[itemPos].lookaheadChCnt);//����ĿСβ����+1
								}
							}
						}
						
					}
				}
			}
		}
	}
}

//����ʶ���ķ����л�ǰ׺��LR(1) DFA,������LR(1)��Ŀ���淶�岢����LR(1)��������ΪDFA����Ŀ֮��Ĺ�ϵ�����ڷ������еõ�
void LR::LR1_DFA()
{
	//�Է������ʼ��
	//goto�������ʼ��
	for (int i = 0; i < STATE_NUM; ++i)
	{
		for (int j = 0; j < NONTERMINAL_NUM; ++j)
		{
			gotoTable[i][j] = -1;
		}
	}

	//�ع��ķ�
	//stateNum=0;
	itemSetCollection[stateNum].itemSet[0].productionOrder = G.productionNum;//�ع��ķ�S'->S��һ����ʽ�洢���ķ�����ʽ��������λ��
	itemSetCollection[stateNum].itemSet[0].candidateOrder = 0;//E'->E,�ع��ķ��ĺ�ѡʽ�����Ϊ-1
	//[S'->.S,$]
	itemSetCollection[stateNum].itemSet[0].dotPos = 0;
	itemSetCollection[stateNum].itemSet[0].lookaheadCh[0] = '$';
	itemSetCollection[stateNum].itemSet[0].lookaheadChCnt = 1;
	stateNum = 1;
	//closure([S'->.S,$])
	Closure(&itemSetCollection[0]);//������

	int changeFlag = 1;//�и���
	while (changeFlag == 1)
	{
		changeFlag = 0;
		ITEMSET newItemSet;
		//����ÿһ����Ŀ��I,��ÿһ��״̬
		for (int i = 0; i < stateNum; ++i)
		{
			//����ÿһ���ķ�����x�������ս���ͷ��ս����
			//�ȱ������ս��
			for (int j = 0; j < G.nonTerminalNum; ++j)
			{	//ÿһ���ķ����Ŷ����ܲ���һ���µ���Ŀ��
				newItemSet.itemCnt = 0;

				char x = G.nonTerminal[j].nonTerminal;
				int xExistFlag = 0;//=0��ʾgo��I,X)=�գ�������Ŀ�������ڴ�ת��
				
				//��������Ŀ����ÿһ����Ŀ
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
						newItemSet.itemSet[newItemSet.itemCnt].dotPos = currentDotPos + 1;//.����һλ
						newItemSet.itemSet[newItemSet.itemCnt].lookaheadChCnt = currentLookaheadCnt;
						for (int l = 0; l < currentLookaheadCnt; ++l)
						{
							newItemSet.itemSet[newItemSet.itemCnt].lookaheadCh[l] = itemSetCollection[i].itemSet[k].lookaheadCh[l];
						}
						++newItemSet.itemCnt;
					}
				}
				if (xExistFlag == 1)//go��I, X) != ��
				{
					Closure(&newItemSet);
					int newItemExisted = ItemSet_Exist(newItemSet);
					if (newItemExisted == -1)//�����ڸ���Ŀ���������,���ڷ�����������״̬ת�����
					{
						itemSetCollection[stateNum] = newItemSet;//�Ƿ�Ҫ�Լ����صȺ��������
						gotoTable[i][j] = stateNum;
						++stateNum;
					}
					else//����Ŀ���Ѵ���
					{
						gotoTable[i][j] = newItemExisted;
					}
				}
			}
			//�ٱ����ս��
			for()
		}
	}
}

//����LR(1)������
/*void LR::LR1_Analyze_Table()
{
}*/

//LR(1)��������//�㷨4.3
void LR::LR1_Analyze()
{
	G.Input_Grammer();//�����ķ�
	cout << endl << "��������ķ����£�" << endl;
	G.Output_Grammer();//����ķ�
	G.FIRST_Set();//��FIRST��
	G.FOLLOW_Set();//��FOLLOW��
	G.Output_First_Follow();//���FIRST����FOLLOW��
	
}

//�����������ַ���w$�������뻺����,������ǰָ��ָ��w$�ĵ�һ������
void LR::Input()
{
	char C;
	cout << endl << "����������Ҫ�����ķ��Ŵ�����$��Ϊ���������־��:" << endl;
	cin >> C;
	int i = 0;
	while (C != '$')
	{
		inputBufer[i] = C;
		++i;
		cin >> C;
	}
	inputBufer[i] = '$';
	forwardIp = 0;//��ǰָ��ָ��w$�ĵ�һ������
}

//�ж���Ŀ���Ƿ��Ѿ����ڣ������ڷ���-1�����ڷ�������Ŀ���淶���е����0..(��״̬���)
int LR::ItemSet_Exist(ITEMSET newItemSet)
{
	int existFlag = 0;
	//�����Ѵ��ڵ�ÿһ����Ŀ��
	int i;
	for (i = 0; i < stateNum && existFlag == 0; ++i)
	{
		existFlag = 1;//�������,����Ŀ��ͬ
		if (newItemSet.itemCnt == itemSetCollection[i].itemCnt)//����Ŀ����ͬ�������Ƚ���Ŀ����������
		{
			//�Ƚ�ÿһ����Ŀ�Ƿ�һ��
			existFlag = 1;
			for (int j = 0; j < newItemSet.itemCnt && existFlag == 1; ++j)
			{
				int sameItemPos = itemSetCollection[i].Item_Exist(newItemSet.itemSet[j].productionOrder,
					newItemSet.itemSet[j].candidateOrder, newItemSet.itemSet[j].dotPos);
				//����Ŀ���ڣ���Сβ����Ŀ��ͬ�������Ƚ�Сβ��
				if (sameItemPos > -1
					&& newItemSet.itemSet[j].lookaheadChCnt == itemSetCollection[i].itemSet[sameItemPos].lookaheadChCnt)
				{
					//�Ƚ�ÿһ��Сβ���Ƿ�һ��
					existFlag = 1;
					for (int k = 0; k < newItemSet.itemSet[j].lookaheadChCnt && existFlag == 1; ++k)
					{
						char currenLookahead = newItemSet.itemSet[j].lookaheadCh[k];
						int sameLookaheadPos = itemSetCollection[i].itemSet[sameItemPos].Lookahead_Exist(currenLookahead);
						if (sameLookaheadPos == -1)//Сβ�Ͳ�����
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
	if (existFlag == 1)//����
		return (i - 1);
	else
		return -1;
}

