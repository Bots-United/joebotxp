// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__LOG_H)
#define __LOG_H

class CLog  
{
public:
	CLog(const char *);
	virtual ~CLog();

	void process(void);

	float m_fTime;
	char m_szFilename[80];
};

extern CLog g_Log;

#endif // !defined(__LOG_H)
