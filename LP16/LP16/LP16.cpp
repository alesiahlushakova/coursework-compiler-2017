#include "stdafx.h"
#include <locale>
#include <cwchar>
#include "Div.h"
#include "LexAnaliz.h"
#include "PolishNotation.h"
#include "Semantics.h"
#include "Generation.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		//проверка файла с исходным кодом на допустимость символов
	
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		Lex::LEX lex = Lex::lexAnaliz(log, in);
		Log::WriteLine(log, "Результат работы лексического анализатора - промежуточный код:\n**********************************************************************************************", "");
		LT::showTable(lex.lextable, log);
		IT::showTable(lex.idtable);
		Log::WriteLine(log, "\n Результат работы синтаксического анализатора :\n*******************************************************************************************************", "");
		MFST_TRACE_START(log)
		unsigned int start_time = clock();
		MFST::Mfst mfst(lex, GRB::getGreibach());
		mfst.start(parm.need_trace,log);
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;
		cout <<"Время трассировки "<< search_time << endl;
	    mfst.savededucation();
		mfst.printrules(log);
	bool isSemantic=Semantics(lex.lextable, in, lex.idtable, log);
	if (isSemantic == true)
	{
		cout << "Семантический анализ выполнен без ошибок"<<endl;
	}
	else
	{
		cout << "Семантический анализ выполнен с ошибками" << endl;
	}
		bool rc = startPolishNotation(lex);
		if (rc)
			cout << "Польская запись построена" << endl;
		else
			cout << "Польская запись не построена" << endl;
		log = Log::getlog(parm.out);
		CG::StartGeneration(lex.lextable, lex.idtable, log);

		Log::Close(log);

	}
	catch (Error::ERROR ex)
	{

		cout <<"Ошибка "<<ex.id<<": "<< ex.message;
		Log::WriteError(log, ex);
	}

	system("notepad in.txt.log");
	system("pause");
	return 0;
};
