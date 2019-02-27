#ifndef __PERCEPTIONBOT_H
#define __PERCEPTIONBOT_H
#include "Perception.h"

#define _ENT_PERC_RADIUS 1000

class CPerceptionBot :
	public CPerception
{
public:
	CPerceptionBot(void);
	virtual ~CPerceptionBot(void);

	virtual void run(void);

	virtual void perceptPlayers(void);
	virtual void mergePlayerPercs(void);
	virtual void perceptEntities(void);
	virtual void perceptObstacles(void);

	virtual void perceptSoundOf(CPlayer *);
	virtual void perceptWalkingOf(CPlayer *,bool);
	virtual void perceptShootingOf(CPlayer *,bool);

	virtual void reset(void);

	void setBot(CBaseBot *p){m_pBot = p;}
	CBaseBot *getBot(void){return m_pBot;}
	CBaseBot *m_pBot;

	enum obstacle_heights{
		POD_MJump = 0,
		POD_Origin,
		POD_Eye,
		POD_Max
	};
	bool m_pbObstacle[POD_Max];						// are there somewhere obstacles
	TraceResult m_pTObstacle[POD_Max];
	float m_fNextPerceptEntities;

	// array of player perceptions
	CPerceipt m_pPlayerPercs[32];
};

#endif __PERCEPTIONBOT_H