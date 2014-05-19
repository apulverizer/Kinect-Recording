/////////////////////////////////////////////////////////////////
// UserTracking.h
//
// This is a class which stores users and can select a specific
// user based on a gesture.
//
// Author: Aaron Pulver
// Date: 5/18/14
///////////////////////////////////////////////////////////////

#ifndef USERTRACKING_H_
#define USERTRACKING_H_

#include <XnOpenNI.h>
#include <vector>
#include <stdio.h>
#include "User.h"
#include "GlobalDefs.h"

class UserTracking
{
private:
	std::vector<User> m_users;
	std::vector<int> m_userIdsFollowing;
	int m_selectedUserId;
	bool updateUserPosition(int id, XnVector3D torso, XnVector3D leftShoulder, XnVector3D rightShoulder, XnVector3D leftElbow,XnVector3D rightElbow, XnVector3D leftHand, XnVector3D rightHand, XnUInt64 newTime);
	public:
	UserTracking();
	~UserTracking();
	
	/** Debug *************************/
	void printSelectedUsers();
	
	
	/** Kinect User Tracking******************/
	int findUsersByGesture(Gesture gesture);
	User* getSelectedUser();
	bool isTracking();
	bool addKinectUser(int id);
	bool removeKinectUser(int id);
	bool clearSelectedUser();
	
	XnStatus updateAllData(xn::UserGenerator * userGenerator);

};


#endif /* USERTRACKING_H_ */
