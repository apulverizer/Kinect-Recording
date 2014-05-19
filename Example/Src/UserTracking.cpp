/*
 * UserTracking.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: apulver
 */

#include "UserTracking.h"

/**
 * Constructor
 */
UserTracking::UserTracking()
{
	m_selectedUserId=0;
}

/**
 * Destructor
 */
UserTracking::~UserTracking()
{
}


/**
 * Get pointer to selected/tracked user
 */
User* UserTracking::getSelectedUser()
{
	if(m_selectedUserId!=0)
	{
		for(int i=0;i<m_users.size();i++)
		{
			if(m_selectedUserId==m_users[i].getId())
			{
				return &m_users[i];
			}
		}
	}
	else
	{
		//TODO
	}
	return NULL;
}

/**
 * Are we tracking someone
 */
bool UserTracking::isTracking()
{
	User* user = getSelectedUser();
	if(user!=NULL)
	{
		return getSelectedUser()->isInFrame();
	}
	else
	{
		return false;
	}
}


/**
 * Update user position
 */
bool UserTracking::updateUserPosition(int id, XnVector3D torso, XnVector3D leftShoulder, XnVector3D rightShoulder, XnVector3D leftElbow,XnVector3D rightElbow, XnVector3D leftHand, XnVector3D rightHand, XnUInt64 newTime)
{
	for(int i=0;i<m_users.size();i++)
	{
		if(m_users[i].getId()==id)
		{
			m_users[i].addPosition(torso,leftShoulder,rightShoulder, leftElbow,rightElbow,leftHand, rightHand, newTime );
			return true;
		}
	}
	return false;
}

bool UserTracking::clearSelectedUser()
{
	m_selectedUserId=0;
	m_userIdsFollowing.clear();
}

/**
 * Find the user doing a gesture and track them
 */
int UserTracking::findUsersByGesture(Gesture gesture)
{
	int found=0;
	m_userIdsFollowing.clear(); // remove users
	for(int i=0;i<m_users.size();i++)
	{
		if((Gesture)m_users[i].getCurrentGesture()==gesture)
		{
			m_userIdsFollowing.push_back(m_users[i].getId());
			found++;
		}
	}
	if(found==1)
	{
		m_selectedUserId=m_userIdsFollowing[0];
	}
	else
	{
		m_selectedUserId=0;
	}
	return found;
}

/**
 * Add new user with Id
 */
bool UserTracking::addKinectUser(int id)
{
	for(int i=0;i<m_users.size();i++)
	{
		if(m_users[i].getId()==id)
		{
			return false;
		}
	}
	m_users.push_back(User(id));
	return true;
}

/**
 * Remove user based on Id
 */
bool UserTracking::removeKinectUser(int id)
{
	for(int i=0;i<m_users.size();i++)
	{
		if(m_users[i].getId()==id)
		{
			if(m_users[i].getId()==m_selectedUserId)
			{
				m_selectedUserId=0;
			}
			m_users.erase(m_users.begin()+i);
			return true;
		}
	}
	return false;
}

/**
 * Updates all of the fields by analyzing the userGenerator
 */
XnStatus UserTracking::updateAllData(xn::UserGenerator* userGenerator)
{
	XnSkeletonJointPosition torso;
	XnSkeletonJointPosition leftShoulder;
	XnSkeletonJointPosition rightShoulder;
	XnSkeletonJointPosition leftElbow;
	XnSkeletonJointPosition rightElbow;
	XnSkeletonJointPosition leftHand;
	XnSkeletonJointPosition rightHand;

	XnUserID aUsers[15];
	XnUInt16 nUsers = 15;

	
	XnStatus nRetVal = userGenerator->GetUsers(aUsers, nUsers);
	CHECK_RC(nRetVal, "GetUsers");
	// for each user get their joint locations and print them
	for(int i= 0; i<nUsers; i++)
	{
		if (userGenerator->GetSkeletonCap().IsTracking(aUsers[i])) {
			userGenerator->GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_TORSO, torso);
			userGenerator->GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_LEFT_SHOULDER, leftShoulder);
			userGenerator->GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_LEFT_ELBOW, leftElbow);
			userGenerator->GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_LEFT_HAND, leftHand);
			userGenerator->GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_RIGHT_SHOULDER, rightShoulder);
			userGenerator->GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_RIGHT_ELBOW, rightElbow);
			userGenerator->GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], XN_SKEL_RIGHT_HAND, rightHand);
			updateUserPosition(aUsers[i],torso.position,leftShoulder.position,rightShoulder.position,leftElbow.position,rightElbow.position,leftHand.position,rightHand.position,userGenerator->GetTimestamp());
		}
	}
	//TODO
	return XN_STATUS_OK;
}

