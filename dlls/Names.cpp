// Names.cpp: implementation of the CNames class.
//
//////////////////////////////////////////////////////////////////////

#include "Names.h"

#include "extdll.h"
#include "util.h"

//#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNames g_Names;

CNamesItem::CNamesItem(){
	*m_szName = 0;
}
CNamesItem::~CNamesItem(){
}

CNames::CNames(){
	m_ICName = m_LNames.begin();
}

CNames::~CNames(){
}

bool CNames::load(const char *szPFileName){
	char szFileName[80];
	FILE *fhd;
	char cmd_line[256],ch;
	int cmd_index;

	UTIL_BuildFileName(szFileName,"joebot",szPFileName);
	if(fhd = fopen(szFileName,"r")){
		while(true){
			cmd_index = 0;
			cmd_line[cmd_index] = 0;
			
			ch = fgetc(fhd);
			
			// skip any leading blanks
			while (ch == ' ')
				ch = fgetc(fhd);
			
			while ((ch != EOF) && (ch != '\r') && (ch != '\n'))
			{
				if (ch == '\t')  // convert tabs to spaces
					ch = ' ';
				
				cmd_line[cmd_index] = ch;
				
				ch = fgetc(fhd);
				
				// skip multiple spaces in input file
				while ((cmd_line[cmd_index] == ' ') &&
					(ch == ' '))      
					ch = fgetc(fhd);
				
				cmd_index++;
			}
			
			if (ch == '\r')  // is it a carriage return?
			{
				ch = fgetc(fhd);  // skip the linefeed
			}
			
			// if reached end of file, then close it
			if (ch == EOF)
			{
				mixIt();
				fclose(fhd);
				
				fhd = NULL;
				return true;
			}
			
			cmd_line[cmd_index] = 0;  // terminate the command line
			
			if(cmd_line[0] != '#'){		// skip comments
				CNamesItem temp;
				
				if(strlen(cmd_line)
					&&strlen(cmd_line) < 32){
					strcpy(temp.m_szName,cmd_line);
					m_LNames.push_back(temp);
				}
			}
		}
	}
	return false;	// to make the compiler happy
}

void CNames::mixIt(void){
	int i1, i2,i;
	list<CNamesItem>::iterator iter1,iter2;
	CNamesItem temp;

	for(i=(int)m_LNames.size()*5; i ; i--){
		i1 = RANDOM_LONG(0,(long)m_LNames.size()-1);
		i2 = RANDOM_LONG(0,(long)m_LNames.size()-1);

		if(i1 == i2)			// no need to swap
			continue;

		iter1 = iter2 = m_LNames.begin();

		for(;i1; i1 --){
			iter1++;
		}
		for(;i2; i2 --){
			iter2++;
		}

		temp = *iter1;			// swap
		*iter1 = *iter2;
		*iter2 = temp;
	}
}

const CNamesItem *CNames::getName(void){
	if(m_LNames.size()){
		m_ICName++;
		if(m_ICName == m_LNames.end()){
			m_ICName = m_LNames.begin();
		}
		return &(*m_ICName);
	}
	else
		return 0;
}