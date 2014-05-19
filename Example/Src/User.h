/////////////////////////////////////////////////////////////////
// User.h
//
// This is a class which stores the joint positions of a user
// and performs basic functions such as finding the angle,
// direction, and any gestures being performed.
//
// Author: Aaron Pulver
// Date: 5/18/14
///////////////////////////////////////////////////////////////

#ifndef USER_h
#define USER_h

#include <deque>
#include <XnOpenNI.h>
#include <cmath>
#include "GlobalDefs.h"
#include "../../Src/GestureRecognizer.h"

class User
{
private:
	static const int QUEUE_SIZE = 60;
	static const int MIN_VARIANCE = 75;
	static const int THRESHOLD_SIZE = 30;
	GestureRecognizer gestureRecognizer;

	/*
	 * The unique id of the user [1-15]
	 */
    int id;
    /**
     * A queue of positions
     */
    std::deque<XnVector3D> torsoPositions;

public:
  User(int id);
  User(const User& other);
  User();
  ~User();

  int getId() const;
  void setId(int id);
  void addPosition(XnVector3D torso, XnVector3D leftShoulder, XnVector3D rightShoulder, XnVector3D leftElbow, XnVector3D rightElbow, XnVector3D leftHand, XnVector3D rightHand , XnUInt64 newTime);
  XnVector3D getCurrentPosition();
  XnFloat getCurrentAngle();
  XnFloat getCurrentDirection(int framesBack);
  bool isInFrame();
  Gesture getCurrentGesture();
  XnFloat getCurrentDistance();

  bool operator== (const User &other) const;
  bool operator!= (const User &other) const;
  User operator=(const User & rhs);
  
};
  
#endif
  
