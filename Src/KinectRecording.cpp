/************************************************
 * Kinect Recording.cpp
 * 
 * Used to record gestures for machine learning.
 * After the user is calibrated by putting their hands up,
 * a gesture is recorded for 2 seconds. Then there is a five
 * second break. This uses OpenNI and the Kinect is set to 30fps.
 * There are 1080 features recorded. The relative X,Y,Z coordinates (to the torso)
 * of the left and right shoulder, elbow, and hand are
 * recorded into LIBSVM format.
 * 
 * Author: Aaron Pulver <atp1317@rit.edu>
 * 
 * Usage: ./KinectRecording <outputFile.txt> <numberOfSamples> <classLabel>
 * 
 * Note: Requires root access (sudo) to access Kinect
 * ********************************************/



#define POSE_TO_USE "Psi"
#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include <XnPropNames.h>
#include <stdio.h>
#include "GestureRecognizer.h"

#define CLASS_LABEL 1

#define CHECK_RC(nRetVal, what)										\
		if (nRetVal != XN_STATUS_OK)									\
		{																\
			printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));\
			return nRetVal;												\
		}

xn::Context g_Context;
xn::UserGenerator g_UserGenerator;

// Called when a new user is detected
void XN_CALLBACK_TYPE
User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie) {
	printf("New User: %d\n", nId);
	if(nId==1)
	{
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(POSE_TO_USE, nId);
	}
}
// Called when a user was lost
void XN_CALLBACK_TYPE
User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie) {
	printf("Lost User: %d\n", nId);
}

// Called when a pose was detected
void XN_CALLBACK_TYPE
Pose_Detected(xn::PoseDetectionCapability& pose, const XnChar* strPose,
		XnUserID nId, void* pCookie) {
	printf("Pose %s for user %d\n", strPose, nId);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

// Called when callibration was started
void XN_CALLBACK_TYPE
Calibration_Start(xn::SkeletonCapability& capability, XnUserID nId,
		void *pCookie) {
	printf("Starting Calibration for user %d\n", nId);
}

// Called when callibration ended
void XN_CALLBACK_TYPE
Calibration_End(xn::SkeletonCapability& capability, unsigned int nId,
		XnCalibrationStatus status, void *pCookie) {
	if (status == 0) {
		printf("User calibrated\n");
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	} else {
		printf("Failed to calibrate user %d\n", nId);
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(POSE_TO_USE,
				nId);
	}
}

int main(int argc, char* argv[]) {
	// check for file
	if(argc<4)
	{
		printf("Missing arguments\nUsage: ./KinectTraining <outputFile.txt> <numberOfSamples> <classLabel>\n");
		return -1;
	}
	GestureRecognizer gestureRecognizer = GestureRecognizer();
	int frames = 60;
	int sampleSize = 0;
	int numSamples = -1;
	int classLabel = 0;
	sscanf(argv[3], "%d", &classLabel);
	sscanf(argv[2], "%d", &sampleSize);
	XnSkeletonJointPosition head;
	XnSkeletonJointPosition torso;
	XnSkeletonJointPosition leftShoulder;
	XnSkeletonJointPosition rightShoulder;
	XnSkeletonJointPosition leftElbow;
	XnSkeletonJointPosition rightElbow;
	XnSkeletonJointPosition leftHand;
	XnSkeletonJointPosition rightHand;

	FILE* fileOutput;

	printf("Recording %d Samples labeled: %d.\nPress any key to exit gracefully!\n",sampleSize,classLabel);
	XnStatus nRetVal = XN_STATUS_OK;
	xn::Context context;
	nRetVal = context.Init();
	CHECK_RC(nRetVal, "Init");

	nRetVal = g_UserGenerator.Create(context);
	CHECK_RC(nRetVal, "Create");

	//Create and register call backs
	// set up skeleton tracking
	XnCallbackHandle h1, h2, h8, h4;
	g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, h1);
	g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(Pose_Detected,
			NULL, h2);
	g_UserGenerator.GetSkeletonCap().SetSmoothing(.1);
	g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(
			Calibration_Start, NULL, h8);
	g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(
			Calibration_End, NULL, h4);
	g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

	nRetVal = context.StartGeneratingAll();
	CHECK_RC(nRetVal, "Generate");

	// open file
	fileOutput=fopen(argv[1],"w");

	// loop until keyboard hit
	while (!xnOSWasKeyboardHit() && numSamples < sampleSize)
	{
		nRetVal = context.WaitAndUpdateAll();
		CHECK_RC(nRetVal, "WaitAndUpdatAll");
		XnUserID aUsers[5];
		XnUInt16 nUsers = 5;

		nRetVal = g_UserGenerator.GetUsers(aUsers, nUsers);
		CHECK_RC(nRetVal, "GetUsers");

		// for first user get their joint locations and print them
		if (g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[0])) {
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_TORSO, torso);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_LEFT_SHOULDER, leftShoulder);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_LEFT_ELBOW, leftElbow);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_LEFT_HAND, leftHand);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_RIGHT_SHOULDER, rightShoulder);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_RIGHT_ELBOW, rightElbow);
			g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[0], XN_SKEL_RIGHT_HAND, rightHand);

			// get 60 frames (2 seconds of data)
			if(frames==60)
			{
				if(numSamples!=-1)
				{
					gestureRecognizer.PrintFeatures(fileOutput,classLabel);
				}
				numSamples++;
				if(numSamples <sampleSize)
				{
					printf("Resting for 5 seconds... Samples Completed: %d\n",numSamples);
				}
			}
			else if(frames==90)
			{
				printf("4\r");
				fflush(stdout);
			}
			else if(frames==120)
			{
				printf("3\r");
				fflush(stdout);
			}
			else if(frames==150)
			{
				printf("2\r");
				fflush(stdout);
			}
			else if(frames==180)
			{
				printf("1\r");
				fflush(stdout);
			}
			else if(frames==210)
			{
				printf("Perform Gesture!\n");
				frames = -1; // increment at end to get to 0
			}
			gestureRecognizer.UpdateFeatures(torso.position,leftShoulder.position,leftElbow.position,leftHand.position,rightShoulder.position,rightElbow.position,rightHand.position);
			frames++;
		}
	}
	fclose(fileOutput);
	printf("Recording Completed\n");
	context.Release();
	return 0;
}
