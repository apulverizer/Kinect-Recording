#include <fstream>
int main(int argc, char* argv[]) {
	// check for file
	if(argc<3)
	{
		printf("Missing files to concatenate... usage: MergeResults <MergedFile.txt> <file1.txt> <file2.txt>...\n");
		return -1;
	}
	int numberOfFiles = argc-2;
	printf("Number of files combining: %d\n",numberOfFiles);
	std::ofstream mergedFile(argv[1], std::ios_base::binary);
	for(int i= 0; i<numberOfFiles;i++)
	{
		printf("Appending [%s] to [%s] ...\n",argv[i+2],argv[1]);
		std::ifstream fileToAppend(argv[i+2], std::ios_base::binary);
		mergedFile << fileToAppend.rdbuf();
		fileToAppend.close();	
	}
	printf("Concatenation Finished\n");
	mergedFile.close();
	return 0;
}

