//alwyas size of array recieve =size of send +100 or +1000...
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	int pid, np, elements_per_process, n_elements_recieved;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	int NumberOfElements = 1;

	////////////////////////////////Master Process  ////////////////////////////////////////////
	if (pid == 0)
	{
		char str[50];
		////////////////////////////////count number of Students ////////////////////////////////////////////
		char chr;
		FILE *ptr = fopen("/shared/Data.txt", "r");
		chr = getc(ptr);
		while (chr != EOF)
		{
			if (chr == '\n')
				NumberOfElements = NumberOfElements + 1;
			chr = getc(ptr);
		}
		fclose(ptr);
		////////////////////////////READ DATA FROM FILE ////////////////////////////////////////////////
		ptr = fopen("/shared/Data.txt", "r");
		if (NULL == ptr)
			printf("file can't be opened \n");
		int ID[NumberOfElements];
		int grade[NumberOfElements];
		int ii = 0;
		int x = 0;
		char *token;
		while (fgets(str, 50, ptr) != NULL)
		{
			token = strtok(str, "  ");
			x = atoi(token);
			ID[ii] = x;
			token = strtok(NULL, "  ");
			x = atoi(token);
			grade[ii] = x;
			ii = ii + 1;
		}
		fclose(ptr);
		////////////////////////////////Divide the students list by the number of processes you have then send it to them. ///////////////////
		int index, i;
		elements_per_process = NumberOfElements / (np - 1); // Master process not inculding with me
		if (np > 1)
		{
			index = 0;
			for (i = 1; i < np - 1; i++)
			{
				MPI_Send(&elements_per_process, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&ID[index], elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&grade[index], elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
				index = index + elements_per_process;
			}
			// index = index+ elements_per_process

			// last process take rest of Data (even /odd)
			int elements_left = NumberOfElements - index;
			MPI_Send(&elements_left, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&ID[index], elements_left, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&grade[index], elements_left, MPI_INT, i, 0, MPI_COMM_WORLD);

			////////////////////////////////Recevie Number of Strudents which pass the exam /////////////////////
			int NumOfPass = 0;
			int tmp;
			for (i = 1; i < np; i++)
			{
				MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status); // receive number of students whcih pass exam from each process
				int sender = status.MPI_SOURCE;
				NumOfPass += tmp;
			}
			printf("Total number of student passed the exam is   : %d out of  %d and number of process is %d\n", NumOfPass, NumberOfElements, pid);
		}
	}

	// Slaves Processes
	else
	{
		////////////////////////////////Receive part of students data/////////////////////
		int recGrade[NumberOfElements+100];													  // temp to store Received ID,GRADE
		int recID[NumberOfElements+100];													  // temp to store Received ID,GRADE
		MPI_Recv(&n_elements_recieved, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);		  // length of each process
		MPI_Recv(&recID, n_elements_recieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);	  // ID STUDENT
		MPI_Recv(&recGrade, n_elements_recieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); // Grade Student
		///////////////////////////Check if they passed or not and print message for each studen/////////////
		int NumberofPassed = 0;
		int i = 0;
		for (; i < n_elements_recieved; i++)
		{
			if (recGrade[i] >= 60)
			{
				printf("%dPassed the exam and number of process is%d\n", recID[i], pid);
				NumberofPassed = NumberofPassed + 1;
			}
			else
				printf("%dFalid please repeat the exam and number of process%d\n", recID[i], pid);
		}
		MPI_Send(&NumberofPassed, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // send number of strudent which pass of exam in each process
	}
	MPI_Finalize();
	return 0;
}
