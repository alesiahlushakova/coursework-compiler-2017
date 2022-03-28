#include "stdafx.h"
#include "Semantics.h"

bool find = false;
bool checkid = false;
bool checkfunc = false;
void Add(semFunction* x, ListFunc *&List)
{
	ListFunc *temp = new ListFunc;
	temp->symbol = x;
	temp->Next = List->Head;
	List->Head = temp;
}

void ClearList(ListFunc *List)
{
	while (List->Head != NULL)
	{
		ListFunc *temp = List->Head->Next;
		delete List->Head;
		List->Head = temp;
	}
}

void Add(unsigned char* x, ListId *&List)
{
	ListId *temp = new ListId;
	temp->symbol = x;
	temp->Next = List->Head;
	List->Head = temp;
}

void ClearList(ListId *List)
{
	while (List->Head != NULL)
	{
		ListId *temp = List->Head->Next;
		delete List->Head;
		List->Head = temp;
	}
}

bool checkId(ListId *List, unsigned char* f)
{
	ListId *temp = List->Head;
	bool find = false;
	int co = 0;
	while (temp != NULL)
	{
		int k = 0;
		co = 0;
		for (k = 0; *(temp->symbol + k) != '\0' && *(f + k) != '\0'; k++)
		{
			if (*(temp->symbol + k) == *(f + k))
			{
				co++;
			}
		}
		if (co == k)
		{
			find = true;
			break;
		}
		temp = temp->Next;
	}
	return find;
}

bool Compare(semFunction* f1, semFunction* f2)
{
	if (f1->name != f2->name)
		return false;
	//std::cout << "\n\n" << f1->name << "     " << f2->name << "\n";
	if (f1->parmsNumber != f2->parmsNumber)
		return false;
	//std::cout  << f1->number_of_params << "     " << f2->parmsNumber << "\n";
	for (int i = 0; i < f1->parmsNumber; i++)
	{
		//std::cout << f1->parmTypesArray[i] << " <- из списка    " << f2->parmTypesArray[i] << "\n";
		if (f1->parmTypesArray[i] != f2->parmTypesArray[i])
			return false;
	}
	return true;
}

bool checkFunc(ListFunc *MyList, semFunction* f)
{
	ListFunc *temp = MyList->Head;

	while (temp != NULL)
	{
		if (Compare(temp->symbol, f))
		{
			find = true;
			break;
		}
		temp = temp->Next;
	}
	return find;
}

semFunction* CreateFunc(int number, int* arr, unsigned char* name)
{
	semFunction* f = new semFunction();
	f->parmsNumber = number;
	f->parmTypesArray = new int[number];
	for (int i = 0; i < number; i++)
		f->parmTypesArray[i] = arr[i];
	f->name = name;
	return f;
}

bool checkLib(unsigned char* t)
{
	bool z = false;
	unsigned char strlen[] = "strlen";
	unsigned char issubstring[] = "substr";
	unsigned char getArSum[] = "arsum";
	unsigned char getGeomSum[] = "geomsum";
	unsigned char* libFunctions[] = { strlen, issubstring,getArSum,getGeomSum, NULL };

	for (int i = 0; libFunctions[i] != NULL; i++)
	{

		int count = 0;
		int k;
		for (k = 0; *((libFunctions[i]) + k) != '\0' && *(t + k) != '\0'; k++)
		{
			if (*((libFunctions[i]) + k) == *(t + k))
				count++;
		}

		if (count == k)
		{
			z = true;
			break;
		}
	}
	return z;
}

bool  Semantics(LT::LexTable &Lextable, In::IN &in, IT::IdTable &idtable, Log::LOG &log)
{
	bool stlib = false;
	bool mainFlag = false;
	bool errorParam = false;
	bool errorid = false;
	bool formatError = false;
	bool exitParamError = false;
	int params[10];
	ListFunc *Functions = new ListFunc;
	ListId *Ids = new ListId;
	Ids->Head = NULL;
	Functions->Head = NULL;
	unsigned char* name;
	int counter = 0;
	int type = 0;
	bool libError = false;

	for (int i = 0; i < idtable.size; i++)								//проверка на переменную неопознанного типа
	{
		if (idtable.table[i].iddatatype != IT::V && idtable.table[i].iddatatype != IT::F
			&& idtable.table[i].iddatatype != IT::P && idtable.table[i].iddatatype != IT::L
			&& idtable.table[i].iddatatype != IT::OP && idtable.table[i].iddatatype != IT::STD)
			errorid = true;
		if (stlib && idtable.table[i].idtype == IT::STD)
			throw ERROR_THROW(613);
		if (idtable.table[i].idtype == IT::STD)
			stlib = true;
	}

	for (int i = 0; i < Lextable.size; i++)					//проверка на количество main
	{
		if (Lextable.table[i].lexema == LEX_STD)
			stlib = true;
		if (Lextable.table[i].lexema == LEX_MAIN)
			counter++;
	}
	if (counter != 1)
		mainFlag = true;
	counter = 0;

	for (int i = 0; i < Lextable.size; i++)					//проверка типов данных при присваивании
	{
		if (formatError)
			break;
		if (Lextable.table[i].lexema == LEX_EQUAL)
		{
			type = idtable.table[Lextable.table[i - 1].idxTI].iddatatype;
			i++;
			while (Lextable.table[i].lexema != LEX_DOT)
			{
				if (Lextable.table[i].lexema == LEX_ID || Lextable.table[i].lexema == LEX_LITERAL)
				{
					if (idtable.table[Lextable.table[i].idxTI].idtype == IT::F)
					{
						if (idtable.table[Lextable.table[i].idxTI].iddatatype != type)
						{
							formatError = true;
							break;
						}
						while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
							i++;
						continue;
					}
					if (idtable.table[Lextable.table[i].idxTI].iddatatype != type)
					{
						formatError = true;
						break;
					}
				}
				i++;
			}
		}
	}

	for (int i = 0; i < Lextable.size; i++)
	{

		if (Lextable.table[i].lexema == LEX_FUNC)						//ищет все прототипы функций
		{
			int index = Lextable.table[i + 1].idxTI;
			name = idtable.table[index].id;
			int k = i + 2;
			counter = 0;
			while (Lextable.table[k].lexema != LEX_RIGHTTHESIS)
			{
				if (Lextable.table[k].lexema != LEX_COMMA && Lextable.table[k].lexema != LEX_LEFTTHESIS)
				{
					index = Lextable.table[k + 1].idxTI;
					params[counter] = idtable.table[index].iddatatype;
					counter++;
					k++;
				}
				k++;
			}
			semFunction* f = CreateFunc(counter, params, name);					//создаем структуру с описанием прототипа функции
			Add(f, Functions);
		}
		if (Lextable.table[i].lexema == LEX_OPERATOR)
		{
			if (!strcmp((char*)idtable.table[Lextable.table[i].idxTI].id, "/"))// проверка делени€ на ноль
			{
				if (Lextable.table[i + 1].lexema != LEX_LEFTTHESIS)
				{
					if (idtable.table[Lextable.table[i + 1].idxTI].value.vint == 0)
					{
						throw ERROR_THROW(615);
					}
				}
			}

			for (int j = i; Lextable.table[j].lexema != LEX_DOT; j++)
			{
				if (Lextable.table[j].idxTI < idtable.size)
				{
					if (idtable.table[Lextable.table[j].idxTI].iddatatype != IT::INT && idtable.table[Lextable.table[j].idxTI].idtype != IT::P && idtable.table[Lextable.table[j].idxTI].idtype != IT::OP)
						throw ERROR_THROW(616);
				}
			}
		}
	}

	int level;
	int typeInit;
	int typeExit;
	for (int i = 0; i < Lextable.size; i++)					//проверка возвращаемого значени€ функции
	{
		if (Lextable.table[i].lexema == LEX_FUNC && !stlib && checkLib(idtable.table[Lextable.table[i + 1].idxTI].id))
			libError = true;
		if (Lextable.table[i].lexema == LEX_FUNC && !(stlib && checkLib(idtable.table[Lextable.table[i + 1].idxTI].id)))
		{																		//проверка функций Ќ≈ стандартной библиотеки
			typeInit = idtable.table[Lextable.table[i + 1].idxTI].iddatatype;	//тип при инициализации
			int k = i;
			level = 0;
			while (Lextable.table[k].lexema != LEX_BACK /*&& level == 0*/)
			{

				k++;
			}

			typeExit = idtable.table[Lextable.table[k + 1].idxTI].iddatatype;	//тип возвращаемого значени€

			if (typeInit != typeExit)
			{
				exitParamError = true;
				break;
			}
		}
		else
		{
			if (Lextable.table[i].lexema == LEX_FUNC && (stlib && checkLib(idtable.table[Lextable.table[i + 1].idxTI].id)))	//если полключена библиотека и 
			{																	//провер€ем стандартную функцию
				unsigned char strlen[] = "strlen";
				unsigned char issubstring[] = "substr";
				unsigned char getArSum[] = "arsum";
				unsigned char getGeomSum[] = "geomsum";
				unsigned char* libFunctions[] = { strlen, issubstring,getArSum,getGeomSum, NULL };
				bool find = false;
				int id = -1;
				for (int i3 = 0; libFunctions[i3] != NULL; i3++)
				{
					int count = 0;
					int k;
					for (k = 0; *((libFunctions[i3]) + k) != '\0' && *(idtable.table[Lextable.table[i + 1].idxTI].id + k) != '\0'; k++)
					{
						if (*((libFunctions[i3]) + k) == *(idtable.table[Lextable.table[i + 1].idxTI].id + k))
							count++;
					}
					if (count == k)
					{
						find = true;
						id = i3;
						break;
					}
				}
				if (find)
				{

					switch (id)
					{
					case 0:								//strlen
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::INT)
							exitParamError = true;
						if (idtable.table[Lextable.table[i + 4].idxTI].iddatatype != IT::STR)
							libError = true;
						break;

					case 1:								//issubstring
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::RC)
							exitParamError = true;
						if (idtable.table[Lextable.table[i + 4].idxTI].iddatatype != IT::STR || idtable.table[Lextable.table[i + 7].idxTI].iddatatype != IT::STR)
							libError = true;
						break;
					case 2:								//getArSum
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::INT)
							exitParamError = true;
						if (idtable.table[Lextable.table[i + 4].idxTI].iddatatype != IT::INT || idtable.table[Lextable.table[i + 7].idxTI].iddatatype != IT::INT || 
							idtable.table[Lextable.table[i + 10].idxTI].iddatatype != IT::INT)
							libError = true;
						break;
					case 3:								//getGeomSum
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::INT)
							exitParamError = true;
						if (idtable.table[Lextable.table[i + 4].idxTI].iddatatype != IT::INT || idtable.table[Lextable.table[i + 7].idxTI].iddatatype != IT::INT || 
							idtable.table[Lextable.table[i + 10].idxTI].iddatatype != IT::INT)
							libError = true;
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < Lextable.size; i++)					//проверка использовани€ необъ€вленной переменной
	{
		if (Lextable.table[i].lexema == LEX_INIT)									//заполн€ем лист объ€вленными идентификаторами
		{
			if (Lextable.table[i + 2].lexema != LEX_FUNC)
			{
				Add(idtable.table[Lextable.table[i + 2].idxTI].id, Ids);
			}
		}

		if (Lextable.table[i].lexema == LEX_FUNC)
		{
			i++;			//сдвиг на название функции
			i++;			//сдвиг на (
			while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
			{
				if (Lextable.table[i].lexema == LEX_ID)
					Add(idtable.table[Lextable.table[i].idxTI].id, Ids);
				i++;
			}
		}

		int count;
		if (Lextable.table[i].idxTI < idtable.size)
		{
			if (idtable.table[Lextable.table[i].idxTI].idtype == IT::F)
			{
				if (Lextable.table[i - 1].lexema != LEX_FUNC)		//если это не объ€вление функции
				{
					name = idtable.table[Lextable.table[i].idxTI].id;
					i++;
					count = 0;
					//std::cout << "\n\n" << name << "\n";
					while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
					{
						if (Lextable.table[i].idxTI < idtable.size)		//если идентификатор
						{


							params[count] = idtable.table[Lextable.table[i].idxTI].iddatatype;
							count++;
							

						}
						i++;
					}
					semFunction* fcheck = CreateFunc(count, params, name);

					if (!checkFunc(Functions, fcheck))
					{
						checkfunc = true;

					}
				}
			}
		}
	}


	for (int i = 0; i < idtable.size; i++)
	{
		if (idtable.table[i].idtype != IT::OP && idtable.table[i].idtype != IT::STD && idtable.table[i].idtype != IT::F && idtable.table[i].idtype != IT::L && idtable.table[i].idtype != IT::F && idtable.table[i].idtype != IT::P)
		{
			if (!(checkId(Ids, idtable.table[i].id)))
			{
				checkid = true;
			}
		}
	
	}


	ClearList(Ids);



	if (errorParam)								//проверка параметров, кол-ва и типов
		throw ERROR_THROW(606);
	if (!find || libError)
		throw ERROR_THROW(610);
	if (exitParamError)							//проверка типа выходного параметра
		throw ERROR_THROW(609);
	if (errorid)								//наличие переменных неопределенного типа в программе, 
		throw ERROR_THROW(605);															// ошибки в объ€влении функции
	if (formatError)							//проверка на совпадение типов данных при присваивании переменной
		throw ERROR_THROW(607);
	if (checkid)
		throw ERROR_THROW(611);
	if (checkfunc)
		throw ERROR_THROW(612);
	return true;
}