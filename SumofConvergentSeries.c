
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    int pid, np;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    int n, localSize;
    double Elements[100000];
    double localStart, localFinish, elapsed, localElapsed, Start2, Finish2;
    double Summ[100000];

    MPI_Barrier(MPI_COMM_WORLD);
    ////////////////////////////////Master Process  ////////////////////////////////////////////
    if (pid == 0)
    {

        printf("Enter N : ");
        scanf("%d", &n);
        localStart = MPI_Wtime();
        // Start2 = MPI_Wtime();
        localSize = (n / np) + 2 * np;
        int Starting = 0;
        int Ending = (n / np);
        int i = 0;
        for (Starting; Starting < Ending; Starting++)
        {
            Elements[i] = (1.00 / (double)((Starting + 1) * (Starting + 1) * (Starting + 1)));
            i++;
        }
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Reduce(&Elements, &Summ, localSize, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        double totalSum = 0.00;
        double expected = 1.202056903159594;
        for (i = 0; i < localSize; i++)
        {
            totalSum += Summ[i];
        }

        // Finish2 = MPI_Wtime();

        printf("Calculated  is  %0.15lf \n", totalSum);
        printf("ERROR  is  %0.15lf \n", expected - totalSum);
        // printf("Elapsed Time With Sum = %lf\n", Finish2 - Start2);

        localFinish = MPI_Wtime();
        localElapsed = localFinish - localStart;
        MPI_Reduce(&localElapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        printf("Elapsed Time With Max time For processes = %lf\n", elapsed);

    } // Slaves Processes
    else
    {
        localStart = MPI_Wtime();
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        localSize = (n / np) + 2 * np;
        int Starting = (n / np) * (pid);
        int Ending = (n / np) * (pid + 1);
        int i = 0;
        if (n % np != 0)
            if (pid == np - 1)
                Ending = Ending + (n - Ending);

        for (Starting; Starting < Ending; Starting++)
        {
            Elements[i] = (1.00 / (double)((Starting + 1) * (Starting + 1) * (Starting + 1)));
            i++;
        }
        MPI_Reduce(&Elements, &Summ, localSize, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        localFinish = MPI_Wtime();
        localElapsed = localFinish - localStart;
        MPI_Reduce(&localElapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }
    // localFinish = MPI_Wtime();
    // localElapsed = localFinish - localStart;
    // MPI_Reduce(&localElapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // if (pid == 0)
    // printf("Elapsed Time With Max time For processes = %lf\n", elapsed);

    MPI_Finalize();

    return 0;
}



// N=100 Speed Efficiency N=1000 Speed Efficiency
// P=1 0.0024 1 1 0.002 1 1
// P=2 0.9329 2.57*10^-
// 3
// 1.071*10^-3 1.2423 1.6099 80.49*10^-4
// P=4 0.7676 3.1266 6.513*10^-3 1.030 1.9417 4.854*10^-4
// P=8 0.7495 3.202 3.33*10^-3 1.1055 1.8091 2.2614*10^-
// 5
// P=16 0.7132 3.365 1.752*10^-4 1.255 1.5936 9.96*10^-5
// P=32 0.80 3 7.182*10^-5 1.301 1.526 4.803*10^-5
// P=64 0.844 2.843 3.7026*10^-
// 5
// 1.312 1.424 2.381*10^-5