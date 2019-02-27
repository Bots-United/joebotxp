// Emotion.h: interface for the CEmotion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__EMOTION_H)
#define __EMOTION_H

#include "Behaviour.h"

class CEmotion : public CBehaviour			// inherits normal behaviour class
{
public:
	CEmotion(CBaseBot *pBot):CBehaviour(pBot){}
	virtual ~CEmotion();

	virtual void preprocess(void);				// This is called before this emotion is feeded with perceptions
	virtual void postprocess(void);				// This is called after this emotion is feeded with perceptions
};

#endif // !defined(__EMOTION_H)
