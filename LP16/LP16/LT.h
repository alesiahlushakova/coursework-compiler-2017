#pragma once
#define LEXEMA_FIXSIZE	1			// фиксированный размер лексемы
#define LT_MAXSIZE		4096		// максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX	0xfffffff	// нет элемента таблицы идентификаторов
#define LEX_NUMBER		't'	// лексема для number
#define LEX_STRING		't'	// лексема для string
#define LEX_RC			't' //лексема для rc
#define LEX_TYPE        't'
#define LEX_ID			'i'	// лексема для идентификатора
#define LEX_LITERAL		'l'	// лексема для литерала
#define LEX_FUNC	    'f'	// лексема для func
#define LEX_INIT		'd'	// лексема для declare
#define LEX_BACK		'b'	// лексема для back
#define LEX_SHOW		's'	// лексема для show
#define LEX_MAIN		'm'	// лексема для main
#define LEX_DOT		    '.'	// лексема для .
#define LEX_COMMA		','	// лексема для ,
#define LEX_LEFTBRACE	'<'	// лексема для <
#define LEX_BRACELET	'>'	// лексема для >
#define LEX_LEFTTHESIS	'('	// лексема для (
#define LEX_RIGHTTHESIS	')'	// лексема для )
#define LEX_PLUS		'v'	// лексема для +
#define LEX_MINUS		'v'	// лексема для -
#define LEX_STAR		'v'	// лексема для *
#define LEX_DIRSLASH	'v'	// лексема для /
#define LEX_OPERATOR	'v'	// лексема для операторов
#define LEX_EQUAL		'='
#define LEX_STD			'L'

namespace LT		// таблица лексем
{
	struct Entry	// строка таблицы лексем
	{
		unsigned char lexema;	// лексема
		int sn;							// номер строки в исходном тексте
		int idxTI;						// индекс в таблице идентификаторов или LT_TI_NULLIDX
		int priority;					// приоритет
	};

	struct LexTable						// экземпляр таблицы лексем
	{
		int maxsize;					// емкость таблицы лексем < LT_MAXSIZE
		int size;						// текущий размер таблицы лексем < maxsize
		Entry* table;					// массив строк таблицы лексем
	};

	LexTable Create(		// создать таблицу лексем
		int size			// емкость таблицы лексем < LT_MAXSIZE
	);

	void Add(				// добавить строку в таблицу лексем
		LexTable& lextable,	// экземпляр таблицы лексем
		Entry entry			// строка таблицы лексем
	);

	Entry GetEntry(			// получить строку таблицы лексем
		LexTable& lextable,	// экземпляр таблицы лексем
		int n				// номер получаемой строки
	);

	void Delete(LexTable& lextable);	// удалить таблицу лексем (освободить память)

	Entry writeEntry(					// заполнить строку таблицы лексем
		Entry &entry,
		unsigned char lexema,
		int indx,
		int line
	);

	void showTable(LexTable lextable, Log::LOG &log);	// вывод таблицы лексем
};