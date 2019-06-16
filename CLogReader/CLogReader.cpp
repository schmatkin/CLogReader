#include "CLogReader.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#pragma warning(disable : 4996)//���������� ����� � borland c++ builder ������� �� ���� ������������,
                               //��������� fopen_s � strcpy_s ��� visual

CLogReader::CLogReader()
{
	file_ = NULL;
	filterStr = NULL;
	lastLine = false;
	posInFile = 0;
}

CLogReader::~CLogReader()
{
	free(filterStr);
	Close();
}


bool CLogReader::Open(const char* path)
{

	if (path)
	{
		file_ = fopen(path, "rb");

	}

	return file_;


}


void CLogReader::Close(void)
{
	if (file_)
	{
		fclose(file_);
		file_ = NULL;
	}
}

bool CLogReader::SetFilter(const char* filter)
{
	bool result;

	if (filter != NULL) {
		size_t length = strlen(filter) + 1;
		filterStr = static_cast<char*>(malloc(length + 1));

		if (filterStr != NULL) {
			strcpy(filterStr, filter);
			filterStr[length - 1] = '\0';

			result = true;
		}
		else {
			result = false;
		}
	}
	else {
		result = false;
	}

	return result;


}

bool CLogReader::GetNextLine(char* buf, const int bufsize)
{


	bool result;
	if (lastLine) { return false; } //���������� ��������� ������
	fseek(file_, posInFile, SEEK_SET);

	int s = 0;
	while (1) {

		fgets(buf, bufsize, file_);
		posInFile = ftell(file_);
		if (compareStrings(filterStr, buf)) {

			if (feof(file_)) lastLine = true;//���� �������� ������ ���������
			result = true;

			break;
		}

		if (feof(file_)) { result = false;  break; }
		if (ferror(file_)) { result = false; break; }

	}




	return result;
}
int CLogReader::compareStrings(const char* filter, const char* string)
{
	const char* s, * w;

	s = string;
	w = filter;

	int sovp = 0;
	int p = 0;
	int k = 0;
	bool ksovp = true;//��� ����������� �������� ���� abc*

	while (*s)
	{

		if (*w == *s) {
			sovp++;
			s++;
			k++;
			w++;
			if (k == 1) ksovp = false;
			if (!*s && *w) return 0;
			if (!*w) {
				if (*s) {
					if ((int(*s) == 13 || int(*s) == 10)) return 1; //���� LF ��� CR
					for (int i = 0; i < sovp + 2; i++)
						*w--;
					sovp = 0;
					continue;
				}
				return 1;
			}
		} //if(*w==*s)
		else {
			if (*w == '*') {
				ksovp = true;
				w++;
				if (!*w) return 1;
				else if (*w == '*') {
					sovp = 0;
					continue;
				}

				else {
					while (*s != *w)
					{
						s++;
						k++;

						if (!*s) return 0;
					}//while

				}

				sovp = 0;
				p++;
				s++;
				k++;
				w++;
			} //if '*'
			else if (*w == '?') {
				s++;
				k++;
				w++;
				if ((!*s || (int(*s) == 13 || int(*s) == 10)) && !*w) return 1;//���� ��������� ? � ������ �� �� ���������, ���� ������� ������
				ksovp = true;
			}//else if(*w=='?')
			else if (ksovp == false) {
				ksovp = true;
				return 0;
			}
			else {
				ksovp = true;
				if (sovp != 0) {  //���� ����� � ����������� �������
					for (int i = 0; i < sovp + 2; i++) *w--;
					sovp = 0;
					continue;
				}
				if (p != 0)    //���� ����� ������ ������ ���� ������
				{
					w--;
					w--;
					p = 0;
					continue;
				}
				return 0;
			}
			if (!*s && *w) return 0; // ���� �� ����� �������� �����, � � ������ ��� ���� ������, ������������
								 // �� ������ �������� �����, �� �������� ���� � ��� ����������

		}//else
	}
	/*Form1->Memo1->Lines->Add(*w);
	Form1->Memo1->Lines->Add(n);*/
	//Form1->Memo1->Lines->Add(IntToStr(m)+" !!!!!!!!!!!!!!!!!");
	// "D:\\history 10000.txt" "?sq*urtyry??"
}