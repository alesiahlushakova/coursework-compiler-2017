#pragma once
#include "Greibach.h"
#include "LT.h"
#define GRB_ERROR_SERIES 600
#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)

namespace GRB
{
	Greibach greibach(
		NS('S'), TS('$'),                     // стартовый символ, дно стека
		6,									  // количество правил
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // неверная структура программы
			6,                                // кол-во
			Rule::Chain(8, TS(LEX_MAIN), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BACK), NS('E'), TS(LEX_DOT), TS(LEX_BRACELET), TS(LEX_DOT)),
			Rule::Chain(14, TS(LEX_TYPE), TS(LEX_FUNC), TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('F'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BACK), NS('E'), TS(LEX_DOT), TS(LEX_BRACELET), TS(LEX_DOT), NS('S')),
			Rule::Chain(13, TS(LEX_TYPE), TS(LEX_FUNC), TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('F'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS('b'), NS('E'), TS(LEX_DOT), TS(LEX_BRACELET), TS(LEX_DOT)),
			Rule::Chain(10, TS(LEX_STD), TS(LEX_DOT), TS(LEX_MAIN), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BACK), NS('E'), TS(LEX_DOT), TS(LEX_BRACELET), TS(LEX_DOT)),
			Rule::Chain(16, TS(LEX_STD), TS(LEX_DOT), TS(LEX_TYPE), TS(LEX_FUNC), TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('F'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BACK), NS('E'), TS(LEX_DOT), TS(LEX_BRACELET), TS(LEX_DOT), NS('S')),
			Rule::Chain(15, TS(LEX_STD), TS(LEX_DOT), TS(LEX_TYPE), TS(LEX_FUNC), TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('F'), TS(LEX_RIGHTTHESIS), TS(LEX_LEFTBRACE), NS('N'), TS(LEX_BACK), NS('E'), TS(LEX_DOT), TS(LEX_BRACELET), TS(LEX_DOT))
		),
		Rule(
			NS('N'), GRB_ERROR_SERIES + 1,    // конструкции в функциях
			12,                               // кол-во
			Rule::Chain(5, TS(LEX_INIT), TS(LEX_TYPE), TS(LEX_ID), TS(LEX_DOT), NS('N')),
			Rule::Chain(4, TS(LEX_ID), TS(LEX_EQUAL), NS('E'), TS(LEX_DOT)),
			Rule::Chain(5, TS(LEX_ID), TS(LEX_EQUAL), NS('E'), TS(LEX_DOT), NS('N')),
			Rule::Chain(9, TS(LEX_INIT), TS(LEX_TYPE), TS(LEX_FUNC), TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('F'), TS(LEX_RIGHTTHESIS), TS(LEX_DOT), NS('N')),
			Rule::Chain(4, TS(LEX_SHOW), TS(LEX_ID), TS(LEX_DOT), NS('N')),
			Rule::Chain(4, TS(LEX_SHOW), TS(LEX_LITERAL), TS(LEX_DOT), NS('N')),
			Rule::Chain(4, TS(LEX_SHOW), NS('E'), TS(LEX_DOT), NS('N')),
			Rule::Chain(3, TS(LEX_SHOW), NS('E'), TS(LEX_DOT)),
			Rule::Chain(4, TS(LEX_INIT), TS(LEX_TYPE), TS(LEX_ID), TS(LEX_DOT)),
			Rule::Chain(8, TS(LEX_INIT), TS(LEX_TYPE), TS(LEX_FUNC), TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('F'), TS(LEX_RIGHTTHESIS), TS(LEX_DOT)),
			Rule::Chain(3, TS(LEX_SHOW), TS(LEX_ID), TS(LEX_DOT)),
			Rule::Chain(3, TS(LEX_SHOW), TS(LEX_LITERAL), TS(LEX_DOT))
		),
		Rule(
			NS('E'), GRB_ERROR_SERIES + 2,    // ошибка в выражении
			8,                                // 
			Rule::Chain(1, TS(LEX_ID)),
			Rule::Chain(1, TS(LEX_LITERAL)),
			Rule::Chain(3, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS)),
			Rule::Chain(4, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS)),
			Rule::Chain(2, TS(LEX_ID), NS('M')),
			Rule::Chain(2, TS(LEX_LITERAL), NS('M')),
			Rule::Chain(4, TS(LEX_LEFTTHESIS), NS('E'), TS(LEX_RIGHTTHESIS), NS('M')),
			Rule::Chain(5, TS(LEX_ID), TS(LEX_LEFTTHESIS), NS('W'), TS(LEX_RIGHTTHESIS), NS('M'))
		),
		Rule(
			NS('F'), GRB_ERROR_SERIES + 3,    // ошибка в параметрах функции
			2,                                // 
			Rule::Chain(2, TS(LEX_TYPE), TS(LEX_ID)),
			Rule::Chain(4, TS(LEX_TYPE), TS(LEX_ID), TS(LEX_COMMA), NS('F'))
		),
		Rule(
			NS('W'), GRB_ERROR_SERIES + 4,    // ошибка в параметрах вызываемой функции 
			4,                                // 
			Rule::Chain(1, TS(LEX_ID)),
			Rule::Chain(1, TS(LEX_LITERAL)),
			Rule::Chain(3, TS(LEX_ID), TS(LEX_COMMA), NS('W')),
			Rule::Chain(3, TS(LEX_LITERAL), TS(LEX_COMMA), NS('W'))
		),
		Rule(
			NS('M'), GRB_ERROR_SERIES + 5,    // оператор
			2,								  //
			Rule::Chain(2, TS(LEX_OPERATOR), NS('E')),
			Rule::Chain(3, TS(LEX_OPERATOR), NS('E'), NS('M'))
		)
	);
}