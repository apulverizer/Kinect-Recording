/******************************************************************************
 * GestureRecognition.cpp
 * 
 * Used to classify gestures in real-time using OpenNI and LIBSVM
 * 
 * Uses an array of 1080 features which are updated with each new frame of 
 * data from the Kinect.
 * 
 * Author: Aaron Pulver <atp1317@rit.edu>
 * **************************************************************************/

#include "GestureRecognizer.h"
#include "svm.h"

/**
 * Contstructor to create a gesture recognizer
 * @param pathToModel The file path to the libsvm model
 */
GestureRecognizer::GestureRecognizer(char* pathToModel)
{
	if(LoadModel(pathToModel)) // if sucessful allocate space for features
	{
		numberOfFrames=0;
		svmVec = (struct svm_node *)malloc((1081)*sizeof(struct svm_node));
		// initialize indicies and values
		for(int i=0;i<1080;i++)
		{
			svmVec[i].index=i+1;
			svmVec[i].value=0.0;
		}
		svmVec[1080].index=-1;
		svmVec[1080].value=0.0;
	}
}
/**
 * Constructor
 */
GestureRecognizer::GestureRecognizer()
{
	svmModel=NULL;
	numberOfFrames=0;
	svmVec = (struct svm_node *)malloc((1081)*sizeof(struct svm_node));
	// initialize indicies and values
	for(int i=0;i<1080;i++)
	{
		svmVec[i].index=i+1;
		svmVec[i].value=0.0;
	}
	svmVec[1080].index=-1;
	svmVec[1080].value=0.0;
}

/**
 * LoadModel
 * @param pathToModel The file path to the libsvm model
 */
bool GestureRecognizer::LoadModel(char* pathToModel)
{
	// load model
	if((svmModel = svm_load_model(pathToModel)) == 0)
	{
		fprintf(stderr, "Can't load SVM model %s", pathToModel);
		throw -1;
	}
	return true;
}
/**
 * Destructor
 */
GestureRecognizer::~GestureRecognizer()
{
	if(svmModel!=NULL)
	{
	  free(svmModel);
	}
	if(svmVec!=NULL)
	{
	  free(svmVec);
	}
}
/**
 * Classifies gestures
 * @return an integer which correponds to the class.
 */
int GestureRecognizer::Classify()
{
	// if we have 2 seconds of data
	if(numberOfFrames==60)
	{
		return (int)svm_predict(svmModel,svmVec);
	}
	else
	{
		return 0;
	}
}
/**
 * Prints the current set of features being used to classify
 * @param file an opened file pointer (append mode)
 * @param classLabel how to label this class
 */
void GestureRecognizer::PrintFeatures(FILE *file, int classLabel)
{
	//file = fopen("log.txt", "a");
	fprintf(file,"%d ",classLabel);
	for(int i=0;i<1080;i++)
	{
		fprintf(file,"%d:%lf ",i+1,svmVec[i].value);
	}
	fprintf(file,"\n");
}
/**
 * Update the features used to classify gestures
 * queue of 1080 features (60 frames *18 positions)
 */
void GestureRecognizer::UpdateFeatures(XnVector3D torso,XnVector3D leftShoulder,XnVector3D leftElbow, XnVector3D leftHand,XnVector3D rightShoulder, XnVector3D rightElbow, XnVector3D rightHand)
{
	leftShoulder=RelativeToJoint(torso,leftShoulder);
	leftElbow=RelativeToJoint(torso,leftElbow);
	leftHand=RelativeToJoint(torso,leftHand);
	rightShoulder=RelativeToJoint(torso,rightShoulder);
	rightElbow=RelativeToJoint(torso,rightElbow);
	rightHand=RelativeToJoint(torso,rightHand);

	// if initializing queue, fill it up
	if(numberOfFrames<60)
	{
		svmVec[numberOfFrames*18].value=leftShoulder.X;
		svmVec[numberOfFrames*18+1].value=leftShoulder.Y;
		svmVec[numberOfFrames*18+2].value=leftShoulder.Z;
		svmVec[numberOfFrames*18+3].value=leftElbow.X;
		svmVec[numberOfFrames*18+4].value=leftElbow.Y;
		svmVec[numberOfFrames*18+5].value=leftElbow.Z;
		svmVec[numberOfFrames*18+6].value=leftHand.X;
		svmVec[numberOfFrames*18+7].value=leftHand.Y;
		svmVec[numberOfFrames*18+8].value=leftHand.Z;

		svmVec[numberOfFrames*18+9].value=rightShoulder.X;
		svmVec[numberOfFrames*18+10].value=rightShoulder.Y;
		svmVec[numberOfFrames*18+11].value=rightShoulder.Z;
		svmVec[numberOfFrames*18+12].value=rightElbow.X;
		svmVec[numberOfFrames*18+13].value=rightElbow.Y;
		svmVec[numberOfFrames*18+14].value=rightElbow.Z;
		svmVec[numberOfFrames*18+15].value=rightHand.X;
		svmVec[numberOfFrames*18+16].value=rightHand.Y;
		svmVec[numberOfFrames*18+17].value=rightHand.Z;
		numberOfFrames++;
	}
	// otherwise, shift data and insert new frame to the end
	else
	{
		ShiftDataDown();
		// put data into last frame
		svmVec[1062].value=leftShoulder.X;
		svmVec[1063].value=leftShoulder.Y;
		svmVec[1064].value=leftShoulder.Z;
		svmVec[1065].value=leftElbow.X;
		svmVec[1066].value=leftElbow.Y;
		svmVec[1067].value=leftElbow.Z;
		svmVec[1068].value=leftHand.X;
		svmVec[1069].value=leftHand.Y;
		svmVec[1070].value=leftHand.Z;

		svmVec[1071].value=rightShoulder.X;
		svmVec[1072].value=rightShoulder.Y;
		svmVec[1073].value=rightShoulder.Z;
		svmVec[1074].value=rightElbow.X;
		svmVec[1075].value=rightElbow.Y;
		svmVec[1076].value=rightElbow.Z;
		svmVec[1077].value=rightHand.X;
		svmVec[1078].value=rightHand.Y;
		svmVec[1079].value=rightHand.Z;
		if(gestures.size()>0)
		{
			gestures.pop_front();
		}
	}
	if(svmModel!=NULL)
	{
	  gestures.push_back(Classify());
	}
}
/**
 * RelativeToJoint
 * Normalize the joint positions to a central joint
 */
XnVector3D GestureRecognizer::RelativeToJoint(XnVector3D main, XnVector3D other)
{
	//TODO take this off the stack
	XnVector3D normalized;
	normalized.X = other.X-main.X;
	normalized.Y = other.Y-main.Y;
	normalized.Z = other.Z-main.Z;
	return normalized;
}
/**
 * ShiftDataDown
 * Shifts data down by segments of 18 (1 frame)
 */
void GestureRecognizer::ShiftDataDown()
{
	for(int i=17;i<1080;i++)
	{
		svmVec[i-17].value=svmVec[i].value;
	}
}

/**
 * Assignment operator
 */
GestureRecognizer GestureRecognizer::operator=(const GestureRecognizer & other)
{
	this->numberOfFrames=other.numberOfFrames;
	if(this->svmVec!=NULL)
	{
		free(svmVec);
	}
	if(this->svmModel!=NULL)
	{
		free(svmModel);
	}
	svmVec = (struct svm_node *)malloc((1081)*sizeof(struct svm_node));
	memcpy(this->svmVec,other.svmVec,1081*sizeof(svm_node));
	svmModel = (svm_model*)malloc(sizeof(struct svm_model));
	memcpy(this->svmModel, other.svmModel, sizeof(svm_model));
	return *this;

}
/**
 * Copy Constructor
 */
GestureRecognizer::GestureRecognizer(const GestureRecognizer& other)
{
	this->numberOfFrames=other.numberOfFrames;
	svmVec = (struct svm_node *)malloc((1081)*sizeof(struct svm_node));
	memcpy(this->svmVec,other.svmVec,1081*sizeof(svm_node));
	svmModel = (svm_model*)malloc(sizeof(struct svm_model));
	memcpy(this->svmModel, other.svmModel, sizeof(svm_model));
}

