#------------------------------
# Makefile for KinectRecording
# Author: Aaron Pulver <atp1317@rit.edu>
#------------------------------
# Requires OpenNI 1.5.2

#Build
make:
	g++ -c Src/GestureRecognizer.cpp -I /usr/include/ni -l OpenNI -o Bin/GestureRecognizer.o
	g++ -c Src/svm.cpp -o Bin/svm.o

	g++ Src/MergeFiles.cpp -o Bin/MergeFiles
	g++ Src/KinectRecording.cpp -I /usr/include/ni -o Bin/KinectRecording -l OpenNI Bin/GestureRecognizer.o Bin/svm.o
	
	g++ Src/svm-train.c -o Bin/svm-train Bin/svm.o
	g++ Src/svm-predict.c -o Bin/svm-predict Bin/svm.o
	g++ Src/svm-scale.c -o Bin/svm-scale Bin/svm.o

	echo "Building Example..."
	$(MAKE)	-C	Example/
	
clean:
	rm	-f	Bin/KinectRecording Bin/GestureRecognizer.o Bin/svm.o Bin/svm-train Bin/svm-scale Bin/svm-predict Bin/MergeFiles

