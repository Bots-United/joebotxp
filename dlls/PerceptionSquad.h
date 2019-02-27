#ifndef __PERCEPTIONSQUAD_H
#define __PERCEPTIONSQUAD_H
#include "Perception.h"

class CPerceptionSquad :
	public CPerception
{
public:
	CPerceptionSquad(void);
	virtual ~CPerceptionSquad(void);

	virtual void run(void);
};

#endif __PERCEPTIONSQUAD_H

