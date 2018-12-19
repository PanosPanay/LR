#include "LR.h"

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

//��һ���ַ�����FIRST��
string GRAMMER::First_of_Str(string s)
{
	//δ��ɣ�
	//ĩβ��'\0'
	return string();
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



//LR����
//------------------------------------------------------------------------------------------------
//Ĭ�Ϲ��캯��
LR::LR()
{
	//����Ŀ������Ŀ����ʼ��Ϊ������־"\0"
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

//�Ƚ�2����Ŀ���Ƿ���ͬ����ͬ����1����ͬ����0������ͨ���и����ʽ�ͺ�׺
int LR::CMP_ItemSet(string * itemSet1, string * itemSet2)
{
	//δ���
	return 1;
}

void LR::Closure(string * itemSet)//itemSetΪһ��string����,"\0"���ս�
{
	int changeFlag = 1;//���ڸĶ�
	while (changeFlag == 1)
	{
		for (int i = 0; *(itemSet + i) != "\0"; ++i)//ÿ����ĿA->a.Bw,a|b...
		{
			int j = 0;
			while ((*(itemSet + i))[j] != '.')
			{
				++j;
			}
			++j;//��.֮��һλ�ַ�
			/*if ((*(itemSet + i))[j] == ',')//��Լ���������
			{
			}*/
			char leftP = (*(itemSet + i))[j];
			if (leftP != ',' && G.isNonTerminal(leftP) != -1)
			{
				int theProduction = G.isNonTerminal(leftP);
				for (int num = 0; num < G.productionList[theProduction].candidateNum; ++num)//�ķ�G��ÿ������ʽB->...
				{
					//����ò���ʽ������
					//δд
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
						string firstSet = G.First_of_Str(remainderS);//first��

					}
				}
			}
		}
	}
}

//����ʶ���ķ����л�ǰ׺��LR(1) DFA,������LR(1)��Ŀ���淶�岢����LR(1)��������ΪDFA����Ŀ֮��Ĺ�ϵ�����ڷ������еõ�
void LR::LR1_DFA()
{

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
