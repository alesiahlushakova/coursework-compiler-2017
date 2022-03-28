#include "stdafx.h"
#include "Div.h"
#include "Graphs.h"
#include "LexAnaliz.h"

#define DIV '|'
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'
#define EQUAL '='

using namespace std;

namespace Lex
{
	bool flag2 = false;
	bool flag1 = false;
	LEX lexAnaliz(Log::LOG log, In::IN in)
	{
		LEX lex;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);
		clearSpace(in.text, in.size);

		unsigned char** word = new unsigned char*[max_world];
		for (int i = 0; i < max_world; i++)
			word[i] = new unsigned char[size_world] {NULL};

		do {
			word = divideText(in.text, in.size);
		} while (word == NULL);

		int indxLex = 0;		// индекс лексемы
		int indexID = 0;		// индекс идент.
		int clit = 1;			// счетчик литералов
		int line = 1;			// номер строки
		int position = 0;			// номер позиции в исходном файле
		int leftbr = 0;
		int rightbr=0;

		unsigned char emptystr[] = "";	
		unsigned char* prefix = new unsigned char[10]{ "" };	// текущий префикс
		unsigned char* bufprefix = new unsigned char[10]{ "" };	// буфер для префикса
		unsigned char* oldprefix = new unsigned char[10]{ "" };	// предыдущий префикс
		unsigned char* L = new unsigned char[2]{ "L" };
		unsigned char* bufL = new unsigned char[TI_STR_MAXSIZE];
		unsigned char* nameLiteral = new unsigned char[10]{ "" };
		char* charclit = new char[10]{ "" };		// для строковой записи счетчика

		bool findFunc = false;
		bool findParm = false;
		bool findLib = false;
		IT::Entry entryIT;

		for (int i = 0; word[i] != NULL; i++, indxLex++)
		{
			if (word[i][0] == (unsigned char)'?')
			{
				i++;
				while (word[i][0] != (unsigned char)'?' && (word[i + 1] != NULL))
					i++;
				if (word[i + 1] == NULL)
					throw ERROR_THROW(163);
				continue;
			}

			bool findSameID = false;

			FST::FST fstInit(word[i], FST_INIT);
			if (FST::execute(fstInit))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_INIT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstTypeNumber(word[i], FST_NUMBER);
			if (FST::execute(fstTypeNumber))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_NUMBER, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				entryIT.iddatatype = IT::INT;
				continue;
			}
			FST::FST fstTypeRc(word[i], FST_RC);
			if (FST::execute(fstTypeRc))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RC, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				entryIT.iddatatype = IT::RC;
				continue;
			}
			FST::FST fstTypeString(word[i], FST_STRING);
			if (FST::execute(fstTypeString))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				entryIT.iddatatype = IT::STR;
				_mbscpy(entryIT.value.vstr.str, emptystr);
				continue;
			}
			FST::FST fstFunction(word[i], FST_FUNC);
			if (FST::execute(fstFunction))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_FUNC, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::F;
				findFunc = true;
				continue;
			}
			FST::FST fstLibrary(word[i], FST_STD);
			if (FST::execute(fstLibrary))
			{
				findLib = true;
				LT::Entry entryLT = writeEntry(entryLT, LEX_STD, indexID++, line);
				entryIT.iddatatype = IT::INT;
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = indxLex;
				entryIT.idtype = IT::STD;
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstBack(word[i], FST_BACK);
			if (FST::execute(fstBack))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BACK, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstShow(word[i], FST_SHOW);
			if (FST::execute(fstShow))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_SHOW, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstMain(word[i], FST_MAIN);
			if (FST::execute(fstMain))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_MAIN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				_mbscpy(oldprefix, prefix);
				_mbscpy(prefix, word[i]);
				_mbscpy(entryIT.visibility, emptystr);
				continue;
			}
			FST::FST fstLiteralIs(word[i], FST_RCLIT);					//rc
			if (FST::execute(fstLiteralIs))
			{
				bool value = false;
				if (word[i][0] == 't')
					value = true;
				// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vrc == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::RC)
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}
				if (findSameID)	// если был найден такой же
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::RC;
				entryIT.value.vrc = value;
				entryIT.idxfirstLE = indxLex;						// дальше формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)
				_mbscpy(bufL, L);									// помещаем в буфер "L"
				word[i] = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstIdentif(word[i], FST_ID);
			if (FST::execute(fstIdentif))
			{
				// поиск такого же
				if (findFunc)	// если функция
				{
					int idx = IT::IsId(idtable, word[i]);	// ищем без префикса
					if (idx != TI_NULLIDX)		// если такой идентификатор уже есть
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
				}
				else
				{
					int idx = IT::IsId(idtable, word[i]);	// ищем без префикса, а потом с префиксом
					if (idx != TI_NULLIDX)		// если такой идентификатор уже есть
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
					_mbscpy(bufprefix, prefix);
					word[i] = _mbscat(bufprefix, word[i]);
					idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX)		// если такой идентификатор уже есть
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						continue;
					}
				}
				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);
				LT::Add(lextable, entryLT);
				if (findParm) {		// если параметр
					entryIT.idtype = IT::P;
					_mbscpy(entryIT.visibility, prefix);
				}
				else if (!findFunc) {	// если переменная
					entryIT.idtype = IT::V;
					_mbscpy(entryIT.visibility, prefix);
					if (entryIT.iddatatype == IT::INT)
						entryIT.value.vint = TI_INT_DEFAULT;
					if (entryIT.iddatatype == IT::RC)
						entryIT.value.vrc = TI_RC_DEFAULT;
					if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
				}
				else {		// если функция
					_mbscpy(oldprefix, prefix);
					_mbscpy(prefix, word[i]);
					_mbscpy(entryIT.visibility, emptystr);
				}

				entryIT.idxfirstLE = indxLex;
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				findFunc = false;
				continue;
			}
			FST::FST fstLiteralInt(word[i], FST_INTLIT);
			if (FST::execute(fstLiteralInt))
			{
				int value = atoi((char*)word[i]);	// значение литерала
													// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::INT)
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}
				if (findSameID)	// если был найден такой же
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)
				_mbscpy(bufL, L);	// помещаем в буфер "L"
				word[i] = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstLiteralString(word[i], FST_STRLIT);
			if (FST::execute(fstLiteralString))
			{
				// запись значения
				int length = _mbslen(word[i]);
				for (int k = 0; k < length; k++)	// перезапись массива, убираем кавычки
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;
				// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (!(_mbscmp(idtable.table[k].value.vstr.str, word[i])))
					{
						findSameID = true;
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID)	// если был найден такой же
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.value.vstr.str, word[i]);	// запись значиния строкового литерала
				entryIT.value.vstr.len = length - 2;		// запись длины строкового литерала
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)
				_mbscpy(bufL, L);	// помещаем в буфер "L"
				nameLiteral = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала (L + charclit)
				_mbscpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstOperator(word[i], FST_OPERATOR);
			if (FST::execute(fstOperator))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, indexID++, line);
				switch (word[i][0])
				{
				case PLUS: case MINUS:
					entryLT.priority = 2;
					break;
				case DIRSLASH: case STAR:
					entryLT.priority = 3;
					break;
				}
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = indxLex;
				entryIT.idtype = IT::OP;
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstDot(word[i], FST_DOT);
			if (FST::execute(fstDot))
			{
				if( rightbr!= leftbr)
					throw ERROR_THROW_IN(162, line, position);
				LT::Entry entryLT = writeEntry(entryLT, LEX_DOT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstComma(word[i], FST_COMMA);
			if (FST::execute(fstComma))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);
				entryLT.priority = 1;
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftBrace(word[i], FST_LEFTBRACE);
			if (FST::execute(fstLeftBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstRightBrace(word[i], FST_BRACELET);
			if (FST::execute(fstRightBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftThesis(word[i], FST_LEFTTHESIS);
			if (FST::execute(fstLeftThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);
				leftbr++;
				entryLT.priority = 0;
				LT::Add(lextable, entryLT);
				if (idtable.table[indexID - 1].idtype == IT::F)
					findParm = true;
				continue;
			}
			FST::FST fstRightThesis(word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				rightbr++;
				LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				if (findParm && word[i + 1][0] != LEX_LEFTBRACE && word[i + 2][0] != LEX_LEFTBRACE)		// если после функции нет {
					_mbscpy(prefix, oldprefix);		// возвращаем предыдущую обл. видимости
				findParm = false;
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstEqual(word[i], FST_EQUAL);
			if (FST::execute(fstEqual))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			position += _mbslen(word[i]);
			if (word[i][0] == DIV) {
				line++;
				position = 0;
				indxLex--;
				continue;
			}
			throw ERROR_THROW_IN(162, line, position);
		}
		lex.idtable = idtable;
		lex.lextable = lextable;
		return lex;
	}
}