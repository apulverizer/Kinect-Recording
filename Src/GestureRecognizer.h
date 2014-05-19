/*
 * Author: Aaron Pulver
 *
 * A class to recognize gestures using LIBSVM
 */

#ifndef GESTURE_RECOGNIZER_H
#define GESTURE_RECOGNIZER_H
#include "svm.h"
#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
/*************************************************
 * GestureRecognizer.h
 * 
 * Used to classify gestures in real-time using OpenNI and LIBSVM
 * 
 * Uses an array of 1080 features which are updated with each new frame of 
 * data from the Kinect.
 * 
 * Author: Aaron Pulver <atp1317@rit.edu>
 * **************************************************************************/

#include <XnPropNames.h>
#include <stdio.h>
#include <deque>


class GestureRecognizer
{
private:
	struct svm_node *svmVec; // The array of features
	struct svm_model *svmModel; // The model which is loaded
	int numberOfFrames; // number of frames 
	std::deque<int> gestures;

	void ShiftDataDown();
	XnVector3D RelativeToJoint(XnVector3D main, XnVector3D other);

public:
	int Classify();
	void PrintFeatures(FILE* file, int classLabel);
	void UpdateFeatures(XnVector3D torso,XnVector3D leftShoulder,XnVector3D leftElbow, XnVector3D leftHand,XnVector3D rightShoulder, XnVector3D rightElbow, XnVector3D rightHand);
	GestureRecognizer(char* pathToModel);
	GestureRecognizer();
	bool LoadModel(char* path);
	GestureRecognizer operator=(const GestureRecognizer & rhs);
	GestureRecognizer(const GestureRecognizer& other);
	~GestureRecognizer();
};















#endif
