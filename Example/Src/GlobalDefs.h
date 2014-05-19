
#ifndef GLOBAL_DEFS_H
#define GLOBAL_DEFS_H

#define CHECK_RC(nRetVal, what)										\
		if (nRetVal != XN_STATUS_OK)									\
		{																\
			printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));\
			return nRetVal;												\
		}

#define PI (3.14159265)
#define RAD_TO_DEG(rad) (rad*57.2957795)

enum Gesture
{
	HANDS_UP = 1,
	RIGHT_HAND_COME = 2,
	RIGHT_HAND_STOP = 3,
	LEFT_HAND_ROTATE = 4,
	NOTHING = 5,
};

#endif
