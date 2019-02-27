// Names.h: interface for the CNames class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__NAMES_H)
#define __NAMES_H

#include <list>
using namespace std;

class CNamesItem{
public:
	CNamesItem();
	~CNamesItem();
	//const CNamesItem &operator=(const CNamesItem &);

	char m_szName[32];

	// maybe we somewhen can extend this class for using personalities, personal NN files, whatever
};

class CNames  
{
public:
	CNames();
	virtual ~CNames();

	bool load(const char *);
	void mixIt(void);
	const CNamesItem *getName(void);

	list<CNamesItem> m_LNames;
	list<CNamesItem>::iterator m_ICName;		// iterator for m_LNames, so you can go thru the names
};

extern CNames g_Names;

#endif // !defined(__NAMES_H)
