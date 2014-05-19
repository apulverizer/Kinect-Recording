#------------------------------
# Makefile for KinectRecording
# Author: Aaron Pulver <atp1317@rit.edu>
#------------------------------
# Requires OpenNI 1.5.2

#Build
make:

	g++ -c Src/User.cpp -I /usr/include/ni -l OpenNI -o Bin/User.o
	g++ -c Src/UserTracking.cpp -I /usr/include/ni -l OpenNI -o Bin/UserTracking.o 
	g++ Src/Example.cpp -I /usr/include/ni -o Bin/Example -l OpenNI Bin/UserTracking.o Bin/User.o ../Bin/svm.o ../Bin/GestureRecognizer.o
	
clean:
	rm	-f	Bin/User.o Bin/UserTracking.o Bin/Example




