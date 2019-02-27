#ifndef __PATHSTAT_H
#define __PATHSTAT_H

class CPathStat
{
public:
	CPathStat(void);
	~CPathStat(void);

	void reset(void);

	void save(FILE *fhd);
	void load(FILE *fhd);

	int m_iFailed;
	int m_iWorldDamage;		// damage done by world, e.g. via falling somewhere down
};

#endif
