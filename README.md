=============================================================================
KinectRecording
=============================================================================

KinectRecording is a package of programs which can be used to record and recognize
hand/upperbody gestures from a Kinect. KinectRecording records the X,Y,Z positions 
(relative to the torso) of:

Left Shoulder
Left Elbow
Left Hand
Right Shoulder
Right Elbow
Right Hand

The data is recorded in 2 second intervals. With 30fps and 18 joints, this
results in 1080 features. These features are written to the specified file
in LIBSVM format. The first column is the class label.The following 
columns are the actual features. The general process if outlined below:

1. Run KinectRecording to record one gesture for number of samples
	./KinectRecording <Output.txt> <number of iterations> <class label>
2. Repeat step 1 for another gesture (new file) until all gestures recorded.
3. Merge all of the data sets together into one large file (LIBSVM format)
	./MergeFiles <Output.txt> <file1.txt> <file2.txt>....
4. Run svm-train using your large data set to obtain a model.
	See LIBSVM README for more instructions
5. You now have a model which can be imported into an existing application.
	See Example 

You can also load the data sets using MATLAB if you wish to perform
optimiztions such as PSO, GA, or Grid search. Once the best parameters have
been chosen, you can train your model using svm-train with the correct
options. MATLAB does not have a way to export a model (that I know of) so you would have
to do this step using the c/c++ programs.

Requires:
* OpenNI 1.5.7
* SensorKinect driver (avin2)
* LIBSVM


