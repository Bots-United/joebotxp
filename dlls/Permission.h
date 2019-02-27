#ifndef __CPERMISSION_H
#define __CPERMISSION_H

#include <vector>
using namespace std;

class CPermissionItem{
public:
	CPermissionItem(){
		reset();
	}
	void reset(void){
		m_id = 0;
		m_fTimeReserved = 0;
	}
	
	void *m_id;
	float m_fTimeReserved;
};

//
// this class organizes permissions. one permission is stored per slot, and the slot index is the permission index,
// to provide fast lookup. Therefore keep the index low, do _NOT_ use bitfield constants like (1<<20) !
//

class CPermission
{
public:
	CPermission(void);
	virtual ~CPermission(void);

	void reset(void);
	void checkSize(unsigned int);

	bool requestPermission(void *, unsigned int, float);		// ID, type, time to have it --- return true if we can get it
	bool releasePermission(void *, unsigned int);			// ID, type   --- return true if sucessful - otherwise we dont even own this permission

	float getTime(void);
private:
	vector<CPermissionItem> m_LPermissions;
};

#endif __CPERMISSION_H