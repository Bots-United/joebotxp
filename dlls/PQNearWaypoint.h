// PQNearWaypoint.h: interface for the CPQNearWaypoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PQNEARWAYPOINT_H__7B9BC322_755E_4D22_881A_B3A1DAB7FECF__INCLUDED_)
#define AFX_PQNEARWAYPOINT_H__7B9BC322_755E_4D22_881A_B3A1DAB7FECF__INCLUDED_

class CPQINearWaypoint  
{
public:
	CPQINearWaypoint();
	virtual ~CPQINearWaypoint();
	bool operator < (const CPQINearWaypoint &);
	bool operator > (const CPQINearWaypoint &);

	int m_iWPIndex;
	float m_fDistance;
};

#endif // !defined(AFX_PQNEARWAYPOINT_H__7B9BC322_755E_4D22_881A_B3A1DAB7FECF__INCLUDED_)
