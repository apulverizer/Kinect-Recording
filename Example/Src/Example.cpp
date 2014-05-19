/////////////////////////////////////////////////////////////////////////
// Example.cpp
//
// This is a simple gesture recognizing application. It uses the provided
// model to recognize 4 gestures and a nothing gesture. The four gestures/
// poses are putting both hands in the air over your head, waving towards
// yourself with your right hand, holding your right out like you're trying
// to stop something, and moving your left and in a circular motion.
//
// Author: Aaron Pulver
// Date: 5/19/14
//
////////////////////////////////////////////////////////////////////////

#include "UserTracking.h"
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnPropNames.h>
#include "GlobalDefs.h"

/////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////
xn::Context g_Context;
xn::UserGenerator g_UserGenerator;
UserTracking g_UserTracking;
////////////////////////////////////////////////////////////////////////

// Called when a new user is detected
void XN_CALLBACK_TYPE
User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie) {
	printf("New User: %d\n", nId);
	generator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}
// Called when a user was lost
void XN_CALLBACK_TYPE
User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie) {
	printf("Lost User: %d\n", nId);
	// see if first user was lost. If so, set flag to exit gracefully
	g_UserTracking.removeKinectUser(nId);
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
		capability.StartTracking(nId);
		g_UserTracking.addKinectUser(nId);
	} else {
		printf("Failed to calibrate user %d\n", nId);
		capability.RequestCalibration(nId, TRUE);
	}
}

/**
 * Converts a gesture number to a string
 */
inline const char* GestureToString(Gesture gesture)
				{
	switch (gesture) {
	case HANDS_UP:
		return "Hands Up";
	case RIGHT_HAND_COME:
		return "Come";
	case RIGHT_HAND_STOP:
		return "Stop";
	case LEFT_HAND_ROTATE:
		return "Rotate";
	case NOTHING:
		return "Nothing";
	default:
		return "Nothing";
	}
				}

int main() {
	XnStatus nRetVal = XN_STATUS_OK;
	xn::Context context;
	nRetVal = context.Init();
	CHECK_RC(nRetVal, "Init");
	nRetVal = g_UserGenerator.Create(context);
	CHECK_RC(nRetVal, "Create");

	/////////////////////////////////////////////////////////////////////////////////////////
	// Set up handles to callback functions
	////////////////////////////////////////////////////////////////////////////////////////
	XnCallbackHandle h1, h2, h3;
	g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, h1);
	g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(Calibration_Start, NULL, h2);
	g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(Calibration_End, NULL, h3);
	g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_UPPER);
	g_UserGenerator.GetSkeletonCap().SetSmoothing(.1);
	printf("Connecting to Kinect...\n");
	// Start the Kinect
	nRetVal = context.StartGeneratingAll();
	CHECK_RC(nRetVal, "Generate");
	printf("Welcome to Example 1...Press any key to exit gracefully!\n");
	printf("This example tracks a single person (person who first puts their hands in the air)\n");

        bool bDoOnce = true;

	//////////////////////////////////////////////////////////////////////////////////////////
	// Main loop where all execution will happen
	//////////////////////////////////////////////////////////////////////////////////////////
	while (!xnOSWasKeyboardHit()) {

                    //////////////////////////////////////////////////////////////////////////////////////
                    // Update Kinect/Phone data and users
                    // DO NOT DELETE
                    /////////////////////////////////////////////////////////////////////////////////////
                    nRetVal = context.WaitAndUpdateAll();
                    CHECK_RC(nRetVal, "WaitAndUpdatAll");
                    g_UserTracking.updateAllData(&g_UserGenerator);


                    //////////////////////////////////////////////////////////////////////////////////////
                    // Other logic
                    /////////////////////////////////////////////////////////////////////////////////////
                    if(g_UserTracking.getSelectedUser()==NULL)
                        {
                            g_UserTracking.findUsersByGesture(HANDS_UP);
                        }
                    else
			{
				printf("User %d Performing: %s\n",g_UserTracking.getSelectedUser()->getId(),GestureToString(g_UserTracking.getSelectedUser()->getCurrentGesture()));
			}

	}


	/////////////////////////////////////////////////////////////////////////////////////////
	// Clean up resources
	// DO NOT DELETE
	////////////////////////////////////////////////////////////////////////////////////////
	context.Release();

	return 0;
}
