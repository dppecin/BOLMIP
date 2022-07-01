#ifdef DEBUG
#include <vld.h>
#endif

#include <iostream>
#include <cstdlib>

#include <time.h> 
#include "IPSolver.h"
#include "SequentialAlgorithm.h"

using namespace std;

int main(int argc, char** argv)
{
	//time_t start = time(0);

	// initialize the RNG with a fixed seed for reproducibility
	srand(123456);

	cout << "BOMIP Solver 1.0 by ISyE - 2017" << endl;

	cout << endl << "command line:";
	for (int a = 0; a < argc; a++)
		cout << " " << argv[a];
	cout << endl;

	IPSolver* ipsolver = IPSolver::getInstance();
	ipsolver->read(argc, argv);

	SequentialAlgorithm* algorithm = new SequentialAlgorithm();

	try
	{
		algorithm->run();
	}
	catch (IloException& e)
	{
		cout << "ERROR: Concert exception: " << e << endl;
		return -1;
	}

	algorithm->printStatistics();

	delete algorithm;

	//time_t end = time(0);
	//printf ("\nTotal time: %.f\n", difftime(end, start));

	return 0;
}
