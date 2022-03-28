#include "stdafx.h"

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG lg = { NULL,NULL };
		std::ofstream *file = new std::ofstream(logfile);
		if (file->fail())
			throw ERROR_THROW(112);
		lg.stream = file;
		wcscpy_s(lg.logfile, logfile);
		return lg;
	}

	void WriteLine(LOG log, char* c, ...)
	{
		char **ptr = &c;
		int len = 0;
		for (; *ptr != ""; ptr++)
			len += strlen(*ptr);
		ptr = &c;
		char *str = new char[len + 1];
		str[0] = 0;
		for (; *ptr != ""; ptr++)
			strcat(str, *ptr);
		*log.stream << str << std::endl;
		delete str;
	}

	void WriteLine(LOG log, wchar_t* c, ...)
	{
		wchar_t **ptr = &c;
		int len = 0;
		for (; *ptr != L""; ptr++)
			len += wcslen(*ptr);
		ptr = &c;
		char *str = new char[len + 1];
		wchar_t *wstr = new wchar_t[len + 1];
		wstr[0] = 0;
		for (; *ptr != L""; ptr++)
			wcscat(wstr, *ptr);
		wcstombs(str, wstr, len + 1);
		*log.stream << str << std::endl;
		delete str;
		delete wstr;
	}

	void WriteLog(LOG log)
	{
		time_t rawtime;
		struct tm *timeinfo;
		char str[60];			// ������, � ������� ����� ��������� ������� �����                      
		time(&rawtime);			// ������� ���� � ��������
		timeinfo = localtime(&rawtime);		 // ������� ��������� �����, �������������� � ���������
		strftime(str, 60, "****** �������� ****** ����:  %d.%m.%Y %X -----------", timeinfo);		// ����������� ������ �������
		*log.stream << str << std::endl;
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		char buf[PARM_MAX_SIZE];
		*(log.stream) << "****** ��������� **********" << std::endl;
		wcstombs(buf, parm.in, PARM_MAX_SIZE);
		*(log.stream) << "-in:  " << buf << std::endl;
		wcstombs(buf, parm.out, PARM_MAX_SIZE);
		*(log.stream) << "-out: " << buf <<std::endl;
		wcstombs(buf, parm.log, PARM_MAX_SIZE);
		*(log.stream) << "-log: " << buf << std::endl;

	}
	

	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "****** �������� ������ *******" << std::endl;
		*log.stream << "���������� ��������: " << in.size << std::endl;
		*log.stream << "���������������	   : " << in.ignore << std::endl;
		*log.stream << "���������� �����   : " << in.lines << std::endl;
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		if (!log.stream)
		std::cout << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << ", ������� " << error.inext.col << std::endl;
		else if (error.inext.col >= 0 && error.inext.line >= 0)
			*log.stream << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << ", ������� " << error.inext.col << std::endl;
		else
			*log.stream << "������ " << error.id << ": " << error.message << std::endl;
	}

	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
	}
}