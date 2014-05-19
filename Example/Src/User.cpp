#include "GlobalDefs.h"
#include "User.h"

/**
 * Constructor
 * @param ID the ID of the user
 */
User::User(int ID)
{
  gestureRecognizer.LoadModel((char*)"../../Models/Model.txt");
  id=ID;
}
/**
 * Constructor
 * Create a new user with id 0
 */
User::User()
{
	gestureRecognizer.LoadModel((char*)"../../Models/Model.txt");
	id=0;
}

/**
 * getId
 * Get the user Id
 */
int User::getId() const
{
	return id;
}


/**
 * Overloaded equals operator
 */
bool User::operator==(const User &other) const
{
    return (id == other.getId());
}
/**
 * Overloaded not equals operator
 */
bool User::operator!=(const User &other) const
{
	return (id != other.getId());
}
/**
 * setId
 * @param id the id to set
 */
void User::setId(int id)
{
	this->id=id;
}

/**
 * Copy Constructor
 */
User::User(const User& other)
{
	this->id=other.id;
	this->gestureRecognizer = other.gestureRecognizer;
	this->torsoPositions = other.torsoPositions;
}

/**
 * Assignment operator
 */
User User::operator=(const User &other )
{
	this->id=other.id;
	this->gestureRecognizer = other.gestureRecognizer;
	this->torsoPositions = other.torsoPositions;
	return *this;
}

XnFloat User::getCurrentDistance()
{
  XnVector3D position = getCurrentPosition();
  return sqrt((position.Z*position.Z)+(position.X*position.X));
  
}


/**
 * Add a new position/time to the rolling queue window
 */
void User::addPosition(XnVector3D torso, XnVector3D leftShoulder, XnVector3D rightShoulder, XnVector3D leftElbow, XnVector3D rightElbow, XnVector3D leftHand, XnVector3D rightHand, XnUInt64 newTime)
{
  if(torsoPositions.size()==QUEUE_SIZE)
  {
    torsoPositions.pop_front();
  }
  gestureRecognizer.UpdateFeatures(torso,leftShoulder,leftElbow, leftHand, rightShoulder, rightElbow, rightHand);
  torsoPositions.push_back(torso);
}

/**
 * Get the current (most recent position of the user)
 */
XnVector3D User::getCurrentPosition()
{
  if(torsoPositions.size()>=1)
  {
    return torsoPositions.back();
  }
  else
  {
	return XnVector3D();
  }
}

/**
 * Get the current angle (Radians) from the Kinect
 */
XnFloat User::getCurrentAngle()
{
	if(torsoPositions.size()>=1)
	{
		return (atan(torsoPositions.back().X/torsoPositions.back().Z));
	}
	else
	{
		return 0.0;
	}
}

/**
 * Get angle (Radians) moving based on a certain number of frames back
 * Based on typical coordinate systems where 0 degrees is on the right, 90 up top, 180 to the left, and 270 going down
 */
XnFloat User::getCurrentDirection( int framesBack)
{
	int size = torsoPositions.size();
	if(size<60)
	{
		return 0.0;
	}
	else
	{
		float deltaX=(torsoPositions.back().X-torsoPositions[size-framesBack].X);
		float deltaZ=(torsoPositions.back().Z-torsoPositions[size-framesBack].Z);
		float angle = atan(deltaZ/deltaX);
		// lower left quadrant
		if(deltaX <0 && deltaZ<0)
		{
		  angle+=PI;
		}
		// upper left quadrant
		else if(deltaX <0 && deltaZ>0)
		{
		  angle+=PI;
		}
		// lower right quadrant
		else if(deltaX >0 && deltaZ<0)
		{
		  angle +=2*PI;
		}

		return angle;
	}
}

/**
 * Is the user in the frame. Checks if last two positions are identical.
 */
bool User::isInFrame()
{
	int size = torsoPositions.size();
	if(size>=2)
	{
		return !((torsoPositions[size-1].X == torsoPositions[size-2].X) && (torsoPositions[size-1].Y == torsoPositions[size-2].Y) && (torsoPositions[size-1].Z == torsoPositions[size-2].Z));
	}
	else
	{
		return false;
	}
}
/**
 * Gets the the current gesture being performed
 */
Gesture User::getCurrentGesture()
{
	return (Gesture)gestureRecognizer.Classify();
}

/**
 * Destructor
 */
User::~User()
{

}
  
