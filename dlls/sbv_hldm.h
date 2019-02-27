#ifndef  __SBV_HLDM
#define  __SBV_HLDM

#include "Behaviour.h"
#include "BaseSquad.h"

class CSBehaviour:public CBehaviour{
public:
	CSBehaviour(CBaseSquad *pSquad):CBehaviour(0){m_pSquad = pSquad;}

	CBaseSquad *m_pSquad;
};

class CSBVNP_HLDM_RunAround:public CSBehaviour{
public:
	CSBVNP_HLDM_RunAround(CBaseSquad *pSquad):CSBehaviour(pSquad)
		{strcpy(m_szName,"CSBVNP_HLDM_RunAround");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
	virtual void initialize(CGoal *);
};

class CSBVNP_HLDM_Cohesion:public CSBehaviour{
public:
	CSBVNP_HLDM_Cohesion(CBaseSquad *pSquad):CSBehaviour(pSquad)
		{strcpy(m_szName,"CSBVNP_HLDM_Cohesion");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CSBVNP_HLDM_Avoidance:public CSBehaviour{
public:
	CSBVNP_HLDM_Avoidance(CBaseSquad *pSquad):CSBehaviour(pSquad)
		{strcpy(m_szName,"CSBVNP_HLDM_Avoidance");
		percept(CBehaviour::P_NONE);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void execute(CGoal *);
};

class CSBVNP_CS_GotoBombSite:public CSBehaviour{
public:
	CSBVNP_CS_GotoBombSite(CBaseSquad *pSquad):CSBehaviour(pSquad)
		{strcpy(m_szName,"CSBVNP_CS_GotoBombSite");
		percept(CBehaviour::P_NONE);
		m_iBombsiteWP = -1;}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void initialize(CGoal *);
	virtual void execute(CGoal *);

	virtual void reset(void){m_iBombsiteWP = -1;}

	int m_iBombsiteWP;
};


class CSBV_HLDM_AttackDirect:public CSBehaviour{
public:
	CSBV_HLDM_AttackDirect(CBaseSquad *pSquad):CSBehaviour(pSquad)
		{strcpy(m_szName,"CSBV_HLDM_AttackDirect");
		percept(CBehaviour::P_STMem);}

	virtual void evaluate(list<CGoal> *,CPerceipt* p=0);
	virtual void initialize(CGoal *);
	virtual void execute(CGoal *);
};

#endif //__SBV_HLDM