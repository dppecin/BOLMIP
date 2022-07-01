#include "SequentialAlgorithm.h"

/* Multi-Box -- works just for the iterative inner loop; OBS.: NOT USED IN OUR PAPERS */
//#define IMPROVEMENTMULTIBOX

/* Special treatment of rectangles with corner points with same IP parts – mixed version; OBS.: NOT USED IN OUR PAPERS */
//#define IMPROVEMENTMIXED

#define ALLCONSTRAINTS

#define TIMELIMIT
#define MAXTOTALTIME  86400

//#define APPROXSOLUTIONSTATISTICS 

using namespace std;

bool sortfunction(NDP n1, NDP n2)
{
	//return ((n1.x < n2.x) || ((fabs(n1.x - n2.x) < EPS/10) && (n1.y > n2.y)));
	return ((n1.x < n2.x - 1.0e-6) || ((fabs(n1.x - n2.x) < 1.0e-6) && (n1.y > n2.y)));
}

SequentialAlgorithm::SequentialAlgorithm() : newSolver(IPSolver::getInstance())
{
	print = true;

	numberOfRectanglesSolved = 0;

	numberOfRectanglesEqualIPs = 0;

	numberOfRectanglesZstarOnCutLine = 0;
	numberOfRectanglesZstarNotOnCutLine = 0;

	numberOfRectanglesProducedEqualZero = 0;
	numberOfRectanglesProducedEqualOne = 0;
	numberOfRectanglesProducedEqualTwo = 0;

	totalArea = 0.0;
	totalAreaRectanglesSolved = 0.0;
	remainingArea = 0.0;
	totalTimeSoFar = 0.0;

	numberOfLexicographicIP = 0;
	numberOfMinIP = 0;
	numberOfScalarizationIP = 0;
	numberOfScalarizationWithNoGoodIP = 0;
	numberOfLP = 0;
	numberOfNDPs = 0;

	numberOfLineSegments = 0;
	numberOfOpenLineSegments = 0;
	numberOfHalfOpenLineSegments = 0;
	numberOfClosedLineSegments = 0;
	numberOfPoints = 0;

	totalTimeLexicographicIP = 0.0;
	totalTimeMinIP = 0.0;
	totalTimeScalarizationIP = 0.0;
	totalTimeScalarizationWithNoGoodIP = 0.0;
	totalTimeLP = 0.0;

	totalTimeOuterLoop = 0.0;
	totalTimeInnerLoop = 0.0;
	totalTimeLineGen = 0.0;

	numberCallsInnerLoop = 0;
	numberProvedOptByOneScalInnerLoop = 0;
	numberCallsOuterLooptoInnerLoop = 0;
	maximumdepth = 0;

	numberOfInnerLoopWithNoGoodIP = 0;
	totalTimeOfInnerLoopWithNoGoodIP = 0.0;

	innerLoopNumberOfLP = 0;
	innerLoopTotalTimeLP = 0.0;

	maxTimeScal = 0.0;
	maxTimeLex = 0.0;

	approxTotalTime = 16.00;

	totalTime = 0.0;

	lexArea = 0.0;

	noGoodSuccessRate = 0;
	noGoodNumberOfCalls = 0;

	numberOfPieceWiseLinearSolves = 0;
	numberOfSegmentsPieceWiseLinear = 0;
	segmentSolutionWasFound = 0;
	numberOfPieceWiseLinearSolvesIfFeasible = 0;

}

SequentialAlgorithm::~SequentialAlgorithm()
{
	//delete newSolver; 
}

void SequentialAlgorithm::printStatistics()
{

	printf("\n---------------------------- STATISTICS ----------------------------\n\n");
	printf("\nTotal Number of (extreme) NDPs: %d\n", numberOfNDPs);
	printf("   Number of Integer Frontiers: %d\n", numberOfIntegerFrontiers);
	printf("Total Number of Line Segments: %d\n", numberOfLineSegments);
	printf("   Total Number of Open Line Segments: %d\n", numberOfOpenLineSegments);
	printf("   Total Number of Half Open Line Segments: %d\n", numberOfHalfOpenLineSegments);
	printf("   Total Number of Closed Line Segments: %d\n", numberOfClosedLineSegments);
	printf("Total Number of Points: %d\n", numberOfPoints);

	//printf("\nTotal Time: %.2lf\n", timeTotalTime.getCPUTotalSecs());

	printf("\nTotal Time: %.2lf\n", totalTime);

#ifndef RESTRICTIVELINE

#ifndef IMPROVEMENTNOGOOD
	printf("Total Time for IP: %.2lf (Lex: %.2lf; Min: %.2lf; Scal: %.2lf)\n", totalTimeLexicographicIP +
		totalTimeMinIP + totalTimeScalarizationIP, totalTimeLexicographicIP, totalTimeMinIP, totalTimeScalarizationIP);
#else
	printf("Total Time for IP: %.2lf (Lex: %.2lf; Min: %.2lf; Scal: %.2lf; NoGood: %.2lf)\n", totalTimeLexicographicIP +
		totalTimeMinIP + totalTimeScalarizationIP + totalTimeScalarizationWithNoGoodIP, totalTimeLexicographicIP, totalTimeMinIP, totalTimeScalarizationIP, totalTimeScalarizationWithNoGoodIP);
#endif 	

#else

#ifndef IMPROVEMENTNOGOOD
	printf("Total Time for IP: %.2lf (Lex: %.2lf; RLIP: %.2lf; Scal: %.2lf)\n", totalTimeLexicographicIP +
		totalTimeOfInnerLoopWithNoGoodIP + totalTimeScalarizationIP, totalTimeLexicographicIP, totalTimeOfInnerLoopWithNoGoodIP, totalTimeScalarizationIP);
#else
	printf("Total Time for IP: %.2lf (Lex: %.2lf; RLIP: %.2lf; Scal: %.2lf; NoGood: %.2lf)\n", totalTimeLexicographicIP +
		totalTimeOfInnerLoopWithNoGoodIP + totalTimeScalarizationIP + totalTimeScalarizationWithNoGoodIP, totalTimeLexicographicIP, totalTimeOfInnerLoopWithNoGoodIP, totalTimeScalarizationIP, totalTimeScalarizationWithNoGoodIP);
#endif

#endif
	printf("Total Time for LP: %.2lf\n", totalTimeLP);

#ifndef RESTRICTIVELINE

#ifndef IMPROVEMENTNOGOOD
	printf("\nTotal Number of IP: %d (Lex: %d; Min: %d; Scal: %d)\n", 2 * numberOfLexicographicIP + numberOfMinIP +
		numberOfScalarizationIP, numberOfLexicographicIP, numberOfMinIP, numberOfScalarizationIP);
#else	
	printf("\nTotal Number of IP: %d (Lex: %d; Min: %d; Scal: %d; NoGood: %d)\n", 2 * numberOfLexicographicIP + numberOfMinIP +
		numberOfScalarizationIP + numberOfScalarizationWithNoGoodIP, numberOfLexicographicIP, numberOfMinIP, numberOfScalarizationIP, numberOfScalarizationWithNoGoodIP);
#endif

#else

#ifndef IMPROVEMENTNOGOOD
	printf("\nTotal Number of IP: %d (Lex: %d; RLIP: %d; Scal: %d)\n", 2 * numberOfLexicographicIP + numberOfInnerLoopWithNoGoodIP +
		numberOfScalarizationIP, numberOfLexicographicIP, numberOfInnerLoopWithNoGoodIP, numberOfScalarizationIP);
#else
	printf("\nTotal Number of IP: %d (Lex: %d; RLIP: %d; Scal: %d; NoGood: %d)\n", 2 * numberOfLexicographicIP + numberOfInnerLoopWithNoGoodIP +
		numberOfScalarizationIP + numberOfScalarizationWithNoGoodIP, numberOfLexicographicIP, numberOfInnerLoopWithNoGoodIP, numberOfScalarizationIP, numberOfScalarizationWithNoGoodIP);

#endif

#endif

	printf("Total Number of LP: %d\n", numberOfLP);

	printf("\nTotal Number of Rectangles Solved: %d\n", numberOfRectanglesSolved);
	printf("Total Number of Rectangles Solved with Equal IP solutions: %d (%.2lf)\n", numberOfRectanglesEqualIPs, ((double)(100 * numberOfRectanglesEqualIPs)) / numberOfRectanglesSolved);
	printf("Total Number of Rectangles with zstar on Cut Line: %d (%.2lf)\n", numberOfRectanglesZstarOnCutLine, ((double)(100 * numberOfRectanglesZstarOnCutLine)) / numberOfRectanglesSolved);
	printf("Total Number of Rectangles without zstar on Cut Line: %d (%.2lf)\n", numberOfRectanglesZstarNotOnCutLine, ((double)(100 * numberOfRectanglesZstarNotOnCutLine)) / numberOfRectanglesSolved);
	printf("  Total Number of Rectangles Produced with Equal 0, 1 and 2: %d, %d, %d\n", numberOfRectanglesProducedEqualZero, numberOfRectanglesProducedEqualOne, numberOfRectanglesProducedEqualTwo);

	printf("\nTotal Time for OuterLoop: %.2lf\n", totalTimeOuterLoop);
	printf("Total Time for InnerLoop: %.2lf\n", totalTimeInnerLoop);
	printf("Total Time for LineGen: %.2lf\n", totalTimeLineGen);

	printf("\nNumber of Calls to Inner Loop: %d\n", numberCallsInnerLoop);

#ifdef RECURSIVEALGO
	printf("Number of Calls from Outer Loop to Inner loop: %d\n", numberCallsOuterLooptoInnerLoop);
	printf("Factor: %.2lf%\n", 100 * (((double)numberCallsOuterLooptoInnerLoop) / numberCallsInnerLoop));
	printf("Maximum of depth level: %d\n", maximumdepth);
#else
	printf("Number Proved Opt by just One Scal: %d\n", numberProvedOptByOneScalInnerLoop);
	printf("Factor: %.2lf%\n", 100 * (((double)numberProvedOptByOneScalInnerLoop) / numberCallsInnerLoop));
	printf("Total Time for LP inside Inner Loop: %.2lf\n", innerLoopTotalTimeLP);
	printf("Total Number of LP inside Inner Loop: %d\n", innerLoopNumberOfLP);
#endif

	printf("Max Lex IP Time = %lf\n", maxTimeLex);
	printf("Max Scal IP Time = %lf\n", maxTimeScal);

#ifdef IMPROVEMENTNOGOOD
	printf("No-Good success rate: %.2lf\n", 100 * (((double)(noGoodSuccessRate)) / noGoodNumberOfCalls));
#endif

	printf("\n---------------------------- STATISTICS ----------------------------\n");

}


void SequentialAlgorithm::run()
{
	timeTotalTime.start();
	timeSoFar.start();

	printf("EPS used: %lf\n", EPS);
	printf("EPSCPL used: %lf\n", EPSCPL);
	printf("LINEGENDELTA used: %d\n", LINEGENDELTA);

#ifdef USEPQUEUE
	printf("PQUEUE used\n");
#else
	printf("STACK used\n");
#endif 

#ifdef BLMALGO
	printf("BLM Algorithm\n");
#endif 

#ifdef IMPROVEMENTMULTIBOX
	printf("MULTIBOX Algorithm\n");
#endif

#ifdef IMPROVEMENTMIXED
	printf("MIXED Algorithm\n");
#endif 

#ifdef IMPROVEMENTNOGOOD
	printf("NOGOOD Algorithm\n");
#endif

#ifdef RECURSIVEALGO
	printf("RECURSIVE Algorithm\n");
#endif

#ifdef RESTRICTIVELINE
	printf("RESTRICTIVE LINE Algorithm\n");
#endif

#ifdef RESTRICTIVELINEALGORITHMSWITCHTOETM
	printf("RESTRICTIVE LINE SWITCH TO eTM Algorithm, PARAM = %lf\n", RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM);
#endif

#ifdef PWL
	printf("PIECEWISE LINEAR Algorithm\n");
#endif

	printf("\n");

	//newSolver = new IPSolver();

	NDP p1, p2;
	p1.connected = false;
	p2.connected = false;
	p1.closed = true;
	p2.closed = true;

	printf("\nSolving...\n");
	numberOfLexicographicIP++;
	timeLexicographicIP.reset();
	timeLexicographicIP.start();
	lexminObj1Obj2(p1, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), false, p1);
	timeLexicographicIP.stop();
	totalTimeLexicographicIP += timeLexicographicIP.getCPUTotalSecs();

	printf("L-NDP: (%lf, %lf)\n", p1.x, p1.y);
	if (print) printf("Lex Time = %lf\n", timeLexicographicIP.getCPUTotalSecs());
	if (print) printf("Lex Area = %lf\n", lexArea);
	lexMinUpdate(timeLexicographicIP.getCPUTotalSecs(), lexArea);

	if (maxTimeLex < timeLexicographicIP.getCPUTotalSecs()) maxTimeLex = timeLexicographicIP.getCPUTotalSecs();

	numberOfLexicographicIP++;
	timeLexicographicIP.reset();
	timeLexicographicIP.start();
	lexminObj2Obj1(p2, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), true, p1);
	timeLexicographicIP.stop();
	totalTimeLexicographicIP += timeLexicographicIP.getCPUTotalSecs();

	printf("R-NDP: (%lf, %lf)\n\n", p2.x, p2.y);
	if (print) printf("Lex Time = %lf\n", timeLexicographicIP.getCPUTotalSecs());
	if (print) printf("Lex Area = %lf\n", lexArea);
	lexMinUpdate(timeLexicographicIP.getCPUTotalSecs(), lexArea);

	if (maxTimeLex < timeLexicographicIP.getCPUTotalSecs()) maxTimeLex = timeLexicographicIP.getCPUTotalSecs();

	ndpList.push_back(p1);
	ndpList.push_back(p2);

	/*
		@IMPORTANT:

		-- For Box Line Method (BLM) algorithms, a priority queue (pqueue below) must be used and outerLoop(); must be uncomment below
			-- For eTM, a stack must be used and restrictiveLineAlgorithm(); must be uncomment below
		-- For PWL, a stack must be used and restrictiveFrontierAlgorithm(); must be uncomment below
		-- To switch from BLM to eTM, use a priority queue and uncomment both outerLoop() and  restrictiveLineAlgorithm()
		-- To switch from BLM to PWL, use a priority queue and uncomment both outerLoop() and  restrictiveFrontierAlgorithm();

	*/

	if (!equalPoints(p1, p2))
	{
		Rectangle r(p1, p2);

		totalArea = r._area;

		printf("Total Area = %.2lf\n\n", totalArea);
#ifdef USEPQUEUE
		pqueue.push(r);
#else
		stack.push_back(r);
#endif
		remainingArea += r._area;

#ifdef BLMALGO
		outerLoop();
#endif

#ifdef RESTRICTIVELINE

#ifndef PWL
		restrictiveLineAlgorithm();
#else
		restrictiveFrontierAlgorithm();
#endif

#endif

	}


#ifdef APPROXSOLUTIONSTATISTICS 
	timeTotalTime.stop();
	totalTime += timeTotalTime.getCPUTotalSecs();

	timeSoFar.stop();
	totalTimeSoFar += timeSoFar.getCPUTotalSecs();

	approximateSolution(totalTimeSoFar);

	printf("\n------------------------ Final Approx Solution ----------------------------------\n");
	for (int i = 0; i < (int)approxSolVector.size(); i++)
	{
		/*
				printf("\n\n");

				printf("Total time: %.2lf\n",approxSolVector[i].totalTime);
				printf("Total area of the largest box in the queue: %lf\n", approxSolVector[i].areaLargestBox);
				printf("Total number of isolated points: %.2lf\n", 100*((double)approxSolVector[i].numberIsolatedPoints/approxSolVector[(int)approxSolVector.size()-1].numberIsolatedPoints));
				printf("Total number of NDPs: %.2lf\n", 100*((double)approxSolVector[i].numberNDPs/approxSolVector[(int)approxSolVector.size()-1].numberNDPs));
				printf("Total lenght of non-dominated line segments: %.2lf\n", 100*(approxSolVector[i].lengthNDLS/approxSolVector[(int)approxSolVector.size()-1].lengthNDLS));

				printf("Total proportion of resolved areas: %.2lf\n", approxSolVector[i].proportionSolvedArea);
				printf("Total proportion of integer frontier: %.2lf\n", 100*((double)approxSolVector[i].numberOfIntegerSolutions/approxSolVector[(int)approxSolVector.size()-1].numberOfIntegerSolutions));
				printf("\n------------------------ Final Approx Solution ----------------------------------\n");

				printf("\n\n");
		*/

		printf(" %.2lf & %.2lf & %lf & %.2lf & %.2lf & %.2lf & %.2lf & %.2lf\n", approxSolVector[i].stepTime, approxSolVector[i].totalTime, approxSolVector[i].areaLargestBox, 100 * ((double)approxSolVector[i].numberIsolatedPoints / approxSolVector[(int)approxSolVector.size() - 1].numberIsolatedPoints),
			100 * ((double)approxSolVector[i].numberNDPs / approxSolVector[(int)approxSolVector.size() - 1].numberNDPs), 100 * (approxSolVector[i].lengthNDLS / approxSolVector[(int)approxSolVector.size() - 1].lengthNDLS),
			approxSolVector[i].proportionSolvedArea, 100 * ((double)approxSolVector[i].numberOfIntegerSolutions / approxSolVector[(int)approxSolVector.size() - 1].numberOfIntegerSolutions));

	}
	printf("\n------------------------ Final Approx Solution ----------------------------------\n");

	for (int i = 0; i < (int)approxNDPMatrix.size() - 1; i++)
	{
		for (int j = 0; j < (int)approxNDPMatrix[i].size(); j++)
		{
			//printf("%lf %lf %d %d\n", approxNDPMatrix[i][j].x, approxNDPMatrix[i][j].y, approxNDPMatrix[i][j].connected ? 1 : 0, approxNDPMatrix[i][j].id);

			bool newId = true;
			for (int k = 0; k < (int)approxNDPMatrix[(int)approxNDPMatrix.size() - 1].size(); k++)
			{
				if (equalPoints(approxNDPMatrix[i][j], approxNDPMatrix[(int)approxNDPMatrix.size() - 1][k]))
				{
					approxNDPMatrix[i][j].id = approxNDPMatrix[(int)approxNDPMatrix.size() - 1][k].id;
					newId = false;
					break;
				}
			}

			if (newId)
			{
				printf("\n\nNDP id not found: Part %d\n\n\n", i);
				printf("\n==>> %lf %lf %d %d\n", approxNDPMatrix[i][j].x, approxNDPMatrix[i][j].y, approxNDPMatrix[i][j].connected ? 1 : 0, approxNDPMatrix[i][j].id);
				//exit(0);
			}

		}
	}

	printf("\n------------------------ Print Final Approx Solution ----------------------------------\n");
	for (int i = 0; i < (int)approxNDPMatrix.size(); i++)
	{
		printf("Part %d:\n\n", i);
		ofstream approxSolutions;

		string name(newSolver->getName().begin(), newSolver->getName().end() - 3);

		stringstream ss;
		ss << name << "-spurePart" << i + 1 << ".txt";
		name = ss.str();

		approxSolutions.open(name.c_str());

		for (int j = 0; j < (int)approxNDPMatrix[i].size(); j++)
		{
			printf("%lf %lf %d %d\n", approxNDPMatrix[i][j].x, approxNDPMatrix[i][j].y, approxNDPMatrix[i][j].connected ? 1 : 0, approxNDPMatrix[i][j].id);

			approxSolutions << approxNDPMatrix[i][j].x << " " << approxNDPMatrix[i][j].y << " " << (approxNDPMatrix[i][j].connected ? 1 : 0) << " " << approxNDPMatrix[i][j].id << endl;

		}
		approxSolutions.close();
		printf("\n\n");
	}
	printf("\n------------------------ Print Final Approx Solution ----------------------------------\n");

	timeSoFar.reset();
	timeSoFar.start();

	timeTotalTime.reset();
	timeTotalTime.start();
#endif

	CPUTimer sortTime;
	sortTime.start();
	std::sort(ndpList.begin(), ndpList.end(), sortfunction);
	sortTime.stop();

	printf("\nNumber in ndpList = %d\n", (int)ndpList.size());
	printf("Time for sorting = %.2lf\n", sortTime.getCPUTotalSecs());
	printf("\n\nFinished!!!\n\n");

	timeTotalTime.stop();
	totalTime += timeTotalTime.getCPUTotalSecs();

	timeSoFar.stop();
	totalTimeSoFar += timeSoFar.getCPUTotalSecs();

	ndpList[0].id = 1;
	numberOfIntegerFrontiers = 1;
	for (int i = 1; i < (int)ndpList.size(); i++)
	{
		bool newId = true;
		for (int j = 0; j < i; j++)
		{
			if (isSameFrontier(ndpList[j], ndpList[i]))
			{
				ndpList[i].id = ndpList[j].id;
				newId = false;
				break;
			}
		}

		if (newId)
		{
			numberOfIntegerFrontiers++;
			ndpList[i].id = numberOfIntegerFrontiers;
		}
	}

	timeSoFar.reset();
	timeSoFar.start();

	timeTotalTime.reset();
	timeTotalTime.start();

	std::vector<NDP> ndpListPostProcessor;

	printf("\n\nNondominated frontier:\n\n");

	for (int i = 0; i < (int)ndpList.size(); i++)
	{

		if ((i + 1 == (int)ndpList.size()) || !equalPoints(ndpList[i], ndpList[i + 1]))
		{
			//printf("(%lf, %lf) %d\n", ndpList[i].x, ndpList[i].y, ndpList[i].connected ? 1 : 0);
			//printf("(%.3lf, %.3lf) %d\n", ndpList[i].x, ndpList[i].y, ndpList[i].connected ? 1 : 0);

			//printf("%.3lf %.3lf %d %d\n", ndpList[i].x, ndpList[i].y, ndpList[i].connected ? 1 : 0, ndpList[i].id);

			ndpListPostProcessor.push_back(ndpList[i]);

			//printf("%.3lf, %.3lf\n", ndpList[i].x, ndpList[i].y);

		}
		else
		{
			bool connected = (ndpList[i].connected || ndpList[i + 1].connected);
			bool closed = (ndpList[i].closed && ndpList[i + 1].closed);

			//printf("(%lf, %lf) %d\n", ndpList[i].x, ndpList[i].y, connected ? 1 : 0);
			//printf("(%.3lf, %.3lf) %d\n", ndpList[i].x, ndpList[i].y, connected ? 1 : 0);
			//printf("%.3lf %.3lf %d %d\n", ndpList[i].x, ndpList[i].y, connected ? 1 : 0, ndpList[i].id);

			ndpList[i].connected = connected;
			ndpList[i].closed = closed;
			ndpListPostProcessor.push_back(ndpList[i]);

			//printf("%.3lf, %.3lf\n", ndpList[i].x, ndpList[i].y);

			i++;
		}

		//ndpListPostProcessor.push_back(ndpList[i]);

	}


	for (int i = 0; i < (int)ndpListPostProcessor.size(); i++)
	{

		if ((i > 0) && (i < (int)ndpListPostProcessor.size() - 1) &&
			(fabs(fabs((ndpListPostProcessor[i - 1].y - ndpListPostProcessor[i].y) / (ndpListPostProcessor[i].x - ndpListPostProcessor[i - 1].x)) -
				fabs((ndpListPostProcessor[i].y - ndpListPostProcessor[i + 1].y) / (ndpListPostProcessor[i + 1].x - ndpListPostProcessor[i].x))) < EPS))
		{
			continue;
		}

		numberOfNDPs++;

		if (ndpListPostProcessor[i].connected)
		{
			numberOfLineSegments++;
			if (ndpListPostProcessor[i].closed && ndpListPostProcessor[i + 1].closed)
			{
				numberOfClosedLineSegments++;
			}
			else
			{
				if (ndpListPostProcessor[i].closed || ndpListPostProcessor[i + 1].closed) numberOfHalfOpenLineSegments++;
				else numberOfOpenLineSegments++;
			}

		}
		else if ((i > 0) && !ndpListPostProcessor[i - 1].connected) numberOfPoints++;

		//printf("%.3lf %.3lf %d %d\n", ndpListPostProcessor[i].x, ndpListPostProcessor[i].y, ndpListPostProcessor[i].connected ? 1 : 0, ndpListPostProcessor[i].id);

		printf("%lf %lf %d %d\n", ndpListPostProcessor[i].x, ndpListPostProcessor[i].y, ndpListPostProcessor[i].connected ? 1 : 0, ndpListPostProcessor[i].id);

		//printf("%lf %lf %d %d %d\n", ndpListPostProcessor[i].x, ndpListPostProcessor[i].y, ndpListPostProcessor[i].connected ? 1 : 0, ndpListPostProcessor[i].id, ndpListPostProcessor[i].closed ? 1 : 0);
		//printf("%lf %lf %d\n", ndpListPostProcessor[i].x, ndpListPostProcessor[i].y, ndpListPostProcessor[i].connected ? 1 : 0);

	}

	timeTotalTime.stop();
	totalTime += timeTotalTime.getCPUTotalSecs();

	timeSoFar.stop();
	totalTimeSoFar += timeSoFar.getCPUTotalSecs();

	ndpList.clear();

	printf("\nlatex %s & %d & %d & %d & %d & %d &  %d & %d & %.1lf & %.1lf & %.1lf &  %d & %d & %d & %d & %d & %d & %d &  %d &  %d &  %d &  %d &  %d & %d\n",

		newSolver->getName().c_str(),
		numberOfNDPs,
		numberOfIntegerFrontiers,
		numberOfLineSegments,
		numberOfOpenLineSegments,
		numberOfHalfOpenLineSegments,
		numberOfClosedLineSegments,
		numberOfPoints,

		//timeTotalTime.getCPUTotalSecs(),
		totalTime,

#ifdef RESTRICTIVELINE
		totalTimeLexicographicIP + totalTimeOfInnerLoopWithNoGoodIP + totalTimeScalarizationIP + totalTimeScalarizationWithNoGoodIP,
#else
		totalTimeLexicographicIP + totalTimeMinIP + totalTimeScalarizationIP + totalTimeScalarizationWithNoGoodIP,
#endif
		totalTimeLP,

#ifdef RESTRICTIVELINE
		2 * numberOfLexicographicIP + numberOfInnerLoopWithNoGoodIP + numberOfScalarizationIP + numberOfScalarizationWithNoGoodIP,
#else
		2 * numberOfLexicographicIP + numberOfMinIP + numberOfScalarizationIP + numberOfScalarizationWithNoGoodIP,
#endif

		numberOfLexicographicIP,

#ifdef RESTRICTIVELINE
		numberOfInnerLoopWithNoGoodIP,
#else
		numberOfMinIP,
#endif

		numberOfScalarizationIP,

		numberOfScalarizationWithNoGoodIP,

		numberOfLP,

		numberOfRectanglesSolved,
		numberOfRectanglesEqualIPs,
		numberOfRectanglesZstarOnCutLine,
		numberOfRectanglesZstarNotOnCutLine,
		numberOfRectanglesProducedEqualZero,
		numberOfRectanglesProducedEqualOne,
		numberOfRectanglesProducedEqualTwo
	);


	//nNDP nIPF TT IPT LPT nIP nLex nMin nScal nGood nLP nRec nSIS nZL
	printf("\nlatex2 %s & %d & %d & %.1lf & %.1lf & %.1lf &  %d & %d & %d & %d & %d & %d & %d & %d & %d \n",

		newSolver->getName().c_str(),
		numberOfNDPs,
		numberOfIntegerFrontiers,

		//timeTotalTime.getCPUTotalSecs(),
		totalTime,

#ifdef RESTRICTIVELINE
		totalTimeLexicographicIP + totalTimeOfInnerLoopWithNoGoodIP + totalTimeScalarizationIP + totalTimeScalarizationWithNoGoodIP,
#else
		totalTimeLexicographicIP + totalTimeMinIP + totalTimeScalarizationIP + totalTimeScalarizationWithNoGoodIP,
#endif
		totalTimeLP,

#ifdef RESTRICTIVELINE
		2 * numberOfLexicographicIP + numberOfInnerLoopWithNoGoodIP + numberOfScalarizationIP + numberOfScalarizationWithNoGoodIP,
#else
		2 * numberOfLexicographicIP + numberOfMinIP + numberOfScalarizationIP + numberOfScalarizationWithNoGoodIP,
#endif

		numberOfLexicographicIP,

#ifdef RESTRICTIVELINE
		numberOfInnerLoopWithNoGoodIP,
#else
		numberOfMinIP,
#endif

		numberOfScalarizationIP,

		numberOfScalarizationWithNoGoodIP,

		numberOfLP,

		numberOfRectanglesSolved,
		numberOfRectanglesEqualIPs,
		numberOfRectanglesZstarOnCutLine
	);


#ifdef PWL
	printf("\nlatex3 %s & %d & %d & %d & %.1lf \n",

		newSolver->getName().c_str(),
		numberOfPieceWiseLinearSolves,
		(int)((((double)numberOfSegmentsPieceWiseLinear) / numberOfPieceWiseLinearSolves) + 0.5),
		(int)((((double)segmentSolutionWasFound) / numberOfPieceWiseLinearSolvesIfFeasible) + 0.5),
		totalTimeOfInnerLoopWithNoGoodIP / numberOfPieceWiseLinearSolves);
#endif 

	/*
		printf("\nTotal Area = %.2lf\n\n", totalArea);
		printf("\n------------------------ Final Lex Times ----------------------------------\n");
		for (int i = 0; i < (int)lexMinVector.size(); i++)
			printf(" %lf & %lf \n", lexMinVector[i].time, lexMinVector[i].area);
		printf("\n------------------------ Final Lex Times ----------------------------------\n");

		printf("\n------------------------ Final Scal Times ----------------------------------\n");
		for (int i = 0; i < (int)scalMinVector.size(); i++)
			printf(" %lf & %lf \n", scalMinVector[i].time, scalMinVector[i].area);
		printf("\n------------------------ Final Scal Times ----------------------------------\n");
	*/

}

void SequentialAlgorithm::approximateSolution(double totalTimeSoFar)
{
	std::vector<NDP> ndpListCopy;
	for (int i = 0; i < (int)ndpList.size(); i++) ndpListCopy.push_back(ndpList[i]);

	double sortTTime = 0.0;
	CPUTimer sortTime;
	sortTime.start();

	std::sort(ndpListCopy.begin(), ndpListCopy.end(), sortfunction);

	sortTime.stop();
	sortTTime += sortTime.getCPUTotalSecs();

	ndpListCopy[0].id = 1;
	int numberOfApproximateIntegerFrontiers = 1;
	for (int i = 1; i < (int)ndpListCopy.size(); i++)
	{
		bool newId = true;
		for (int j = 0; j < i; j++)
		{
			if (isSameFrontier(ndpListCopy[j], ndpListCopy[i]))
			{
				ndpListCopy[i].id = ndpListCopy[j].id;
				newId = false;
				break;
			}
		}

		if (newId)
		{
			numberOfApproximateIntegerFrontiers++;
			ndpListCopy[i].id = numberOfApproximateIntegerFrontiers;
		}
	}

	sortTime.reset();
	sortTime.start();

	std::vector<NDP> ndpListPostProcessor;

	for (int i = 0; i < (int)ndpListCopy.size(); i++)
	{
		if ((i + 1 == (int)ndpListCopy.size()) || !equalPoints(ndpListCopy[i], ndpListCopy[i + 1]))
		{
			ndpListPostProcessor.push_back(ndpListCopy[i]);
		}
		else
		{
			bool connected = (ndpListCopy[i].connected || ndpListCopy[i + 1].connected);
			bool closed = (ndpListCopy[i].closed && ndpListCopy[i + 1].closed);

			ndpListCopy[i].connected = connected;
			ndpListCopy[i].closed = closed;
			ndpListPostProcessor.push_back(ndpListCopy[i]);

			i++;
		}
	}

	int numberOfApproximateNDPs = 0;
	int numberOfApproximatePoints = 0;
	double approximateLengthNDLS = 0.0;
	double approximateLengthVerticalGap = 0.0;
	double approximateLengthHorizontalGap = 0.0;
	std::vector<NDP> approxNDPList;
	for (int i = 0; i < (int)ndpListPostProcessor.size(); i++)
	{
		numberOfApproximateNDPs++;
		approxNDPList.push_back(ndpListPostProcessor[i]);

		if (!ndpListPostProcessor[i].connected && ((i > 0) && !ndpListPostProcessor[i - 1].connected)) numberOfApproximatePoints++;

		//printf("%lf %lf %d %d\n", ndpListPostProcessor[i].x, ndpListPostProcessor[i].y, ndpListPostProcessor[i].connected ? 1 : 0, ndpListPostProcessor[i].id);

		if (i < (int)ndpListPostProcessor.size() - 1)
		{
			if (ndpListPostProcessor[i].connected)
			{
				double a = ndpListPostProcessor[i + 1].x - ndpListPostProcessor[i].x;
				double b = ndpListPostProcessor[i].y - ndpListPostProcessor[i + 1].y;

				approximateLengthNDLS += sqrt(a * a + b * b);
			}
			else
			{
				approximateLengthHorizontalGap += ndpListPostProcessor[i + 1].x - ndpListPostProcessor[i].x;
				approximateLengthVerticalGap += ndpListPostProcessor[i].y - ndpListPostProcessor[i + 1].y;
			}
		}
	}

	sortTime.stop();
	sortTTime += sortTime.getCPUTotalSecs();

	approxSol aps;
	approxNDPMatrix.push_back(approxNDPList);

	if (!pqueue.empty()) aps.areaLargestBox = 100 * ((pqueue.top())._area / totalArea);
	else aps.areaLargestBox = 0.0;

	aps.stepTime = totalTimeSoFar;

	aps.totalTime = totalTimeSoFar + sortTTime;

	aps.numberIsolatedPoints = numberOfApproximatePoints;
	aps.lengthNDLS = approximateLengthNDLS;

	aps.numberNDPs = numberOfApproximateNDPs;

	//aps.lengthVerticalGap  = approximateLengthVerticalGap;
	//aps.lengthHorizontalGap = approximateLengthHorizontalGap;

	std::vector<Rectangle> temp;
	double totalAreaQueue = 0.0;
	while (!pqueue.empty())
	{
		Rectangle r = pqueue.top();
		totalAreaQueue += r._area;

		pqueue.pop();

		temp.push_back(r);
	}
	for (int i = 0; i < (int)temp.size(); i++) pqueue.push(temp[i]);

	aps.proportionSolvedArea = 100 * ((totalArea - totalAreaQueue) / totalArea);

	aps.numberOfIntegerSolutions = numberOfApproximateIntegerFrontiers;

	printf("\n------------------------ Approx Solution ----------------------------------\n");
	printf("Total step time: %.2lf\n", aps.stepTime);
	printf("Total time: %.2lf\n", aps.totalTime);
	printf("Total area of the largest box in the queue: %lf\n", aps.areaLargestBox);
	printf("Total number of isolated points: %d\n", aps.numberIsolatedPoints);
	printf("Total number of NDPs: %d\n", aps.numberNDPs);
	printf("Total lenght of non-dominated line segments: %.2lf\n", aps.lengthNDLS);
	printf("Total proportion of resolved areas: %.2lf\n", aps.proportionSolvedArea);
	printf("Total number of integer frontier: %d\n", aps.numberOfIntegerSolutions);
	printf("\n------------------------ Approx Solution ----------------------------------\n");

	approxSolVector.push_back(aps);

}

bool SequentialAlgorithm::lexminObj1Obj2(NDP& p, double p1x, double p1y, double p2x, double p2y, bool addMIPStart, NDP& s, bool nogoodinnerloop)
{
	lexArea = fabs(p2x - p1x) * fabs(p1y - p2y);

	if ((fabs(p1x - p2x) < EPS) || (fabs(p1y - p2y) < EPS))
	{
		printf("\nSmall rectangle found-1!\n");
		//exit(0);
	}
	if ((p1x > p2x) || (p1y < p2y))
	{
		printf("ERROR: Infeasible rectangle found-1!\n");
		exit(0);
	}

	newSolver->addObjective1();

#ifdef ALLCONSTRAINTS
	newSolver->addConstObj(p1x, p1y, p2x, p2y);
#else
	newSolver->addConstObj2(p1y);
	if (nogoodinnerloop) newSolver->addConstObj1(p2x);
#endif

	if (addMIPStart && ((int)s.continuousSolution.size() <= 0))
	{
		printf("ERROR: addMIPStart-1!\n");
		exit(0);
	}

	if (addMIPStart)  newSolver->addMIPStart(s.continuousSolution, s.integerSolution);

	if (!newSolver->run())
	{
		if (nogoodinnerloop)
		{
			newSolver->removeObjective1();
#ifdef ALLCONSTRAINTS
			newSolver->removeConstObj();
#else
			newSolver->removeConstObj2();
			newSolver->removeConstObj1();
#endif
			//newSolver->removeWeightedSumConstObj(); 

			return false;
		}

		exit(0);
	}

	p.x = newSolver->getObjValue();

#ifdef ALLCONSTRAINTS
	p2x = p.x;
#endif

	double temp = newSolver->getObj2Value();
	//if (print) printf("\npx = %lf; py = %lf\n", p.x, newSolver->getObj2Value());

	//if (addMIPStart)
	{
		p.continuousSolution = newSolver->getContinuousVariablesSolution();
		p.integerSolution = newSolver->getIntegerVariablesSolution();

	}
	//newSolver->printSolution(); 

	newSolver->removeObjective1();
#ifdef ALLCONSTRAINTS
	newSolver->removeConstObj();
#else
	newSolver->removeConstObj2();
	if (nogoodinnerloop) newSolver->removeConstObj1();
#endif

	newSolver->addObjective2();

#ifdef ALLCONSTRAINTS
	//newSolver->addConstObj(p1x, p1y, p2x, p2y); 
	newSolver->addConstObj(p2x - EPS, p1y, p2x, p2y);
#else
	newSolver->addConstObj1(p.x);
#endif

	if (addMIPStart)  newSolver->addMIPStart(p.continuousSolution, p.integerSolution);

	if (!newSolver->run())
	{
		printf("ERROR2\n");
		//exit(0);
		p.y = temp;
	}
	else
	{
		p.y = newSolver->getObjValue();

		//newSolver->printSolution();

		p.continuousSolution = newSolver->getContinuousVariablesSolution();
		p.integerSolution = newSolver->getIntegerVariablesSolution();
	}

	//newSolver->printSolution();

	newSolver->removeObjective2();
#ifdef ALLCONSTRAINTS
	newSolver->removeConstObj();
#else
	newSolver->removeConstObj1();
#endif

	return true;
}

bool SequentialAlgorithm::lexminObj2Obj1(NDP& p, double p1x, double p1y, double p2x, double p2y, bool addMIPStart, NDP& s, bool nogoodinnerloop)
{
	lexArea = fabs(p2x - p1x) * fabs(p1y - p2y);

	if ((fabs(p1x - p2x) < EPS) || (fabs(p1y - p2y) < EPS))
	{
		printf("\nSmall rectangle found-3!\n");
		//exit(0);
	}
	if ((p1x > p2x) || (p1y < p2y))
	{
		printf("ERROR: Infeasible rectangle found-3!\n");
		exit(0);
	}

	newSolver->addObjective2();
#ifdef ALLCONSTRAINTS
	newSolver->addConstObj(p1x, p1y, p2x, p2y);
#else
	newSolver->addConstObj1(p2x);
	if (nogoodinnerloop) newSolver->addConstObj2(p1y);
#endif

	if (addMIPStart && ((int)s.continuousSolution.size() <= 0))
	{
		printf("ERROR: addMIPStart!\n");
		exit(0);
	}

	if (addMIPStart)  newSolver->addMIPStart(s.continuousSolution, s.integerSolution);

	if (!newSolver->run())
	{
		if (nogoodinnerloop)
		{
			newSolver->removeObjective2();
#ifdef ALLCONSTRAINTS
			newSolver->removeConstObj();
#else
			newSolver->removeConstObj1();
			newSolver->removeConstObj2();
#endif
			//newSolver->removeWeightedSumConstObj(); // test

			return false;
		}

		printf("ERROR3\n");
		exit(0);
	}

	p.y = newSolver->getObjValue();

#ifdef ALLCONSTRAINTS
	p1y = p.y;
#endif

	//if (addMIPStart)
	{
		p.continuousSolution = newSolver->getContinuousVariablesSolution();
		p.integerSolution = newSolver->getIntegerVariablesSolution();
	}

	//newSolver->printSolution();

	newSolver->removeObjective2();
#ifdef ALLCONSTRAINTS
	newSolver->removeConstObj();
	//if (nogoodinnerloop) newSolver->removeWeightedSumConstObj(); // test
#else
	newSolver->removeConstObj1();
	if (nogoodinnerloop) newSolver->removeConstObj2();
#endif

	newSolver->addObjective1();
#ifdef ALLCONSTRAINTS
	//newSolver->addConstObj(p1x, p1y, p2x, p2y);  
	newSolver->addConstObj(p1x, p1y, p2x, p1y - EPS);
#else
	newSolver->addConstObj2(p.y);
#endif

	if (addMIPStart)  newSolver->addMIPStart(p.continuousSolution, p.integerSolution);

	if (!newSolver->run())
	{
		printf("ERROR4\n");
		//exit(0);
	}
	else
	{
		p.x = newSolver->getObjValue();

		p.continuousSolution = newSolver->getContinuousVariablesSolution();
		p.integerSolution = newSolver->getIntegerVariablesSolution();
	}

	//newSolver->printSolution();

	newSolver->removeObjective1();

#ifdef ALLCONSTRAINTS
	newSolver->removeConstObj();
#else
	newSolver->removeConstObj2();
#endif

	return true;
}

void SequentialAlgorithm::scalMinUpdate(double time, double area)
{
	scalMin sm;
	sm.time = time;
	sm.area = area;
	scalMinVector.push_back(sm);
}

void SequentialAlgorithm::lexMinUpdate(double time, double area)
{
	lexMin lm;
	lm.time = time;
	lm.area = area;
	lexMinVector.push_back(lm);
}

bool SequentialAlgorithm::equalPoints(NDP& p1, NDP& p2)
{
	if ((fabs(p1.x - p2.x) < EPS) && (fabs(p1.y - p2.y) < EPS)) return true;
	return false;
}

double SequentialAlgorithm::minPointsCoord(NDP& p1, NDP& p2)
{
	double deltax = fabs(p1.x - p2.x);
	double deltay = fabs(p1.y - p2.y);

	return ((deltax <= deltay) ? deltax : deltay);
}

double SequentialAlgorithm::maxPointsCoord(NDP& p1, NDP& p2)
{
	double deltax = fabs(p1.x - p2.x);
	double deltay = fabs(p1.y - p2.y);

	return ((deltax >= deltay) ? deltax : deltay);
}

bool SequentialAlgorithm::isSmallRectangle(double p1x, double p1y, double p2x, double p2y)
{
	double deltax = fabs(p1x - p2x);
	double deltay = fabs(p1y - p2y);

	return ((deltax < EPS) || (deltay < EPS));
}

bool SequentialAlgorithm::isSameFrontier(NDP& p1, NDP& p2)
{
	for (int i = 0; i < p1.integerSolution.size(); i++)
		if (p1.integerSolution[i] != p2.integerSolution[i]) return false;

	return true;
}


void SequentialAlgorithm::restrictiveFrontierAlgorithm()
{

	std::vector<std::vector<NDP> > frontierPointsSet;
	while (!stack.empty())
	{
		frontierPointsSet.clear();

		printf("NEWALGO| Number of active rectangles: %d\n\n", (int)stack.size());

		Rectangle r = stack.back();
		stack.pop_back();

		if (print)  printf("NEWALGO| Exploring rectangle (%lf, %lf) (%lf, %lf)\n", r._p1.x, r._p1.y, r._p2.x, r._p2.y);

#ifdef APPROXSOLUTIONSTATISTICS 
		timeSoFar.stop();
		totalTimeSoFar += timeSoFar.getCPUTotalSecs();

		if (totalTimeSoFar >= approxTotalTime)
		{
			timeTotalTime.stop();
			totalTime += timeTotalTime.getCPUTotalSecs();

			while (!pqueue.empty())  pqueue.pop();

			for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

			Rectangle req(r._p1, r._p2);
			pqueue.push(req);

			approximateSolution(totalTimeSoFar);

			while (!pqueue.empty())  pqueue.pop();

			approxTotalTime *= 2;

			timeTotalTime.reset();
			timeTotalTime.start();
		}

		timeSoFar.reset();
		timeSoFar.start();
#endif

		if (isSameFrontier(r._p1, r._p2))
		{
			numberOfRectanglesEqualIPs++;

			printf("Same Frontier\n");

#ifdef IMPROVEMENTNOGOOD
			numberOfScalarizationWithNoGoodIP++;
			timeScalarizationWithNoGoodIP.reset();
			timeScalarizationWithNoGoodIP.start();

			newSolver->addNoGoodConstraint((r._p1).integerSolution);

			NDP ystar;

			bool foundNoGood = scalarize(r._p1, r._p2, ystar, r._p1, r._p2, true);

			timeScalarizationWithNoGoodIP.stop();
			totalTimeScalarizationWithNoGoodIP += timeScalarizationWithNoGoodIP.getCPUTotalSecs();

			if (print) printf("Scal Time = %lf\n", timeScalarizationWithNoGoodIP.getCPUTotalSecs());
			if (print) printf("Scal Area = %lf\n", scalArea);
			scalMinUpdate(timeScalarizationWithNoGoodIP.getCPUTotalSecs(), scalArea);

			if (maxTimeScal < timeScalarizationWithNoGoodIP.getCPUTotalSecs()) maxTimeScal = timeScalarizationWithNoGoodIP.getCPUTotalSecs();

			newSolver->removeNoGoodConstraint();

			if (!foundNoGood)
			{
				buildLPFrontierForNoGood(r._p1, r._p2);
				continue;
			}
			/*
						else
						{
							NDP p;
							NDP wstar;
							if (!lineGenSameFrontierWithNoGood(p, ystar, r._p1, r._p2)) wstar = ystar;
							else wstar = p;

							p1 = wstar;
						}
			*/
#endif
		}

		NDP previous = r._p1;
		NDP current = r._p1;

		numberOfRectanglesSolved++;

		NDP z1, z2;
		//lineGen(z1, z2, r._p1, r._p1, r._p2);

		std::vector<NDP> frontierPoints;
		frontierPoints.clear();

		buildLPFrontierForSoyluAlgorithm(current, r._p2, frontierPoints);
		frontierPointsSet.push_back(frontierPoints);

		z1 = frontierPoints[0];
		z2 = z1;
		if ((int)frontierPoints.size() > 1)  z2 = frontierPoints[1];

#ifdef APPROXSOLUTIONSTATISTICS 
		timeSoFar.stop();
		totalTimeSoFar += timeSoFar.getCPUTotalSecs();

		if (totalTimeSoFar >= approxTotalTime)
		{
			timeTotalTime.stop();
			totalTime += timeTotalTime.getCPUTotalSecs();

			while (!pqueue.empty())  pqueue.pop();

			for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

			Rectangle req(current, r._p2);
			pqueue.push(req);

			approximateSolution(totalTimeSoFar);

			while (!pqueue.empty())  pqueue.pop();

			approxTotalTime *= 2;

			timeTotalTime.reset();
			timeTotalTime.start();
		}

		timeSoFar.reset();
		timeSoFar.start();
#endif


		if (!equalPoints(z1, z2))
		{
			NDP zL;
			newSolver->addNoGoodConstraint(z1.integerSolution);
			bool ok = restrictiveLineToTheRight(zL, z1, z2, r._p1, r._p2);

#ifdef APPROXSOLUTIONSTATISTICS 
			timeSoFar.stop();
			totalTimeSoFar += timeSoFar.getCPUTotalSecs();

			if (totalTimeSoFar >= approxTotalTime)
			{
				timeTotalTime.stop();
				totalTime += timeTotalTime.getCPUTotalSecs();

				while (!pqueue.empty())  pqueue.pop();

				for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

				Rectangle req(r._p1, r._p2);
				pqueue.push(req);

				approximateSolution(totalTimeSoFar);

				while (!pqueue.empty())  pqueue.pop();

				approxTotalTime *= 2;

				timeTotalTime.reset();
				timeTotalTime.start();
			}

			timeSoFar.reset();
			timeSoFar.start();
#endif

			newSolver->removeNoGoodConstraint();
			if (!ok)
			{
				NDP z1c, z2c;
				//lineGen(z1c, z2c, zL, zL, r._p2);

				std::vector<NDP> frontierPoints2;
				frontierPoints2.clear();
				buildLPFrontierForSoyluAlgorithm(zL, r._p2, frontierPoints2);
				frontierPointsSet.push_back(frontierPoints2);

				z1c = frontierPoints2[0];
				z2c = z1c;
				if ((int)frontierPoints2.size() > 1)  z2c = frontierPoints2[1];

				double lambda1 = fabs(z1.y - z2.y);
				double lambda2 = fabs(z2.x - z1.x);

				if (lambda1 * z1.x + lambda2 * z1.y <= (lambda1 * z2c.x + lambda2 * z2c.y) + EPS)
				{
					previous = zL;
					current = r._p1;
				}
				else
				{
					previous = r._p1;
					current = zL;
				}
			}
		}

		/*
				timeSoFar.stop();
				totalTimeSoFar += timeSoFar.getCPUTotalSecs();

				if (print) printf("Time so far: %lf\n\n", totalTimeSoFar);

				timeSoFar.reset();
				timeSoFar.start();
		*/

		while (true)
		{
			timeSoFar.stop();
			totalTimeSoFar += timeSoFar.getCPUTotalSecs();

#ifdef APPROXSOLUTIONSTATISTICS 
			if (totalTimeSoFar >= approxTotalTime)
			{
				timeTotalTime.stop();
				totalTime += timeTotalTime.getCPUTotalSecs();

				while (!pqueue.empty())  pqueue.pop();

				for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

				Rectangle req(current, r._p2);
				pqueue.push(req);

				approximateSolution(totalTimeSoFar);

				while (!pqueue.empty())  pqueue.pop();

				approxTotalTime *= 2;

				timeTotalTime.reset();
				timeTotalTime.start();
			}
#endif

			if (print) printf("Time so far: %lf\n\n", totalTimeSoFar);

			timeSoFar.reset();
			timeSoFar.start();

			if (isSmallRectangle(current.x, current.y, r._p2.x, r._p2.y))
			{
				if (print) printf("\n\nFinishing the box!!!\n\n");
				break;
			}

			bool found = false;
			int idx = -1;
			for (int i = 0; i < (int)frontierPointsSet.size(); i++)
			{
				if (isSameFrontier(frontierPointsSet[i][0], current))
				{
					found = true;
					idx = i;
					break;
				}
			}

			std::vector<NDP> frontierPoints;
			frontierPoints.clear();
			if (!found)
			{
				buildLPFrontierForSoyluAlgorithm(current, r._p2, frontierPoints);
				frontierPointsSet.push_back(frontierPoints);
			}
			else
			{
				int k = -1;
				for (int j = 0; j < (int)frontierPointsSet[idx].size(); j++)
				{
					if (!equalPoints(frontierPointsSet[idx][j], current) && (frontierPointsSet[idx][j].x > current.x))
					{
						k = j;
						break;
					}
				}

				frontierPoints.push_back(current);
				if (k != -1) for (int j = k; j < (int)frontierPointsSet[idx].size(); j++) frontierPoints.push_back(frontierPointsSet[idx][j]);

				frontierPointsSet[idx] = frontierPoints;
			}


			if ((int)frontierPoints.size() > 1)
			{
				newSolver->addNoGoodConstraint(current.integerSolution);

				NDP zL;
				bool ok = restrictiveFrontierToTheRight(zL, frontierPoints, current, r._p2, previous);

				newSolver->removeNoGoodConstraint();

				if (ok)
				{
					for (int it = 0; it < (int)frontierPoints.size() - 1; it++)
					{
						frontierPoints[it].connected = true;
						ndpList.push_back(frontierPoints[it]);
					}

					NDP z2 = frontierPoints[(int)frontierPoints.size() - 1];

					z2.connected = false;
					ndpList.push_back(z2);

					if (print) printf("\nNEWALGO| z1: (%lf, %lf)\n", frontierPoints[0].x, frontierPoints[0].y);
					if (print) printf("NEWALGO| z2: (%lf, %lf)\n\n", z2.x, z2.y);

					previous = frontierPoints[0];
					current = zL;

					if (zL.integerSolution != frontierPoints[0].integerSolution)
					{
						if (!z2.closed) previous = current;
					}
				}
				else
				{
					previous = zL;

					if (isSameFrontier(previous, current)) printf("previous is same frontier of current\n");
				}
			}
			else
			{
				if (print) printf("\n\nz1 equal z2!\n\n");

#ifdef APPROXSOLUTIONSTATISTICS 
				timeSoFar.stop();
				totalTimeSoFar += timeSoFar.getCPUTotalSecs();

				if (totalTimeSoFar >= approxTotalTime)
				{
					timeTotalTime.stop();
					totalTime += timeTotalTime.getCPUTotalSecs();

					while (!pqueue.empty())  pqueue.pop();

					for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

					Rectangle req(current, r._p2);
					pqueue.push(req);

					approximateSolution(totalTimeSoFar);

					while (!pqueue.empty())  pqueue.pop();

					approxTotalTime *= 2;

					timeTotalTime.reset();
					timeTotalTime.start();
				}

				timeSoFar.reset();
				timeSoFar.start();
#endif

				current.connected = false;
				current.closed = true;
				ndpList.push_back(current);

				newSolver->addNoGoodConstraint(current.integerSolution);

				NDP p1;
				p1.closed = true;

				numberOfLexicographicIP++;
				timeLexicographicIP.reset();
				timeLexicographicIP.start();
				lexminObj1Obj2(p1, current.x, current.y, r._p2.x, r._p2.y, true, r._p2);
				timeLexicographicIP.stop();
				totalTimeLexicographicIP += timeLexicographicIP.getCPUTotalSecs();

				newSolver->removeNoGoodConstraint();

				if (fabs(current.y - p1.y) <= EPS) p1.closed = false;

				previous = current;
				current = p1;

				if (print) printf("OuterLoop| Lex Time = %lf\n", timeLexicographicIP.getCPUTotalSecs());
				if (print) printf("OuterLoop| Lex Area = %lf\n", lexArea);
				lexMinUpdate(timeLexicographicIP.getCPUTotalSecs(), lexArea);

				if (maxTimeLex < timeLexicographicIP.getCPUTotalSecs()) maxTimeLex = timeLexicographicIP.getCPUTotalSecs();
			}
		}
	}
}


bool SequentialAlgorithm::restrictiveFrontierToTheRight(NDP& zL, std::vector<NDP>& frontierPoints, NDP& L, NDP& R, NDP& previous)
{

	NDP z1 = frontierPoints[0];
	NDP z2 = frontierPoints[(int)frontierPoints.size() - 1];

	if (!isSameFrontier(z1, z2))
	{
		printf("z1 NOT from same frontier than z2\n");
		exit(0);
	}

	if (print) printf("RestrictiveLine| z1: (%lf, %lf)\n", z1.x, z1.y);
	if (print) printf("RestrictiveLine| z2: (%lf, %lf)\n", z2.x, z2.y);
	if (print) printf("RestrictiveLine| Number Line Segments = %d\n", (int)frontierPoints.size() - 1);

	numberOfPieceWiseLinearSolves++;
	numberOfSegmentsPieceWiseLinear += (int)frontierPoints.size() - 1;
	numberCallsInnerLoop++;

	timeInnerLoop.reset();
	timeInnerLoop.start();

	newSolver->createAddFrontierConst(frontierPoints, previous.integerSolution);

	zL.closed = true;
	zL.connected = false;

	if (print) printf("RestrictiveLine| Searching for zL in r = (%lf %lf) (%lf %lf)\n", z1.x, z1.y, z2.x, R.y);

	timeOfInnerLoopWithNoGoodIP.reset();
	timeOfInnerLoopWithNoGoodIP.start();

	//CPUTimer timetest1;

	//timetest1.start();

	bool zLfound = lexminObj1Obj2(zL, z1.x, z1.y - 1.0e-4, z2.x, R.y, false, R, true);

	//timetest1.stop();

	//printf("\nTime1= %lf\n", timetest1.getCPUTotalSecs());

	if (zLfound) numberOfInnerLoopWithNoGoodIP += 2;
	else numberOfInnerLoopWithNoGoodIP++;

	timeOfInnerLoopWithNoGoodIP.stop();
	totalTimeOfInnerLoopWithNoGoodIP += timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs();

	if (print) printf("RestrictiveLine| Lex Time = %lf\n", timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs());

	if (maxTimeLex < timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs()) maxTimeLex = timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs();

	if (zLfound) { if (print) printf("RestrictiveLine| zL found: (%lf %lf)\n", zL.x, zL.y); }
	else { if (print) printf("RestrictiveLine| zL NOT found\n"); }

	bool ok;
	if (!zLfound) // || equalPoints(zL,z1))// || (wTzstar <= lambda1 * zL.x + lambda2 * zL.y + EPS))  
	{
		ok = true;
		zL = z2;

		if (print) printf("RestrictiveLine| z2.closed = true\n");
	}
	else
	{
		if (equalPoints(zL, z1))
		{
			ok = false;
		}
		else
		{
			ok = true;

			int k = -1;
			for (int j = 0; j < (int)frontierPoints.size(); j++)
			{
				if ((frontierPoints[j].x > zL.x) || (fabs(frontierPoints[j].x - zL.x) < EPS))
				{
					k = j;
					break;
				}
			}

			segmentSolutionWasFound += k;
			numberOfPieceWiseLinearSolvesIfFeasible++;

			double lambda1 = fabs(frontierPoints[k - 1].y - frontierPoints[k].y);
			double lambda2 = fabs(frontierPoints[k].x - frontierPoints[k - 1].x);

			double wTzstar = lambda1 * frontierPoints[k - 1].x + lambda2 * frontierPoints[k - 1].y;

			if (fabs((lambda1 * zL.x + lambda2 * zL.y) - wTzstar) <= 1.0e-2)
			{
				frontierPoints[k].closed = true;

				frontierPoints[k].x = zL.x;
				frontierPoints[k].y = zL.y;

				if (print) printf("RestrictiveLine| z2.closed = true\n");
			}
			else
			{
				frontierPoints[k].closed = false;

				frontierPoints[k].x = zL.x;
				frontierPoints[k].y = (wTzstar - lambda1 * zL.x) / lambda2;

				if (print) printf("RestrictiveLine| z2.closed = false\n");
			}

			std::vector<NDP> frontierPointsCopy;
			for (int j = 0; j <= k; j++) frontierPointsCopy.push_back(frontierPoints[j]);

			frontierPoints.clear();
			for (int j = 0; j <= k; j++) frontierPoints.push_back(frontierPointsCopy[j]);

		}
	}

	newSolver->removeFrontierConst();

	timeInnerLoop.stop();
	totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();

	return ok;
}


void SequentialAlgorithm::restrictiveLineAlgorithm()
{

	std::vector<std::vector<NDP> > frontierPointsSet;
	while (!stack.empty())
	{
		frontierPointsSet.clear();

		printf("NEWALGO| Number of active rectangles: %d\n\n", (int)stack.size());

		Rectangle r = stack.back();
		stack.pop_back();

		if (print)  printf("NEWALGO| Exploring rectangle (%lf, %lf) (%lf, %lf)\n", r._p1.x, r._p1.y, r._p2.x, r._p2.y);


#ifdef APPROXSOLUTIONSTATISTICS 
		timeSoFar.stop();
		totalTimeSoFar += timeSoFar.getCPUTotalSecs();

		if (totalTimeSoFar >= approxTotalTime)
		{
			timeTotalTime.stop();
			totalTime += timeTotalTime.getCPUTotalSecs();

			while (!pqueue.empty())  pqueue.pop();

			for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

			Rectangle req(r._p1, r._p2);
			pqueue.push(req);

			approximateSolution(totalTimeSoFar);

			while (!pqueue.empty())  pqueue.pop();

			approxTotalTime *= 2;

			timeTotalTime.reset();
			timeTotalTime.start();
		}

		timeSoFar.reset();
		timeSoFar.start();
#endif


		if (isSameFrontier(r._p1, r._p2))
		{
			numberOfRectanglesEqualIPs++;

			printf("Same Frontier\n");

#ifdef IMPROVEMENTNOGOOD
			numberOfScalarizationWithNoGoodIP++;
			timeScalarizationWithNoGoodIP.reset();
			timeScalarizationWithNoGoodIP.start();

			newSolver->addNoGoodConstraint((r._p1).integerSolution);

			NDP ystar;

			bool foundNoGood = scalarize(r._p1, r._p2, ystar, r._p1, r._p2, true);

			timeScalarizationWithNoGoodIP.stop();
			totalTimeScalarizationWithNoGoodIP += timeScalarizationWithNoGoodIP.getCPUTotalSecs();

			if (print) printf("Scal Time = %lf\n", timeScalarizationWithNoGoodIP.getCPUTotalSecs());
			if (print) printf("Scal Area = %lf\n", scalArea);
			scalMinUpdate(timeScalarizationWithNoGoodIP.getCPUTotalSecs(), scalArea);

			if (maxTimeScal < timeScalarizationWithNoGoodIP.getCPUTotalSecs()) maxTimeScal = timeScalarizationWithNoGoodIP.getCPUTotalSecs();

			newSolver->removeNoGoodConstraint();

			if (!foundNoGood)
			{
				buildLPFrontierForNoGood(r._p1, r._p2);
				continue;
			}
			/*
						else
						{
							NDP p;
							NDP wstar;
							if (!lineGenSameFrontierWithNoGood(p, ystar, r._p1, r._p2)) wstar = ystar;
							else wstar = p;

							p1 = wstar;
						}
			*/
#endif
		}

		NDP previous = r._p1;
		NDP current = r._p1;

		numberOfRectanglesSolved++;

		NDP z1, z2;
		//lineGen(z1, z2, r._p1, r._p1, r._p2);

		std::vector<NDP> frontierPoints;
		frontierPoints.clear();

		buildLPFrontierForSoyluAlgorithm(current, r._p2, frontierPoints);
		frontierPointsSet.push_back(frontierPoints);

		z1 = frontierPoints[0];
		z2 = z1;
		if ((int)frontierPoints.size() > 1)  z2 = frontierPoints[1];

#ifdef APPROXSOLUTIONSTATISTICS 
		timeSoFar.stop();
		totalTimeSoFar += timeSoFar.getCPUTotalSecs();

		if (totalTimeSoFar >= approxTotalTime)
		{
			timeTotalTime.stop();
			totalTime += timeTotalTime.getCPUTotalSecs();

			while (!pqueue.empty())  pqueue.pop();

			for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

			Rectangle req(current, r._p2);
			pqueue.push(req);

			approximateSolution(totalTimeSoFar);

			while (!pqueue.empty())  pqueue.pop();

			approxTotalTime *= 2;

			timeTotalTime.reset();
			timeTotalTime.start();
		}

		timeSoFar.reset();
		timeSoFar.start();
#endif


		if (!equalPoints(z1, z2))
		{
			NDP zL;
			newSolver->addNoGoodConstraint(z1.integerSolution);
			bool ok = restrictiveLineToTheRight(zL, z1, z2, r._p1, r._p2);

#ifdef APPROXSOLUTIONSTATISTICS 
			timeSoFar.stop();
			totalTimeSoFar += timeSoFar.getCPUTotalSecs();

			if (totalTimeSoFar >= approxTotalTime)
			{
				timeTotalTime.stop();
				totalTime += timeTotalTime.getCPUTotalSecs();

				while (!pqueue.empty())  pqueue.pop();

				for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

				Rectangle req(r._p1, r._p2);
				pqueue.push(req);

				approximateSolution(totalTimeSoFar);

				while (!pqueue.empty())  pqueue.pop();

				approxTotalTime *= 2;

				timeTotalTime.reset();
				timeTotalTime.start();
			}

			timeSoFar.reset();
			timeSoFar.start();
#endif

			newSolver->removeNoGoodConstraint();
			if (!ok)
			{
				NDP z1c, z2c;
				//lineGen(z1c, z2c, zL, zL, r._p2);

				std::vector<NDP> frontierPoints2;
				frontierPoints2.clear();
				buildLPFrontierForSoyluAlgorithm(zL, r._p2, frontierPoints2);
				frontierPointsSet.push_back(frontierPoints2);

				z1c = frontierPoints2[0];
				z2c = z1c;
				if ((int)frontierPoints2.size() > 1)  z2c = frontierPoints2[1];

				double lambda1 = fabs(z1.y - z2.y);
				double lambda2 = fabs(z2.x - z1.x);

				if (lambda1 * z1.x + lambda2 * z1.y <= (lambda1 * z2c.x + lambda2 * z2c.y) + EPS)
				{
					previous = zL;
					current = r._p1;
				}
				else
				{
					previous = r._p1;
					current = zL;
				}
			}
		}

		/*
				timeSoFar.stop();
				totalTimeSoFar += timeSoFar.getCPUTotalSecs();

				if (print) printf("Time so far: %lf\n\n", totalTimeSoFar);

				timeSoFar.reset();
				timeSoFar.start();
		*/

		while (true)
		{
			timeSoFar.stop();
			totalTimeSoFar += timeSoFar.getCPUTotalSecs();

#ifdef APPROXSOLUTIONSTATISTICS 
			if (totalTimeSoFar >= approxTotalTime)
			{
				timeTotalTime.stop();
				totalTime += timeTotalTime.getCPUTotalSecs();

				while (!pqueue.empty())  pqueue.pop();

				for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

				Rectangle req(current, r._p2);
				pqueue.push(req);

				approximateSolution(totalTimeSoFar);

				while (!pqueue.empty())  pqueue.pop();

				approxTotalTime *= 2;

				timeTotalTime.reset();
				timeTotalTime.start();
			}
#endif

			if (print) printf("Time so far: %lf\n\n", totalTimeSoFar);

			timeSoFar.reset();
			timeSoFar.start();

			if (isSmallRectangle(current.x, current.y, r._p2.x, r._p2.y))
			{
				if (print) printf("\n\nFinishing the box!!!\n\n");
				break;
			}

			bool found = false;
			int idx = -1;
			for (int i = 0; i < (int)frontierPointsSet.size(); i++)
			{
				if (isSameFrontier(frontierPointsSet[i][0], current))
				{
					found = true;
					idx = i;
					break;
				}
			}

			std::vector<NDP> frontierPoints;
			frontierPoints.clear();
			if (!found)
			{
				buildLPFrontierForSoyluAlgorithm(current, r._p2, frontierPoints);
				frontierPointsSet.push_back(frontierPoints);
			}
			else
			{
				int k = -1;
				for (int j = 0; j < (int)frontierPointsSet[idx].size(); j++)
				{
					if (!equalPoints(frontierPointsSet[idx][j], current) && (frontierPointsSet[idx][j].x > current.x))
					{
						k = j;
						break;
					}
				}

				frontierPoints.push_back(current);
				if (k != -1) for (int j = k; j < (int)frontierPointsSet[idx].size(); j++) frontierPoints.push_back(frontierPointsSet[idx][j]);

				frontierPointsSet[idx] = frontierPoints;
			}


			if ((int)frontierPoints.size() > 1)
			{
				if (print) printf("\n\nz1 diff z2!\n\n");

				for (int it = 0; it < (int)frontierPoints.size() - 1; it++)
				{

#ifdef APPROXSOLUTIONSTATISTICS 
					timeSoFar.stop();
					totalTimeSoFar += timeSoFar.getCPUTotalSecs();

					if (totalTimeSoFar >= approxTotalTime)
					{
						timeTotalTime.stop();
						totalTime += timeTotalTime.getCPUTotalSecs();

						while (!pqueue.empty())  pqueue.pop();

						for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

						Rectangle req(current, r._p2);
						pqueue.push(req);

						approximateSolution(totalTimeSoFar);

						while (!pqueue.empty())  pqueue.pop();

						approxTotalTime *= 2;

						timeTotalTime.reset();
						timeTotalTime.start();
					}

					timeSoFar.reset();
					timeSoFar.start();
#endif

					NDP zL;
					NDP z1, z2;

					z1 = frontierPoints[it];
					z2 = frontierPoints[it + 1];

					if (isSameFrontier(previous, current))
					{
						newSolver->addNoGoodConstraint(current.integerSolution);
					}
					else
					{
						newSolver->addNoGoodConstraint2(previous.integerSolution);
						newSolver->addNoGoodConstraint(current.integerSolution);
					}

					bool ok = restrictiveLineToTheRight(zL, z1, z2, current, r._p2);

					if (isSameFrontier(previous, current))
					{
						newSolver->removeNoGoodConstraint();
					}
					else
					{
						newSolver->removeNoGoodConstraint2();
						newSolver->removeNoGoodConstraint();
					}

					if (ok)
					{
						z1.connected = true;
						ndpList.push_back(z1);
						z2.connected = false;
						ndpList.push_back(z2);

						if (print) printf("\nNEWALGO| z1: (%lf, %lf)\n", z1.x, z1.y);
						if (print) printf("NEWALGO| z2: (%lf, %lf)\n\n", z2.x, z2.y);

						previous = z1;
						current = zL;

						if (zL.integerSolution != z1.integerSolution)
						{
							if (!z2.closed) previous = current;
							break;
						}
					}
					else
					{
						previous = zL;
					}
				}
			}
			else
			{
				if (print) printf("\n\nz1 equal z2!\n\n");

#ifdef APPROXSOLUTIONSTATISTICS 
				timeSoFar.stop();
				totalTimeSoFar += timeSoFar.getCPUTotalSecs();

				if (totalTimeSoFar >= approxTotalTime)
				{
					timeTotalTime.stop();
					totalTime += timeTotalTime.getCPUTotalSecs();

					while (!pqueue.empty())  pqueue.pop();

					for (int i = 0; i < (int)stack.size(); i++) pqueue.push(stack[i]);

					Rectangle req(current, r._p2);
					pqueue.push(req);

					approximateSolution(totalTimeSoFar);

					while (!pqueue.empty())  pqueue.pop();

					approxTotalTime *= 2;

					timeTotalTime.reset();
					timeTotalTime.start();
				}

				timeSoFar.reset();
				timeSoFar.start();
#endif

				current.connected = false;
				current.closed = true;
				ndpList.push_back(current);

				newSolver->addNoGoodConstraint(current.integerSolution);

				NDP p1;
				p1.closed = true;

				numberOfLexicographicIP++;
				timeLexicographicIP.reset();
				timeLexicographicIP.start();
				lexminObj1Obj2(p1, current.x, current.y, r._p2.x, r._p2.y, true, r._p2);
				timeLexicographicIP.stop();
				totalTimeLexicographicIP += timeLexicographicIP.getCPUTotalSecs();

				newSolver->removeNoGoodConstraint();

				if (fabs(current.y - p1.y) <= EPS) p1.closed = false;

				previous = current;
				current = p1;

				if (print) printf("OuterLoop| Lex Time = %lf\n", timeLexicographicIP.getCPUTotalSecs());
				if (print) printf("OuterLoop| Lex Area = %lf\n", lexArea);
				lexMinUpdate(timeLexicographicIP.getCPUTotalSecs(), lexArea);

				if (maxTimeLex < timeLexicographicIP.getCPUTotalSecs()) maxTimeLex = timeLexicographicIP.getCPUTotalSecs();
			}
		}
	}
}

bool SequentialAlgorithm::restrictiveLineToTheRight(NDP& zL, NDP& z1, NDP& z2, NDP& L, NDP& R)
{
	numberCallsInnerLoop++;

	timeInnerLoop.reset();
	timeInnerLoop.start();

	if (print) printf("RestrictiveLine| z1: (%lf, %lf)\n", z1.x, z1.y);
	if (print) printf("RestrictiveLine| z2: (%lf, %lf)\n", z2.x, z2.y);

	if (equalPoints(z1, z2))
	{
		printf("\nRestrictiveLine| Error Restrictive Line-1!\n");
		exit(0);
	}

	z1.closed = z2.closed = true;

	double lambda1 = fabs(z1.y - z2.y);
	double lambda2 = fabs(z2.x - z1.x);

	double triangleArea = (fabs(z1.y - z2.y) * fabs(z2.x - z1.x)) / 2;

	if (print) printf("RestrictiveLine| [lambda1 = %lf; lambda2 = %lf]\n", lambda1, lambda2);

	double wTzstar = lambda1 * z1.x + lambda2 * z1.y;
	if (print) printf("RestrictiveLine| wTzstar = %lf\n", wTzstar);

	newSolver->createAddWeightedSumConstObj(lambda1, lambda2, wTzstar);
	//newSolver->createAddConvexCombination(z1.x, z1.y, z2.x, z2.y); 

	zL.closed = true;
	zL.connected = false;

	if (print) printf("RestrictiveLine| Searching for zL in r = (%lf %lf) (%lf %lf)\n", z1.x, z1.y, z2.x, R.y);

	timeOfInnerLoopWithNoGoodIP.reset();
	timeOfInnerLoopWithNoGoodIP.start();

	bool zLfound = lexminObj1Obj2(zL, z1.x, z1.y, z2.x, R.y, false, R, true);

	if (zLfound) numberOfInnerLoopWithNoGoodIP += 2;
	else numberOfInnerLoopWithNoGoodIP++;

	timeOfInnerLoopWithNoGoodIP.stop();
	totalTimeOfInnerLoopWithNoGoodIP += timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs();

	if (print) printf("RestrictiveLine| Lex Time = %lf\n", timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs());
	if (print) printf("RestrictiveLine| Lex Area = %lf\n", fabs(lexArea - triangleArea));
	lexMinUpdate(timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs(), fabs(lexArea - triangleArea));

	if (maxTimeLex < timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs()) maxTimeLex = timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs();

	if (zLfound) { if (print) printf("RestrictiveLine| zL found: (%lf %lf)\n", zL.x, zL.y); }
	else { if (print) printf("RestrictiveLine| zL NOT found\n"); }

	bool ok;
	if (!zLfound) // || equalPoints(zL,z1))// || (wTzstar <= lambda1 * zL.x + lambda2 * zL.y + EPS))  
	{
		ok = true;
		zL = z2;

		if (print) printf("RestrictiveLine| z2.closed = true\n");
	}
	else
	{
		if (equalPoints(zL, z1))
		{
			ok = false;
		}
		else
		{
			ok = true;

			if (fabs((lambda1 * zL.x + lambda2 * zL.y) - wTzstar) <= EPS)
			{
				z2.x = zL.x;
				z2.y = zL.y;
			}
			else
			{
				z2.closed = false;

				z2.x = zL.x;
				z2.y = (wTzstar - lambda1 * zL.x) / lambda2;
			}
		}

		if (zL.integerSolution == z2.integerSolution)
		{
			printf("RestrictiveLine| ERROR!!!\n");
			exit(0);
		}

		if (print) printf("RestrictiveLine| z2.closed = false\n");
	}

	newSolver->removeWeightedSumConstObj();
	//newSolver->removeConvexCombination(); 

	timeInnerLoop.stop();
	totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();

	return ok;
}


void SequentialAlgorithm::outerLoop()
{
	timeOuterLoop.reset();
	timeOuterLoop.start();

#ifdef USEPQUEUE
	while (!pqueue.empty())
#else
	while (!stack.empty())
#endif
	{

		timeSoFar.stop();
		totalTimeSoFar += timeSoFar.getCPUTotalSecs();

#ifdef APPROXSOLUTIONSTATISTICS 
		if (totalTimeSoFar >= approxTotalTime)
		{
			timeTotalTime.stop();
			totalTime += timeTotalTime.getCPUTotalSecs();

			timeOuterLoop.stop();
			totalTimeOuterLoop += timeOuterLoop.getCPUTotalSecs();

			approximateSolution(totalTimeSoFar);
			approxTotalTime *= 2;

			timeOuterLoop.reset();
			timeOuterLoop.start();

			timeTotalTime.reset();
			timeTotalTime.start();
		}
#endif
		if (print) printf("OuterLoop| Time so far: %lf\n\n", totalTimeSoFar);

#ifdef TIMELIMIT
		if (totalTimeSoFar >= MAXTOTALTIME)
		{
			while (!pqueue.empty())
			{
				Rectangle r = pqueue.top();
				pqueue.pop();

				r._p1.connected = false;
				r._p1.closed = true;

				r._p2.connected = false;
				r._p2.closed = true;

				ndpList.push_back(r._p1);
				ndpList.push_back(r._p2);
			}

			if (print) printf("\n\nMax total time reached!\n\n");

			timeOuterLoop.stop();
			totalTimeOuterLoop += timeOuterLoop.getCPUTotalSecs();

			return;
		}
#endif

		timeSoFar.reset();
		timeSoFar.start();

#ifdef USEPQUEUE
		printf("OuterLoop| Number of active rectangles: %d\n\n", (int)pqueue.size());
		Rectangle r = pqueue.top();
		pqueue.pop();
#else
		if (print) printf("OuterLoop| Number of active rectangles: %d\n\n", (int)stack.size());
		Rectangle r = stack.back();
		stack.pop_back();
#endif
		numberOfRectanglesSolved++;

		totalAreaRectanglesSolved = totalArea - remainingArea;
		remainingArea -= r._area;

		if (print) printf("OuterLoop| Exploring rectangle (%lf, %lf) (%lf, %lf)\n", r._p1.x, r._p1.y, r._p2.x, r._p2.y);
		if (print) printf("OuterLoop| Area: %.2lf (percentual: %lf)\n", r._area, 100 * (r._area / totalArea));
		if (print) printf("OuterLoop| Total Area Covered: %.2lf (percentual: %.2lf)\n\n", totalAreaRectanglesSolved, 100 * (totalAreaRectanglesSolved / totalArea));


#ifdef  RESTRICTIVELINEALGORITHMSWITCHTOETM
		if (100 * (totalAreaRectanglesSolved / totalArea) > RESTRICTIVELINEALGORITHMSWITCHTOETMPARAM)
		{
			pqueue.push(r);
			while (!pqueue.empty())
			{
				Rectangle r = pqueue.top();
				pqueue.pop();

				stack.push_back(r);
			}
			printf("\nLeaving outerloop\n");

			return;
		}
#endif 

		bool sameFrontier = false;
		NDP p1;
		p1.connected = false;
		p1.closed = true;

		if (!isSmallRectangle(r._p1.x, r._p1.y, r._p2.x, r._p2.y) && isSameFrontier(r._p1, r._p2))
		{
			numberOfRectanglesEqualIPs++;

#ifdef  IMPROVEMENTMIXED
			sameFrontier = true;

			if (buildLPFrontierForMixed(p1, r._p1, r._p2)) continue;
#endif 

#ifdef IMPROVEMENTNOGOOD
			noGoodNumberOfCalls++;

			sameFrontier = true;

			numberOfScalarizationWithNoGoodIP++;
			timeScalarizationWithNoGoodIP.reset();
			timeScalarizationWithNoGoodIP.start();

			newSolver->addNoGoodConstraint((r._p1).integerSolution);

			NDP ystar;

			bool foundNoGood = scalarize(r._p1, r._p2, ystar, r._p1, r._p2, true);

			//NDP z1 = r._p1;
			//NDP z2 = r._p2;
			//bool foundNoGood = restrictiveLineToTheRight(ystar, z1, z2, r._p1, r._p2);

			timeScalarizationWithNoGoodIP.stop();
			totalTimeScalarizationWithNoGoodIP += timeScalarizationWithNoGoodIP.getCPUTotalSecs();

			if (print) printf("OuterLoop| Scal Time = %lf\n", timeScalarizationWithNoGoodIP.getCPUTotalSecs());
			if (print) printf("OuterLoop| Scal Area = %lf\n", scalArea);
			scalMinUpdate(timeScalarizationWithNoGoodIP.getCPUTotalSecs(), scalArea);

			if (maxTimeScal < timeScalarizationWithNoGoodIP.getCPUTotalSecs()) maxTimeScal = timeScalarizationWithNoGoodIP.getCPUTotalSecs();

			newSolver->removeNoGoodConstraint();

			if (!foundNoGood)
			{
				noGoodSuccessRate++;
				buildLPFrontierForNoGood(r._p1, r._p2);
				continue;
			}
			else
			{
				NDP p;
				NDP wstar;
				if (!lineGenSameFrontierWithNoGood(p, ystar, r._p1, r._p2)) wstar = ystar;
				else wstar = p;

				p1 = wstar;
			}
#endif


#ifdef RESTRICTIVEFRONTIER 
			noGoodNumberOfCalls++;

			sameFrontier = true;

			numberOfScalarizationWithNoGoodIP++;
			timeScalarizationWithNoGoodIP.reset();
			timeScalarizationWithNoGoodIP.start();

			newSolver->addNoGoodConstraint((r._p1).integerSolution);

			NDP ystar;

			bool foundNoGood = scalarize(r._p1, r._p2, ystar, r._p1, r._p2, true);

			//NDP z1 = r._p1;
			//NDP z2 = r._p2;
			//bool foundNoGood = restrictiveLineToTheRight(ystar, z1, z2, r._p1, r._p2);

			timeScalarizationWithNoGoodIP.stop();
			totalTimeScalarizationWithNoGoodIP += timeScalarizationWithNoGoodIP.getCPUTotalSecs();

			if (print) printf("OuterLoop| Scal Time = %lf\n", timeScalarizationWithNoGoodIP.getCPUTotalSecs());
			if (print) printf("OuterLoop| Scal Area = %lf\n", scalArea);
			scalMinUpdate(timeScalarizationWithNoGoodIP.getCPUTotalSecs(), scalArea);

			if (maxTimeScal < timeScalarizationWithNoGoodIP.getCPUTotalSecs()) maxTimeScal = timeScalarizationWithNoGoodIP.getCPUTotalSecs();

			newSolver->removeNoGoodConstraint();

			if (!foundNoGood)
			{
				noGoodSuccessRate++;
				buildLPFrontierForNoGood(r._p1, r._p2);
				continue;
			}
			else
			{
				NDP p;
				NDP wstar;
				if (!lineGenSameFrontierWithNoGood(p, ystar, r._p1, r._p2)) wstar = ystar;
				else wstar = p;

				p1 = wstar;
			}
#endif

		}

		bool notOnTheLine = false;
		double split;

		if (!sameFrontier)
		{
			split = (r._p1.y + r._p2.y) / 2;
			if (print) printf("OuterLoop| Split = %lf\n", split);

			if (!isSmallRectangle(r._p1.x, split, r._p2.x, r._p2.y))
			{
				numberOfLexicographicIP++;
				timeLexicographicIP.reset();
				timeLexicographicIP.start();
				lexminObj1Obj2(p1, r._p1.x, split, r._p2.x, r._p2.y, true, r._p2);
				timeLexicographicIP.stop();
				totalTimeLexicographicIP += timeLexicographicIP.getCPUTotalSecs();

				if (print) printf("OuterLoop| Lex Time = %lf\n", timeLexicographicIP.getCPUTotalSecs());
				if (print) printf("OuterLoop| Lex Area = %lf\n", lexArea);
				lexMinUpdate(timeLexicographicIP.getCPUTotalSecs(), lexArea);

				if (maxTimeLex < timeLexicographicIP.getCPUTotalSecs()) maxTimeLex = timeLexicographicIP.getCPUTotalSecs();
			}
			else
			{
				notOnTheLine = true;
				p1 = r._p2;
				if (print) printf("OuterLoop| Small bottom rectangle discarded...\n");
			}
			if (!notOnTheLine && (fabs(p1.y - split) >= EPS)) notOnTheLine = true;

			if (print) printf("OuterLoop| p1: (p1.x = %lf, p1.y = %lf)\n", p1.x, p1.y);
		}

		if (notOnTheLine)
		{
			numberOfRectanglesZstarNotOnCutLine++;
			int numberOfRectanglesProducedByCurrentRectangle = 0;

			if (print) printf("OuterLoop| Point p1 is NOT on the line\n");

			NDP p2;
			p2.connected = false;
			p2.closed = true;
			if (!isSmallRectangle(r._p1.x, r._p1.y, std::max(r._p1.x, p1.x - EPS), split))
			{
				numberOfLexicographicIP++;
				timeLexicographicIP.reset();
				timeLexicographicIP.start();
				lexminObj2Obj1(p2, r._p1.x, r._p1.y, std::max(r._p1.x, p1.x - EPS), split, true, r._p1);
				timeLexicographicIP.stop();
				totalTimeLexicographicIP += timeLexicographicIP.getCPUTotalSecs();

				if (print) printf("OuterLoop| Lex Time = %lf\n", timeLexicographicIP.getCPUTotalSecs());
				if (print) printf("OuterLoop| Lex Area = %lf\n", lexArea);
				lexMinUpdate(timeLexicographicIP.getCPUTotalSecs(), lexArea);

				if (maxTimeLex < timeLexicographicIP.getCPUTotalSecs()) maxTimeLex = timeLexicographicIP.getCPUTotalSecs();
			}
			else
			{
				p2 = r._p1;
				if (print) printf("OuterLoop| Small upper rectangle discarded...\n");
			}

			if (print) printf("OuterLoop| p2: (p2.x = %lf, p2.y = %lf)\n", p2.x, p2.y);


			if (equalPoints(p1, r._p2))
			{
				if (print) printf("OuterLoop| p1 equal r.p2\n");

				r._p2.connected = false;
				ndpList.push_back(r._p2);
			}
			else
			{
				if (print) printf("OuterLoop| New Rectangle [p1, r.p2]\n");

				if (!isSmallRectangle(p1.x, p1.y, r._p2.x, r._p2.y))
				{
					if (print) printf("OuterLoop| r = (%lf %lf) (%lf %lf)\n", p1.x, p1.y, r._p2.x, r._p2.y);

					if ((p1.x > r._p2.x) || (p1.y < r._p2.y)) exit(0);

					numberOfRectanglesProducedByCurrentRectangle++;
					Rectangle r1(p1, r._p2);

#ifdef USEPQUEUE
					pqueue.push(r1);
#else
					stack.push_back(r1);
#endif
					remainingArea += r1._area;
				}
				else
				{
					if (print) printf("OuterLoop| Small therefore discarded...\n");

					p1.connected = false;
					ndpList.push_back(p1);
				}
			}

			if (equalPoints(p2, r._p1))
			{
				if (print) printf("OuterLoop| p2 equal r.p1\n");

				r._p1.connected = false;
				ndpList.push_back(r._p1);
			}
			else
			{
				if (print) printf("OuterLoop| New Rectangle [r.p1, p2]\n");

				if (!isSmallRectangle(r._p1.x, r._p1.y, p2.x, p2.y))
				{
					if (print) printf("OuterLoop| r = (%lf %lf) (%lf %lf)\n", r._p1.x, r._p1.y, p2.x, p2.y);

					if ((r._p1.x > p2.x) || (r._p1.y < p2.y)) exit(0);

					numberOfRectanglesProducedByCurrentRectangle++;
					Rectangle r2(r._p1, p2);

#ifdef USEPQUEUE
					pqueue.push(r2);

#else
					stack.push_back(r2);
#endif
					remainingArea += r2._area;
				}
				else
				{
					if (print) printf("OuterLoop| Small therefore discarded...\n");

					p2.connected = false;
					ndpList.push_back(p2);
				}
			}

			if (numberOfRectanglesProducedByCurrentRectangle == 0) numberOfRectanglesProducedEqualZero++;
			if (numberOfRectanglesProducedByCurrentRectangle == 1) numberOfRectanglesProducedEqualOne++;
			if (numberOfRectanglesProducedByCurrentRectangle == 2) numberOfRectanglesProducedEqualTwo++;

		}
		else
		{
			numberOfRectanglesZstarOnCutLine++;

			if (print) printf("OuterLoop| Point p1 is on the line\n");

			NDP z1, z2;
			if (print) printf("\n\nOuterLoop| Calling Inner Loop...\n\n");

			std::vector<NDP> ndpMLeft;
			std::vector<NDP> ndpMRight;

			r._p1.closed = r._p2.closed = true;
			r._p1.connected = r._p2.connected = false;

			r._p1.id = r._p2.id = -1;

			ndpMLeft.push_back(r._p1);
			ndpMRight.push_back(r._p2);

			timeOuterLoop.stop();
			totalTimeOuterLoop += timeOuterLoop.getCPUTotalSecs();
			if (print) printf("\n*******************************************************************************************\n");

#ifdef RECURSIVEALGO
			numberCallsOuterLooptoInnerLoop++;
			std::vector<std::pair<NDP, NDP> > List;
			innerLoopRecursive(z1, z2, p1, r._p1, r._p2, ndpMLeft, ndpMRight, List, 0, true);
#else

#ifdef RESTRICTIVELINE
			restrictiveLine(z1, z2, p1, r._p1, r._p2, ndpMLeft, ndpMRight);
#else
			innerLoop(z1, z2, p1, r._p1, r._p2, ndpMLeft, ndpMRight);
#endif

#endif

			if (print) printf("\n*******************************************************************************************\n");
			timeOuterLoop.reset();
			timeOuterLoop.start();

			if (print) if (print) printf("\n\nOuterLoop| Inner Loop has finished!\n\n");

			if (print) printf("OuterLoop| z1: (%lf, %lf)\n", z1.x, z1.y);
			if (print) printf("OuterLoop| z2: (%lf, %lf)\n", z2.x, z2.y);

			if (equalPoints(z1, z2))
			{
				if (print) printf("OuterLoop| z1 equal z2\n");
			}
			else
			{
				if ((z1.x > z2.x) || (z1.y < z2.y))
				{
					printf("\nOuterLoop| Error Inner Loop!\n");
					exit(0);
				}

				if (print) printf("OuterLoop| z1 NOT equal z2\n");
			}

			if (!equalPoints(z1, z2))
			{
				z1.connected = true;
				z1.id = -2;
				ndpList.push_back(z1);
				z2.connected = false;
				z2.id = -2;
				ndpList.push_back(z2);
			}

			ndpMLeft.push_back(z1);
			ndpMRight.push_back(z2);

			std::sort(ndpMLeft.begin(), ndpMLeft.end(), sortfunction);

			if (print) printf("\n\n\n1-########################################################-1\n\n\n");
			for (int i = 0; i < (int)ndpMLeft.size(); i++)
			{
				if (print) printf("(%lf %lf %d %d)\n", ndpMLeft[i].x, ndpMLeft[i].y, ndpMLeft[i].connected ? 1 : 0, ndpMLeft[i].id);
				//printf("(%lf, %lf)\n", ndpMLeft[i].x, ndpMLeft[i].y);

			}
			if (print) printf("\n\n\n1-########################################################-1\n\n\n");


			for (int i = 0; i < (int)ndpMLeft.size() - 1; i++)
			{
				if (equalPoints(ndpMLeft[i], ndpMLeft[i + 1]))
				{
					if (print) printf("OuterLoop|-1 r.p1-%d equal zR-%d\n", i, i + 1);

#ifdef RECURSIVEALGO
					if (ndpMLeft[i].connected || ndpMLeft[i + 1].connected) ndpMLeft[i + 1].connected = true;
#endif
				}
				else
				{
					if (print) printf("OuterLoop|-1 New Rectangle [r.p1-%d, zR-%d]\n", i, i + 1);

					if (!isSmallRectangle(ndpMLeft[i].x, ndpMLeft[i].y, ndpMLeft[i + 1].x, ndpMLeft[i + 1].y))
					{

#ifdef RECURSIVEALGO
						if (ndpMLeft[i].connected)
						{
							ndpList.push_back(ndpMLeft[i]);
							ndpList.push_back(ndpMLeft[i + 1]);

							if (print) printf("OuterLoop|-1 Corner points define a line segment\n");

							continue;
						}
#endif

						if (print) printf("OuterLoop|-1 r = (%lf %lf) (%lf %lf)\n", ndpMLeft[i].x, ndpMLeft[i].y, ndpMLeft[i + 1].x, ndpMLeft[i + 1].y);

						if ((ndpMLeft[i].x > ndpMLeft[i + 1].x) || (ndpMLeft[i].y < ndpMLeft[i + 1].y)) exit(0);

						Rectangle r1(ndpMLeft[i], ndpMLeft[i + 1]);

#ifdef USEPQUEUE
						pqueue.push(r1);
#else
						stack.push_back(r1);
#endif
						remainingArea += r1._area;
					}
					else
					{
						//ndpMLeft[i].closed = true;
						ndpMLeft[i].connected = false;
						ndpList.push_back(ndpMLeft[i]);
						//ndpMLeft[i+1].closed = true;
						//ndpMLeft[i+1].connected = false;
						//ndpList.push_back(ndpMLeft[i+1]);

						if (print) printf("OuterLoop|-1 Small rectangle therefore discarded...\n");
					}
				}
			}

			std::sort(ndpMRight.begin(), ndpMRight.end(), sortfunction);

			if (print) printf("\n\n\n2-########################################################-2\n\n\n");
			for (int i = 0; i < (int)ndpMRight.size(); i++)
			{
				if (print) printf("(%lf %lf %d %d)\n", ndpMRight[i].x, ndpMRight[i].y, ndpMRight[i].connected ? 1 : 0, ndpMRight[i].id);
				//printf("(%lf, %lf)\n", ndpMRight[i].x, ndpMRight[i].y);
			}
			if (print) printf("\n\n\n2-########################################################-2\n\n\n");

			for (int i = 0; i < (int)ndpMRight.size() - 1; i++)
			{
				if (equalPoints(ndpMRight[i], ndpMRight[i + 1]))
				{
					if (print) printf("OuterLoop|-2 r.p2-%d equal zL-%d\n", i, i + 1);

#ifdef RECURSIVEALGO
					if (ndpMRight[i].connected || ndpMRight[i + 1].connected) ndpMRight[i + 1].connected = true;
#endif
				}
				else
				{
					if (print) printf("OuterLoop|-2 New Rectangle [zL-%d, r.p2-%d]\n", i, i + 1);

					if (!isSmallRectangle(ndpMRight[i].x, ndpMRight[i].y, ndpMRight[i + 1].x, ndpMRight[i + 1].y))
					{

#ifdef RECURSIVEALGO
						if (ndpMRight[i].connected)
						{
							ndpList.push_back(ndpMRight[i]);
							ndpList.push_back(ndpMRight[i + 1]);

							if (print) printf("OuterLoop|-2 Corner points define a line segment\n");

							continue;
						}

#endif

						if (print) printf("OuterLoop|-2 r = (%lf %lf) (%lf %lf)\n", ndpMRight[i].x, ndpMRight[i].y, ndpMRight[i + 1].x, ndpMRight[i + 1].y);

						if ((ndpMRight[i].x > ndpMRight[i + 1].x) || (ndpMRight[i].y < ndpMRight[i + 1].y)) exit(0);

						Rectangle r2(ndpMRight[i], ndpMRight[i + 1]);

#ifdef USEPQUEUE
						pqueue.push(r2);
#else
						stack.push_back(r2);
#endif
						remainingArea += r2._area;
					}
					else
					{
						//ndpMRight[i].closed = true;
						ndpMRight[i].connected = false;
						ndpList.push_back(ndpMRight[i]);
						//ndpMRight[i+1].closed = true;
						//ndpMRight[i+1].connected = false;

						//ndpList.push_back(ndpMRight[i+1]);

						if (print) printf("OuterLoop|-2 Small rectangle therefore discarded...\n");
					}
				}
			}
		}

	}

	timeOuterLoop.stop();
	totalTimeOuterLoop += timeOuterLoop.getCPUTotalSecs();

}

void SequentialAlgorithm::innerLoop(NDP& z1, NDP& z2, NDP& zstar, NDP& L, NDP& R, std::vector<NDP>& ndpMLeft, std::vector<NDP>& ndpMRight)
{
	numberCallsInnerLoop++;

	if (print) printf("\n\nInnerLoop| Calling Line Gen...\n\n");

	if (print) printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	lineGen(z1, z2, zstar, L, R);
	if (print) printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	timeInnerLoop.reset();
	timeInnerLoop.start();

	if (print) printf("\n\nInnerLoop| Line Gen has finished!\n\n");

	if (print) printf("InnerLoop| z1: (%lf, %lf)\n", z1.x, z1.y);
	if (print) printf("InnerLoop| zstar: (%lf, %lf)\n", zstar.x, zstar.y);
	if (print) printf("InnerLoop| z2: (%lf, %lf)\n", z2.x, z2.y);

	if (!equalPoints(z1, z2))
	{
		if ((z1.x > z2.x) || (z1.y < z2.y))
		{
			printf("\nInnerLoop| Error Inner Loop-1!\n");
			exit(0);
		}
	}
	else
	{
		if (print) printf("InnerLoop| z1 equal z2\n");

		zstar.closed = true;
		zstar.connected = false;
		z1 = zstar;
		z2 = zstar;

		ndpMLeft.push_back(z1);
		ndpMRight.push_back(z2);

		timeInnerLoop.stop();
		totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();

		return;
	}

	z1.closed = z2.closed = true;

	double lambda1 = fabs(z1.y - z2.y);
	double lambda2 = fabs(z2.x - z1.x);

	if (print) printf("InnerLoop| [lambda1 = %lf; lambda2 = %lf]\n", lambda1, lambda2);

	NDP ystar;
	double result;
	double obj1value;
	double wTzstar;

	wTzstar = lambda1 * zstar.x + lambda2 * zstar.y;
	newSolver->createWeightedSumObjective(lambda1, lambda2);

	bool check1 = false;
	if (!equalPoints(z1, L) && (fabs(L.y - z1.y) <= EPS))
	{
		check1 = true;
		if (print) printf("InnerLoop| Initial z1 is on the box line\n");

		ystar = L;
		obj1value = L.x;

		if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);
	}

	bool check2 = false;
	if (!equalPoints(z2, R) && (fabs(R.x - z2.x) <= EPS))
	{
		check2 = true;
		if (print) printf("InnerLoop| Initial z2 is on the box line\n");

		if (!check1)
		{
			ystar = R;
			obj1value = R.x;

			if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);
		}
	}

	if (check1 && check2) if (print) printf("InnerLoop| Both z1 and z2 on the box line\n");

	if (!check1 && !check2)
	{
		numberOfScalarizationIP++;
		timeScalarizationIP.reset();
		timeScalarizationIP.start();

		//newSolver->createWeightedSumObjective(lambda1, lambda2);
		newSolver->addWeightedSumObjective();

#ifdef ALLCONSTRAINTS
		newSolver->addConstObj(L.x, z1.y, z2.x, R.y);
#else
		newSolver->addConstObj1(z2.x);
		newSolver->addConstObj2(z1.y);
#endif

		newSolver->addMIPStart(zstar.continuousSolution, zstar.integerSolution);

		if (!newSolver->run())
		{
			printf("ERROR8\n");
			exit(0);
		}
		//newSolver->printSolution();

		result = newSolver->getObjValue();
		if (print) printf("InnerLoop| MIP result = %lf\n", result);

		//newSolver->printSolution();

		obj1value = newSolver->getObj1Value();

		//NDP ystar;
		ystar.closed = true;
		ystar.connected = false;
		ystar.id = -4;
		ystar.x = obj1value;
		ystar.y = newSolver->getObj2Value();
		if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);

		ystar.continuousSolution = newSolver->getContinuousVariablesSolution();
		ystar.integerSolution = newSolver->getIntegerVariablesSolution();

		newSolver->removeWeightedSumObjective();

#ifdef ALLCONSTRAINTS
		newSolver->removeConstObj();
#else
		newSolver->removeConstObj1();
		newSolver->removeConstObj2();
#endif

		timeScalarizationIP.stop();
		totalTimeScalarizationIP += timeScalarizationIP.getCPUTotalSecs();

		if (print) printf("InnerLoop| Scal Time = %lf\n", timeScalarizationIP.getCPUTotalSecs());
		if (print) printf("InnerLoop| Scal Area = %lf\n", fabs(z2.x - L.x) * fabs(z1.y - R.y));
		scalMinUpdate(timeScalarizationIP.getCPUTotalSecs(), fabs(z2.x - L.x) * fabs(z1.y - R.y));

		if (maxTimeScal < timeScalarizationIP.getCPUTotalSecs()) maxTimeScal = timeScalarizationIP.getCPUTotalSecs();

		//wTzstar = lambda1 * zstar.x + lambda2 * zstar.y;
		if (print) printf("InnerLoop| wTzstar = %12lf\n", wTzstar);

		//if ((ystar.integerSolution != zstar.integerSolution) && (wTzstar <= result + EPS)) 
		//if (wTzstar <= result + EPS)
		//if (ystar.integerSolution == zstar.integerSolution)
		if ((ystar.integerSolution == zstar.integerSolution) || (wTzstar <= result + EPS))
		{
			if (print) printf("InnerLoop|-1 wTzstar <= result\n\n");

			numberProvedOptByOneScalInnerLoop++;

			z1.connected = true;
			z2.connected = false;

			z1.id = -3;
			z2.id = -3;

			ndpMLeft.push_back(z1);
			ndpMRight.push_back(z2);

			timeInnerLoop.stop();
			totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();

			return;
		}
	}

	bool opt = false;
	NDP v1, v2;
	bool ok1 = false, ok2 = false;
	while (!opt)
	{

		/*
				if ((minPointsCoord(zstar, ystar) < EPS) && (maxPointsCoord(zstar, ystar) > EPS))
				{
					printf("ERROR-check\n");
					exit(0);
				}
		*/

		//if (obj1value <= zstar.x - EPS)
		if (obj1value <= zstar.x)
		{
			if (check1) check1 = false;

			ok1 = true;
#ifdef IMPROVEMENTMULTIBOX			
			ndpMLeft.push_back(ystar);
#endif
			if (print) printf("\n\nInnerLoop| Part-1 (Left side of z*)\n");

			innerLoopNumberOfLP++;
			numberOfLP++;
			timeLP.reset();
			timeLP.start();

			newSolver->addObjective2();
			newSolver->addWeightedSumConstObj(wTzstar);

			newSolver->addFixingIntegerConsts2(ystar.integerSolution);
#ifdef ALLCONSTRAINTS
			newSolver->addConstObj(L.x - EPS, z1.y + EPS, zstar.x, zstar.y);
#endif
			newSolver->addMIPStart(ystar.continuousSolution, ystar.integerSolution);

			if (!newSolver->run())
			{
				printf("ERROR9\n");
				exit(0);
			}

			v1.y = newSolver->getObjValue();
			v1.x = newSolver->getObj1Value();

			v1.continuousSolution = newSolver->getContinuousVariablesSolution();
			v1.integerSolution = newSolver->getIntegerVariablesSolution();

			newSolver->removeObjective2();
			newSolver->removeWeightedSumConstObj();
			newSolver->removeFixingIntegerConsts();
#ifdef ALLCONSTRAINTS
			newSolver->removeConstObj();
#endif

			timeLP.stop();
			totalTimeLP += timeLP.getCPUTotalSecs();
			innerLoopTotalTimeLP += timeLP.getCPUTotalSecs();

			if (print) printf("InnerLoop| [v1.x = %lf; v1.y = %lf]\n", v1.x, v1.y);

			bool onZ1Z2line = false;
			if (fabs((lambda1 * v1.x + lambda2 * v1.y) - wTzstar) <= EPS)
			{
				if (print) printf("InnerLoop| v1 is on z1-z2 line\n");

				innerLoopNumberOfLP++;
				numberOfLP++;
				timeLP.reset();
				timeLP.start();

				newSolver->addObjective1();
				newSolver->addConstObj2(v1.y);
				newSolver->addFixingIntegerConsts2(ystar.integerSolution);
				newSolver->addMIPStart(v1.continuousSolution, v1.integerSolution);

				if (!newSolver->run())
				{
					printf("ERROR9-1\n");
					exit(0);
				}

				NDP v;
				v.x = newSolver->getObjValue();
				v.y = newSolver->getObj2Value();

				v.continuousSolution = newSolver->getContinuousVariablesSolution();
				v.integerSolution = newSolver->getIntegerVariablesSolution();

				if (equalPoints(v, v1))
				{
					if (print) printf("InnerLoop| v1 has not changed\n");
				}
				else
				{
					v1 = v;
					if (print) printf("InnerLoop| New v1: [v1.x = %lf; v1.y = %lf]\n", v1.x, v1.y);
				}

				newSolver->removeObjective1();
				newSolver->removeConstObj2();
				newSolver->removeFixingIntegerConsts();

				timeLP.stop();
				totalTimeLP += timeLP.getCPUTotalSecs();
				innerLoopTotalTimeLP += timeLP.getCPUTotalSecs();

				if (fabs((lambda1 * v1.x + lambda2 * v1.y) - wTzstar) <= EPS) onZ1Z2line = true;
			}

			if (onZ1Z2line)
			{
				z1.x = v1.x;
				z1.y = v1.y;
				z1.closed = true;
				if (print) printf("InnerLoop| z1.closed = true\n");
			}
			else
			{
				z1.x = (wTzstar - lambda2 * v1.y) / lambda1;
				z1.y = v1.y;

				z1.closed = false;
				if (print) printf("InnerLoop| z1.closed = false\n");
			}

			if (print) printf("InnerLoop| z1-NDP: (%lf, %lf)\n", z1.x, z1.y);
			if (!equalPoints(z1, zstar))
			{
				if ((z1.x > zstar.x) || (z1.y < zstar.y))
				{
					printf("InnerLoop| z1: (%lf, %lf)\n", z1.x, z1.y);
					printf("InnerLoop| zstar: (%lf, %lf)\n", zstar.x, zstar.y);
					printf("InnerLoop| z2: (%lf, %lf)\n", z2.x, z2.y);

					printf("\nInnerLoop| Error Inner Loop-2!\n");
					exit(0);
				}
			}

			if (print) printf("InnerLoop| End-Part1\n\n");
		}


		//if (obj1value >= zstar.x + EPS)
		else
		{
			if (check2) check2 = false;

			ok2 = true;
#ifdef IMPROVEMENTMULTIBOX
			ndpMRight.push_back(ystar);
#endif

			if (print) printf("\n\nInnerLoop| Part-2 (Right side of z*)\n");

			innerLoopNumberOfLP++;
			numberOfLP++;
			timeLP.reset();
			timeLP.start();

			newSolver->addObjective1();
			newSolver->addWeightedSumConstObj(wTzstar);

			newSolver->addFixingIntegerConsts2(ystar.integerSolution);
#ifdef ALLCONSTRAINTS
			newSolver->addConstObj(zstar.x, zstar.y, z2.x + EPS, R.y - EPS);
#endif
			newSolver->addMIPStart(ystar.continuousSolution, ystar.integerSolution);

			if (!newSolver->run())
			{
				printf("ERROR10\n");
				exit(0);
			}

			v2.x = newSolver->getObjValue();
			v2.y = newSolver->getObj2Value();

			v2.continuousSolution = newSolver->getContinuousVariablesSolution();
			v2.integerSolution = newSolver->getIntegerVariablesSolution();

			newSolver->removeObjective1();
			newSolver->removeWeightedSumConstObj();
			newSolver->removeFixingIntegerConsts();
#ifdef ALLCONSTRAINTS
			newSolver->removeConstObj();
#endif

			timeLP.stop();
			totalTimeLP += timeLP.getCPUTotalSecs();
			innerLoopTotalTimeLP += timeLP.getCPUTotalSecs();

			if (print) printf("InnerLoop| [v2.x = %lf; v2.y = %lf]\n", v2.x, v2.y);

			bool onZ1Z2line = false;
			if (fabs((lambda1 * v2.x + lambda2 * v2.y) - wTzstar) <= EPS)
			{
				if (print) printf("InnerLoop| v2 is on z1-z2 line\n");

				innerLoopNumberOfLP++;
				numberOfLP++;
				timeLP.reset();
				timeLP.start();

				newSolver->addObjective2();
				newSolver->addConstObj1(v2.x);
				newSolver->addFixingIntegerConsts2(ystar.integerSolution);
				newSolver->addMIPStart(v2.continuousSolution, v2.integerSolution);

				if (!newSolver->run())
				{
					printf("ERROR10-1\n");
					exit(0);
				}

				NDP v;
				v.y = newSolver->getObjValue();
				v.x = newSolver->getObj1Value();

				v.continuousSolution = newSolver->getContinuousVariablesSolution();
				v.integerSolution = newSolver->getIntegerVariablesSolution();

				if (equalPoints(v, v2))
				{
					if (print) printf("InnerLoop| v2 has not changed\n");
				}
				else
				{
					v2 = v;
					if (print) printf("InnerLoop| New v2: [v2.x = %lf; v2.y = %lf]\n", v2.x, v2.y);
				}

				newSolver->removeObjective2();
				newSolver->removeConstObj1();
				newSolver->removeFixingIntegerConsts();

				timeLP.stop();
				totalTimeLP += timeLP.getCPUTotalSecs();
				innerLoopTotalTimeLP += timeLP.getCPUTotalSecs();

				if (fabs((lambda1 * v2.x + lambda2 * v2.y) - wTzstar) <= EPS) onZ1Z2line = true;
			}

			if (onZ1Z2line)
			{
				z2.x = v2.x;
				z2.y = v2.y;

				z2.closed = true;
				if (print) printf("InnerLoop| z2.closed = true\n");
			}
			else
			{
				z2.x = v2.x;
				z2.y = (wTzstar - lambda1 * v2.x) / lambda2;

				z2.closed = false;
				if (print) printf("InnerLoop| z2.closed = false\n");
			}

			if (print) printf("InnerLoop| z2-NDP: (%lf, %lf)\n", z2.x, z2.y);

			if (!equalPoints(z2, zstar))
			{
				if ((z2.x < zstar.x) || (z2.y > zstar.y))
				{
					printf("InnerLoop| z1: (%lf, %lf)\n", z1.x, z1.y);
					printf("InnerLoop| zstar: (%lf, %lf)\n", zstar.x, zstar.y);
					printf("InnerLoop| z2: (%lf, %lf)\n", z2.x, z2.y);

					printf("\nInnerLoop| Error Inner Loop-3!\n");
					exit(0);
				}
			}

			if (print) printf("InnerLoop| End-Part2\n\n");
		}

		if (equalPoints(z1, z2))
		{
			z1.closed = z2.closed = true;
			break;
		}

		if (check2)
		{
			ystar = R;
			obj1value = R.x;

			if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);
		}
		else
		{
			numberOfScalarizationIP++;
			timeScalarizationIP.reset();
			timeScalarizationIP.start();

			newSolver->addWeightedSumObjective();

			double slack1 = 0.0;
			if (ok1) slack1 = EPS;

			double slack2 = 0.0;
			if (ok2) slack2 = EPS;

#ifdef ALLCONSTRAINTS
			newSolver->addConstObj(L.x, z1.y - slack1, z2.x - slack2, R.y);
#else
			newSolver->addConstObj2(z1.y - slack1);
			newSolver->addConstObj1(z2.x - slack2);
#endif

			newSolver->addMIPStart(zstar.continuousSolution, zstar.integerSolution);

			if (!newSolver->run())
			{
				printf("ERROR11\n");
				exit(0);
			}
			//newSolver->printSolution();

			result = newSolver->getObjValue();
			if (print) printf("InnerLoop| MIP result = %lf\n", result);

			obj1value = newSolver->getObj1Value();

			ystar.closed = true;
			ystar.connected = false;
			ystar.id = -4;
			ystar.x = obj1value;
			ystar.y = newSolver->getObj2Value();
			if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);

			//newSolver->saveIntegerSolution(); 
			ystar.continuousSolution = newSolver->getContinuousVariablesSolution();
			ystar.integerSolution = newSolver->getIntegerVariablesSolution();

			newSolver->removeWeightedSumObjective();

#ifdef ALLCONSTRAINTS
			newSolver->removeConstObj();
#else
			newSolver->removeConstObj1();
			newSolver->removeConstObj2();
#endif

			timeScalarizationIP.stop();
			totalTimeScalarizationIP += timeScalarizationIP.getCPUTotalSecs();

			if (print) printf("InnerLoop| Scal Time = %lf\n", timeScalarizationIP.getCPUTotalSecs());
			if (print) printf("InnerLoop| Scal Area = %lf\n", fabs((z2.x - slack2) - L.x) * fabs((z1.y - slack1) - R.y));
			scalMinUpdate(timeScalarizationIP.getCPUTotalSecs(), fabs((z2.x - slack2) - L.x) * fabs((z1.y - slack1) - R.y));

			if (maxTimeScal < timeScalarizationIP.getCPUTotalSecs()) maxTimeScal = timeScalarizationIP.getCPUTotalSecs();

			//if (wTzstar <= result + EPS)
			//if (ystar.integerSolution == zstar.integerSolution)
			if ((ystar.integerSolution == zstar.integerSolution) || (wTzstar <= result + EPS))
			{
				if (print) printf("InnerLoop|-2 wTzstar <= result\n\n");
				opt = true;
			}
		}
	}

	if (!z1.closed)
	{
		if (print) printf("InnerLoop| Searching for zR in r = (%lf %lf) (%lf %lf)\n", L.x, L.y, z1.x, z1.y);

		NDP zR;
		zR.closed = true;
		zR.connected = false;
		zR.id = -5;

		numberOfMinIP++;
		timeMinIP.reset();
		timeMinIP.start();

		newSolver->addObjective1();

#ifdef ALLCONSTRAINTS
		newSolver->addConstObj(L.x, z1.y, z1.x, z1.y - EPS);
#else
		newSolver->addConstObj2(z1.y);
#endif
		//newSolver->addMIPStart(v1.continuousSolution, v1.integerSolution);

		if (!newSolver->run())
		{
			printf("ERROR6\n");
			exit(0);
		}

		zR.x = newSolver->getObjValue();
		zR.y = newSolver->getObj2Value();

		zR.continuousSolution = newSolver->getContinuousVariablesSolution();
		zR.integerSolution = newSolver->getIntegerVariablesSolution();

		newSolver->removeObjective1();

#ifdef ALLCONSTRAINTS
		newSolver->removeConstObj();
#else
		newSolver->removeConstObj2();
#endif
		timeMinIP.stop();
		totalTimeMinIP += timeMinIP.getCPUTotalSecs();

		if (print) printf("InnerLoop| zR found: (%lf %lf)\n", zR.x, zR.y);

		ndpMLeft.push_back(zR);
	}


	if (!z2.closed)
	{
		if (print) printf("InnerLoop| Searching for zL in r = (%lf %lf) (%lf %lf)\n", z2.x, std::max(R.y, z2.y - EPS), R.x, R.y);

		NDP zL;
		zL.closed = true;
		zL.connected = false;
		zL.id = -5;

		numberOfMinIP++;
		timeMinIP.reset();
		timeMinIP.start();

		newSolver->addObjective2();

#ifdef ALLCONSTRAINTS
		newSolver->addConstObj(z2.x - EPS, z2.y, z2.x, R.y);
#else
		newSolver->addConstObj1(z2.x);
#endif

		//newSolver->addMIPStart(v2.continuousSolution, v2.integerSolution);

		if (!newSolver->run())
		{
			printf("ERROR7\n");
			exit(0);
		}

		zL.x = newSolver->getObj1Value();
		zL.y = newSolver->getObjValue();

		zL.continuousSolution = newSolver->getContinuousVariablesSolution();
		zL.integerSolution = newSolver->getIntegerVariablesSolution();

		newSolver->removeObjective2();

#ifdef ALLCONSTRAINTS
		newSolver->removeConstObj();
#else
		newSolver->removeConstObj1();
#endif

		timeMinIP.stop();
		totalTimeMinIP += timeMinIP.getCPUTotalSecs();

		if (print) printf("InnerLoop| zL found: (%lf %lf)\n", zL.x, zL.y);

		ndpMRight.push_back(zL);
	}

	timeInnerLoop.stop();
	totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();
}

void SequentialAlgorithm::innerLoopRecursive(NDP& z1, NDP& z2, NDP& zstar, NDP& L, NDP& R, std::vector<NDP>& ndpMLeft, std::vector<NDP>& ndpMRight, std::vector<std::pair<NDP, NDP> >& List, int depth, bool lineGenToTheRight)
{
	numberCallsInnerLoop++;

	if (print) printf("\n\nInnerLoop| depth = %d\n", depth);
	if (depth > maximumdepth) maximumdepth = depth;

	if (print) printf("\n\nInnerLoop| Calling Line Gen...\n\n");

	if (print) printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	lineGen(z1, z2, zstar, L, R, lineGenToTheRight);
	if (print) printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	timeInnerLoop.reset();
	timeInnerLoop.start();

	if (print) printf("\n\nInnerLoop| Line Gen has finished!\n\n");

	if (print) printf("InnerLoop| z1: (%lf, %lf)\n", z1.x, z1.y);
	if (print) printf("InnerLoop| zstar: (%lf, %lf)\n", zstar.x, zstar.y);
	if (print) printf("InnerLoop| z2: (%lf, %lf)\n", z2.x, z2.y);

	if (!equalPoints(z1, z2))
	{
		if ((z1.x > z2.x) || (z1.y < z2.y))
		{
			printf("\nInnerLoop| Error Inner Loop-1!\n");
			exit(0);
		}
	}
	else
	{
		if (print) printf("InnerLoop| z1 equal z2\n");

		zstar.closed = true;
		zstar.connected = false;
		z1 = zstar;
		z2 = zstar;

		ndpMLeft.push_back(z1);
		ndpMRight.push_back(z2);

		timeInnerLoop.stop();
		totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();

		return;
	}

	z1.closed = z2.closed = true;

	double lambda1 = fabs(z1.y - z2.y);
	double lambda2 = fabs(z2.x - z1.x);
	double ratio = lambda1 / lambda2;

	if (print) printf("InnerLoop| [lambda1 = %lf; lambda2 = %lf]; ratio = %lf\n", lambda1, lambda2, ratio);

	if (print) printf("\nInnerLoop|-1 lineTrim: z1: (%lf, %lf), z2: (%lf, %lf)\n", z1.x, z1.y, z2.x, z2.y);
	lineTrim(z1, z2, zstar, List);
	if (print) printf("InnerLoop|-2 lineTrim: z1: (%lf, %lf), z2: (%lf, %lf)\n\n", z1.x, z1.y, z2.x, z2.y);

	List.push_back(std::make_pair(z1, z2));

	//double lambda1 = fabs(z1.y - z2.y);
	//double lambda2 = fabs(z2.x - z1.x); 

	lambda1 = fabs(z1.y - z2.y);
	lambda2 = fabs(z2.x - z1.x);
	double newratio = lambda1 / lambda2;

	/*
		if (fabs(ratio - newratio) > EPS/10)
		{
			printf("\nInnerLoop| Error ratio!\n");
			exit(0);
		}
	 */
	if (print) printf("InnerLoop| [lambda1 = %lf; lambda2 = %lf]; ratio = %lf\n", lambda1, lambda2, lambda1 / lambda2);

	NDP ystar;
	double result;
	double obj1value;
	double wTzstar;

	wTzstar = lambda1 * zstar.x + lambda2 * zstar.y;
	newSolver->createWeightedSumObjective(lambda1, lambda2);

	bool check1 = false;
	if (!equalPoints(z1, L) && (fabs(L.y - z1.y) <= EPS))
	{
		check1 = true;
		if (print) printf("InnerLoop| Initial z1 is on the box line\n");

		ystar = L;
		obj1value = L.x;

		if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);
	}

	bool check2 = false;
	if (!equalPoints(z2, R) && (fabs(R.x - z2.x) <= EPS))
	{
		check2 = true;
		if (print) printf("InnerLoop| Initial z2 is on the box line\n");

		if (!check1)
		{
			ystar = R;
			obj1value = R.x;

			if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);
		}
	}

	if (check1 && check2) if (print) printf("InnerLoop| Both z1 and z2 on the box line\n");

	if (!check1 && !check2)
	{
		numberOfScalarizationIP++;
		timeScalarizationIP.reset();
		timeScalarizationIP.start();

		newSolver->addWeightedSumObjective();

#ifdef ALLCONSTRAINTS
		newSolver->addConstObj(L.x, z1.y, z2.x, R.y);
#else
		newSolver->addConstObj1(z2.x);
		newSolver->addConstObj2(z1.y);
#endif
		newSolver->addMIPStart(zstar.continuousSolution, zstar.integerSolution);

		if (!newSolver->run())
		{
			printf("ERROR8\n");
			exit(0);
		}
		//newSolver->printSolution();

		result = newSolver->getObjValue();
		if (print) printf("InnerLoop| MIP result = %lf\n", result);

		obj1value = newSolver->getObj1Value();

		ystar.closed = true;
		ystar.x = obj1value;
		ystar.y = newSolver->getObj2Value();
		if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);

		ystar.continuousSolution = newSolver->getContinuousVariablesSolution();
		ystar.integerSolution = newSolver->getIntegerVariablesSolution();

		newSolver->removeWeightedSumObjective();

#ifdef ALLCONSTRAINTS
		newSolver->removeConstObj();
#else
		newSolver->removeConstObj1();
		newSolver->removeConstObj2();
#endif

		timeScalarizationIP.stop();
		totalTimeScalarizationIP += timeScalarizationIP.getCPUTotalSecs();

		if (print) printf("InnerLoop| Scal Time = %lf\n", timeScalarizationIP.getCPUTotalSecs());
		if (print) printf("InnerLoop| Scal Area = %lf\n", fabs(z2.x - L.x) * fabs(z1.y - R.y));
		scalMinUpdate(timeScalarizationIP.getCPUTotalSecs(), fabs(z2.x - L.x) * fabs(z1.y - R.y));

		if (maxTimeScal < timeScalarizationIP.getCPUTotalSecs()) maxTimeScal = timeScalarizationIP.getCPUTotalSecs();

		if (print) printf("InnerLoop| wTzstar = %12lf\n", wTzstar);

		//if ((ystar.integerSolution != zstar.integerSolution) && (wTzstar <= result + EPS)) 
		//if (wTzstar <= result + EPS)
		//if (ystar.integerSolution == zstar.integerSolution)
		if ((ystar.integerSolution == zstar.integerSolution) || (wTzstar <= result + EPS))
		{
			if (print) printf("InnerLoop|-1 wTzstar = result\n");

			z1.connected = true;
			z2.connected = false;

			z1.id = -3;
			z2.id = -3;

			ndpMLeft.push_back(z1);
			ndpMRight.push_back(z2);

			List.pop_back();

			timeInnerLoop.stop();
			totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();

			return;
		}
	}

	bool opt = false;
	NDP v1, v2;
	bool ok1 = false, ok2 = false;
	while (!opt)
	{
		/*
				if ((minPointsCoord(zstar, ystar) < EPS) && (maxPointsCoord(zstar, ystar) > EPS))
				{
					printf("InnerLoop| zstar: (%lf, %lf)\n", zstar.x, zstar.y);
					printf("InnerLoop| ystar: (%lf, %lf)\n", ystar.x, ystar.y);

					printf("ERROR-check\n");

					exit(0);
				}
		*/

		//if (obj1value <= zstar.x - EPS)
		if (obj1value <= zstar.x)
		{
			if (check1) check1 = false;
			ok1 = true;

			if (print) printf("\n\nInnerLoop| Part-1, depth %d (Left side of z*)\n", depth);

			NDP z1c, z2c;
			std::vector<NDP> left, right;

			NDP newL = L;
			for (int i = 1; i < (int)ndpMLeft.size(); i++)
			{
				if ((ndpMLeft[i].y < newL.y) || ((fabs(ndpMLeft[i].y - newL.y) <= EPS / 10) && ndpMLeft[i].closed)) newL = ndpMLeft[i];
				//if (ndpMLeft[i].y < newL.y) newL = ndpMLeft[i];

			}

			timeInnerLoop.stop();
			totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();

			if (print) printf("\nInnerLoop| depth = %d; (L = (%lf, %lf); zstar = (%lf, %lf);  R = (%lf; %lf))\n", depth, newL.x, newL.y, ystar.x, ystar.y, zstar.x, zstar.y);
			innerLoopRecursive(z1c, z2c, ystar, newL, zstar, left, right, List, depth + 1, true);
			if (print) printf("\nInnerLoop| back to depth = %d\n", depth);

			timeInnerLoop.reset();
			timeInnerLoop.start();

			for (int i = 0; i < (int)left.size(); i++) ndpMLeft.push_back(left[i]);
			for (int i = 0; i < (int)right.size(); i++) ndpMLeft.push_back(right[i]);

			left.clear();
			right.clear();

			NDP v1 = ndpMLeft[0];
			for (int i = 1; i < (int)ndpMLeft.size(); i++)
			{
				if (ndpMLeft[i].y < v1.y) v1 = ndpMLeft[i];
			}

			if (print) printf("InnerLoop| v1: (%lf, %lf)\n", v1.x, v1.y);

			if (fabs((lambda1 * v1.x + lambda2 * v1.y) - wTzstar) <= EPS)
			{
				z1.x = v1.x;
				z1.y = v1.y;

				z1.closed = true;
				if (print) printf("InnerLoop| z1.closed = true\n");
			}
			else
			{
				z1.x = (wTzstar - lambda2 * v1.y) / lambda1;
				z1.y = v1.y;

				z1.closed = false;
				if (print) printf("InnerLoop| z1.closed = false\n");
			}

			if (print) printf("InnerLoop| z1-NDP: (%lf, %lf)\n", z1.x, z1.y);
			if (!equalPoints(z1, zstar))
			{
				if ((z1.x > zstar.x) || (z1.y < zstar.y))
				{
					printf("InnerLoop| z1-NDP: (%lf, %lf)\n", z1.x, z1.y);
					printf("InnerLoop| zstar-NDP: (%lf, %lf)\n", zstar.x, zstar.y);
					printf("InnerLoop| z2-NDP: (%lf, %lf)\n", z2.x, z2.y);

					printf("\nInnerLoop| Error Inner Loop-2!\n");
					exit(0);
				}
			}

			if (print) printf("\nInnerLoop| End-Part1, depth %d\n\n", depth);
		}
		else
			//if (obj1value >= zstar.x + EPS)
		{
			if (check2) check2 = false;
			ok2 = true;

			if (print) printf("\n\nInnerLoop| Part-2, depth %d (Right side of z*)\n", depth);

			NDP z1c, z2c;
			std::vector<NDP> left, right;

			NDP newR = R;
			for (int i = 0; i < (int)ndpMRight.size(); i++)
			{
				if ((ndpMRight[i].x < newR.x) || ((fabs(ndpMRight[i].x - newR.x) <= EPS / 10) && ndpMRight[i].closed)) newR = ndpMRight[i];
				//if (ndpMRight[i].x < newR.x) newR = ndpMRight[i];

			}

			timeInnerLoop.stop();
			totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();

			if (print) printf("\nInnerLoop| depth = %d; (L = (%lf, %lf); zstar = (%lf, %lf);  R = (%lf; %lf))\n", depth, zstar.x, zstar.y, ystar.x, ystar.y, newR.x, newR.y);
			innerLoopRecursive(z1c, z2c, ystar, zstar, newR, left, right, List, depth + 1, false);
			if (print) printf("\nInnerLoop| back to depth = %d\n", depth);

			timeInnerLoop.reset();
			timeInnerLoop.start();

			for (int i = 0; i < (int)left.size(); i++) ndpMRight.push_back(left[i]);
			for (int i = 0; i < (int)right.size(); i++) ndpMRight.push_back(right[i]);

			left.clear();
			right.clear();

			NDP v2 = ndpMRight[0];
			for (int i = 1; i < (int)ndpMRight.size(); i++)
			{
				if (ndpMRight[i].x < v2.x) v2 = ndpMRight[i];
			}

			if (print) printf("InnerLoop| v2: (%lf, %lf)\n", v2.x, v2.y);

			if ((fabs(lambda1 * v2.x + lambda2 * v2.y) - wTzstar) <= EPS)
			{
				z2.x = v2.x;
				z2.y = v2.y;

				z2.closed = true;
				if (print) printf("InnerLoop| z2.closed = true\n");
			}
			else
			{
				z2.x = v2.x;
				z2.y = (wTzstar - lambda1 * v2.x) / lambda2;

				z2.closed = false;
				if (print) printf("InnerLoop| z2.closed = false\n");
			}

			if (print) printf("InnerLoop| z2-NDP: (%lf, %lf)\n", z2.x, z2.y);
			if (!equalPoints(z2, zstar))
			{
				if ((z2.x < zstar.x) || (z2.y > zstar.y))
				{
					printf("InnerLoop| z1-NDP: (%lf, %lf)\n", z1.x, z1.y);
					printf("InnerLoop| zstar-NDP: (%lf, %lf)\n", zstar.x, zstar.y);
					printf("InnerLoop| z2-NDP: (%lf, %lf)\n", z2.x, z2.y);

					printf("\nInnerLoop| Error Inner Loop-3!\n");
					exit(0);
				}
			}

			if (print) printf("\nInnerLoop| End-Part2, depth %d\n\n", depth);

		}

		if (print) printf("InnerLoop|-x z1-NDP: (%lf, %lf)\n", z1.x, z1.y);
		if (print) printf("InnerLoop|-x z2-NDP: (%lf, %lf)\n", z2.x, z2.y);
		if (print) printf("InnerLoop| zstar-NDP: (%lf, %lf)\n", zstar.x, zstar.y);

		if (equalPoints(z1, z2))
		{
			z1.closed = z2.closed = true;
			break;
		}

		if (check2)
		{
			ystar = R;
			obj1value = R.x;

			if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);
		}
		else
		{

			numberOfScalarizationIP++;
			timeScalarizationIP.reset();
			timeScalarizationIP.start();

			newSolver->createWeightedSumObjective(lambda1, lambda2);
			newSolver->addWeightedSumObjective();

			double slack1 = 0.0;
			if (ok1) slack1 = EPS;

			double slack2 = 0.0;
			if (ok2) slack2 = EPS;

#ifdef ALLCONSTRAINTS
			newSolver->addConstObj(L.x, z1.y - slack1, z2.x - slack2, R.y);
#else
			newSolver->addConstObj2(z1.y - slack1);
			newSolver->addConstObj1(z2.x - slack2);
#endif

			newSolver->addMIPStart(zstar.continuousSolution, zstar.integerSolution);

			if (!newSolver->run())
			{
				printf("ERROR11\n");
				exit(0);
			}
			//newSolver->printSolution();

			result = newSolver->getObjValue();
			if (print) printf("InnerLoop| MIP result = %lf\n", result);

			obj1value = newSolver->getObj1Value();

			ystar.x = obj1value;
			ystar.y = newSolver->getObj2Value();
			if (print) printf("InnerLoop| ystar: (%lf %lf)\n", ystar.x, ystar.y);

			//newSolver->saveIntegerSolution(); 
			ystar.continuousSolution = newSolver->getContinuousVariablesSolution();
			ystar.integerSolution = newSolver->getIntegerVariablesSolution();

			newSolver->removeWeightedSumObjective();

#ifdef ALLCONSTRAINTS
			newSolver->removeConstObj();
#else
			newSolver->removeConstObj1();
			newSolver->removeConstObj2();
#endif

			timeScalarizationIP.stop();
			totalTimeScalarizationIP += timeScalarizationIP.getCPUTotalSecs();

			if (print) printf("InnerLoop| Scal Time = %lf\n", timeScalarizationIP.getCPUTotalSecs());
			if (print) printf("InnerLoop| Scal Area = %lf\n", fabs((z2.x - slack2) - L.x) * fabs((z1.y - slack1) - R.y));
			scalMinUpdate(timeScalarizationIP.getCPUTotalSecs(), fabs((z2.x - slack2) - L.x) * fabs((z1.y - slack1) - R.y));

			if (maxTimeScal < timeScalarizationIP.getCPUTotalSecs()) maxTimeScal = timeScalarizationIP.getCPUTotalSecs();

			//if (wTzstar <= result + EPS)
			//if (ystar.integerSolution == zstar.integerSolution)
			if ((ystar.integerSolution == zstar.integerSolution) || (wTzstar <= result + EPS))
			{
				if (print) printf("InnerLoop|-2 wTzstar <= result\n\n");
				opt = true;
			}
		}
	}

	z1.connected = true;
	z2.connected = false;

	z1.id = -4;
	z2.id = -4;

	ndpMLeft.push_back(z1);
	ndpMRight.push_back(z2);

	List.pop_back();

	timeInnerLoop.stop();
	totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();
}


void SequentialAlgorithm::lineTrim(NDP& z1, NDP& z2, NDP& zstar, std::vector<std::pair<NDP, NDP> >& List)
{
	if ((int)List.size() == 0) return;

	double lambda1 = fabs(z1.y - z2.y);
	double lambda2 = fabs(z2.x - z1.x);
	double wTzstar = lambda1 * zstar.x + lambda2 * zstar.y;

	/*
		printf("\n");
		for (int i = 0; i < (int)List.size(); i++)
			{
			printf("InnerLoop| lineTrim L-%d = [(%lf, %lf), (%lf, %lf)]\n", i, List[i].first.x, List[i].first.y, List[i].second.x, List[i].second.y);
		}
		printf("\n");
	*/

	for (int i = 0; i < (int)List.size(); i++)
	{
		double lambda1i = fabs(List[i].first.y - List[i].second.y);
		double lambda2i = fabs(List[i].second.x - List[i].first.x);

		//double ratio = lambda1i/lambda2i;

			//printf("InnerLoop| [lambda1-%d = %lf; lambda2-%d = %lf]; ratio = %lf\n", i, lambda1i, lambda2i, ratio);

		if ((lambda2 < EPS / 10) && (lambda2i < EPS / 10)) continue;

		if (((lambda2 < EPS / 10) && (lambda2i > EPS / 10)) || ((lambda2 > EPS / 10) && (lambda2i < EPS / 10)) || (fabs(lambda1 / lambda2 - lambda1i / lambda2i) > EPS / 10))
		{
			double wTzstarI = lambda1i * List[i].first.x + lambda2i * List[i].first.y;

			double y1 = (wTzstarI * lambda2 - (wTzstar * lambda2i)) / (lambda1i * lambda2 - (lambda2i * lambda1));

			double y2 = (wTzstar - (lambda1 * y1)) / lambda2;

			//if ((z1.x <= y1) && (y1 <= z2.x) && (List[i].first.x <= y1) && (y1 <= List[i].second.x) && (z2.y <= y2) && (y2 <= z1.y) && (List[i].second.y <= y2) && (y2 <= List[i].first.y))
			if (((z1.x <= y1) || (fabs(z1.x - y1) <= EPS / 10)) && ((y1 <= z2.x) || (fabs(y1 - z2.x) <= EPS / 10)) && ((List[i].first.x <= y1) || (fabs(List[i].first.x - y1) <= EPS / 10)) && ((y1 <= List[i].second.x) || (fabs(y1 - List[i].second.x) <= EPS / 10))
				&& ((z2.y <= y2) || (fabs(z2.y - y2) <= EPS / 10)) && ((y2 <= z1.y) || (fabs(y2 - z1.y) <= EPS / 10)) && ((List[i].second.y <= y2) || (fabs(List[i].second.y - y2) <= EPS / 10)) && ((y2 <= List[i].first.y) || (fabs(y2 - List[i].first.y) <= EPS / 10)))
			{
				if (fabs(z1.x - y1) <= EPS / 10) y1 = z1.x;
				if (fabs(y1 - z2.x) <= EPS / 10) y1 = z2.x;
				if (fabs(List[i].first.x - y1) <= EPS / 10) y1 = List[i].first.x;
				if (fabs(y1 - List[i].second.x) <= EPS / 10) y1 = List[i].second.x;
				if (fabs(z2.y - y2) <= EPS / 10) y2 = z2.y;
				if (fabs(y2 - z1.y) <= EPS / 10) y2 = z1.y;
				if (fabs(List[i].second.y - y2) <= EPS / 10) y2 = List[i].second.y;
				if (fabs(y2 - List[i].first.y) <= EPS / 10) y2 = List[i].first.y;

				if (y1 <= zstar.x)
				{
					z1.x = y1;
					z1.y = y2;

					//printf("InnerLoop| lineTrim z1 updated, L-% = %d\n", i);
				}
				else
				{
					z2.x = y1;
					z2.y = y2;

					//printf("InnerLoop| lineTrim z2 updated, L-% = %d\n", i);
				}
			}
		}
	}
}


void SequentialAlgorithm::lineGen(NDP& z1, NDP& z2, NDP& zstar, NDP& L, NDP& R, bool toTheRightFirst)
{
	timeLineGen.reset();
	timeLineGen.start();

	newSolver->addFixingIntegerConsts2(zstar.integerSolution);

	if (equalPoints(L, R) || (L.x > R.x) || (L.y < R.y))
	{
		printf("Line Gen| Error in Line Gen-1\n");
		exit(0);
	}

	if ((!equalPoints(zstar, L) && !equalPoints(zstar, R)) && ((zstar.x < L.x) || (zstar.x > R.x) || (zstar.y > L.y) || (zstar.y < R.y)))
	{

		printf("%lf %lf %lf %lf %lf %lf\n", zstar.x, zstar.y, L.x, L.y, R.x, R.y);
		printf("Line Gen| Error in Line Gen-2\n");
		exit(0);
	}

	bool z1known = false, z2known = false, wknown = false;

	if (equalPoints(zstar, L)) { z1known = true; z1 = L; }
	if (equalPoints(zstar, R)) { z2known = true; z2 = R; }

	if (print) printf("\nLine Gen| L: (%lf, %lf)\n", L.x, L.y);
	if (print) printf("Line Gen| zstar: (%lf, %lf)\n", zstar.x, zstar.y);
	if (print) printf("Line Gen| R: (%lf, %lf)\n", R.x, R.y);

	double wx, wy;

	if (toTheRightFirst)
	{
		if (print) printf("Line Gen| To the right first\n");

		lineGenToTheRight(z2, zstar, R, L, &wknown, &z2known, &wx, &wy);

		if (!wknown) lineGenToTheLeft(z1, zstar, R, L, &wknown, &z1known, &wx, &wy);
	}
	else
	{
		if (print) printf("Line Gen| To the left first\n");

		lineGenToTheLeft(z1, zstar, R, L, &wknown, &z1known, &wx, &wy);

		if (!wknown) lineGenToTheRight(z2, zstar, R, L, &wknown, &z2known, &wx, &wy);
	}

	if (!wknown && (!z1known || !z2known))
	{
		printf("Line Gen| wknown is false\n");
		//exit(0);
		wknown = true;
	}

	if (print) printf("\n");
	if (wknown) if (print) printf("Line Gen| w: [wx = %lf; wy = %lf]\n", wx, wy);

	if (!z1known && wknown)
	{
		numberOfLP++;
		timeLP.reset();
		timeLP.start();

		newSolver->createWeightedSumObjective(wx, wy);

		newSolver->addObjective1();
		newSolver->addWeightedSumConstObj(wx * zstar.x + wy * zstar.y);
#ifndef ALLCONSTRAINTS
		newSolver->addConstObj2(L.y);
		newSolver->addConstObj1Greater(L.x);
#else
		newSolver->addConstObj(L.x, L.y, zstar.x, zstar.y);
#endif 

		newSolver->addMIPStart(zstar.continuousSolution, zstar.integerSolution);

		if (!newSolver->run())
		{
			printf("ERROR14\n");
			//exit(0);
			z1 = zstar;
		}
		else
		{
			z1.x = newSolver->getObj1Value();
			z1.y = newSolver->getObj2Value();

			z1.continuousSolution = newSolver->getContinuousVariablesSolution();
			z1.integerSolution = newSolver->getIntegerVariablesSolution();
		}

		z1known = true;

		newSolver->removeObjective1();
		newSolver->removeWeightedSumConstObj();
#ifndef ALLCONSTRAINTS
		newSolver->removeConstObj2();
		newSolver->removeConstObj1Greater();
#else
		newSolver->removeConstObj();
#endif

		timeLP.stop();
		totalTimeLP += timeLP.getCPUTotalSecs();

		if (print) printf("Line Gen| z1: (%lf, %lf)\n", z1.x, z1.y);

	}
	if (!z2known && wknown)
	{
		numberOfLP++;
		timeLP.reset();
		timeLP.start();

		newSolver->createWeightedSumObjective(wx, wy);

		newSolver->addObjective2();
		newSolver->addWeightedSumConstObj(wx * zstar.x + wy * zstar.y);
#ifndef ALLCONSTRAINTS
		newSolver->addConstObj1(R.x);
		newSolver->addConstObj2Greater(R.y);
#else
		newSolver->addConstObj(zstar.x, zstar.y, R.x, R.y);
#endif 

		newSolver->addMIPStart(zstar.continuousSolution, zstar.integerSolution);

		if (!newSolver->run())
		{
			printf("ERROR15\n");
			//exit(0);
			z2 = zstar;
		}
		else
		{
			z2.x = newSolver->getObj1Value();
			z2.y = newSolver->getObj2Value();

			z2.continuousSolution = newSolver->getContinuousVariablesSolution();
			z2.integerSolution = newSolver->getIntegerVariablesSolution();
		}

		z2known = true;

		newSolver->removeObjective2();
		newSolver->removeWeightedSumConstObj();
#ifndef ALLCONSTRAINTS
		newSolver->removeConstObj1();
		newSolver->removeConstObj2Greater();
#else
		newSolver->removeConstObj();
#endif

		timeLP.stop();
		totalTimeLP += timeLP.getCPUTotalSecs();

		if (print) printf("Line Gen| z2: (%lf, %lf)\n", z2.x, z2.y);
	}


	if (!wknown && (!equalPoints(z1, zstar) || !equalPoints(z2, zstar)))
	{
		printf("Line Gen| z1: (%lf, %lf)\n", z1.x, z1.y);
		printf("Line Gen| z2: (%lf, %lf)\n", z2.x, z2.y);
		exit(0);
	}

	if ((zstar.x < z1.x - EPS) || (zstar.x > z2.x + EPS) || (zstar.y > z1.y + EPS) || (zstar.y < z2.y - EPS))
	{
		printf("Line Gen| Error in Line Gen-3\n");
		exit(0);
	}

	if ((z1.x < L.x - EPS) || (z1.x > R.x + EPS) || (z1.y > L.y + EPS) || (z1.y < R.y - EPS))
	{
		printf("Line Gen| Error in Line Gen-4\n");
		exit(0);
	}
	if ((z2.x < L.x - EPS) || (z2.x > R.x + EPS) || (z2.y > L.y + EPS) || (z2.y < R.y - EPS))
	{
		printf("Line Gen| Error in Line Gen-5\n");
		exit(0);
	}

	newSolver->removeFixingIntegerConsts();

	timeLineGen.stop();
	totalTimeLineGen += timeLineGen.getCPUTotalSecs();
}

void SequentialAlgorithm::lineGenToTheRight(NDP& z2, NDP& zstar, NDP& R, NDP& L, bool* wknown, bool* z2known, double* wx, double* wy)
{
	NDP t;
	double delta = LINEGENDELTA * EPS;

	if (!(*z2known))
	{
		numberOfLP += 2;
		timeLP.reset();
		timeLP.start();
		lexminObj2Obj1(t, zstar.x - delta, zstar.y + delta, zstar.x + delta, zstar.y - delta, true, zstar);
		//lexminObj2Obj1(t, zstar.x - EPS, zstar.y + EPS, zstar.x + delta, zstar.y - delta, true, zstar);
		timeLP.stop();
		totalTimeLP += timeLP.getCPUTotalSecs();

		if (print) printf("Line Gen|-1 t: (%lf, %lf)\n\n", t.x, t.y);
	}

	int it = 0;
	while (!(*wknown) && !(*z2known))
	{
		if (equalPoints(zstar, t))
		{
			if (print) printf("Line Gen| z2 equal zstar\n");
			z2 = zstar;
			(*z2known) = true;
			break;
		}

		double lambda1 = fabs(zstar.y - t.y);
		double lambda2 = fabs(t.x - zstar.x);

		if (print) printf("Line Gen|-1 [lambda1 = %lf; lambda2 = %lf]\n", lambda1, lambda2);

		numberOfLP++;
		timeLP.reset();
		timeLP.start();

		newSolver->createWeightedSumObjective(lambda1, lambda2);
		newSolver->addWeightedSumObjective();

		newSolver->addMIPStart(zstar.continuousSolution, zstar.integerSolution);

		if (!newSolver->run())
		{
			printf("ERROR12\n");
			exit(0);
		}

		double obj1value = newSolver->getObj1Value();
		double obj2value = newSolver->getObj2Value();

		newSolver->removeWeightedSumObjective();

		timeLP.stop();
		totalTimeLP += timeLP.getCPUTotalSecs();

		if (print) printf("Line Gen|-1 s: (%lf, %lf)\n", obj1value, obj2value);

		double vTzstar = lambda1 * zstar.x + lambda2 * zstar.y;

		if (print) printf("Line Gen|-1 [%lf | %lf]\n", lambda1 * obj1value + lambda2 * obj2value, vTzstar);

		//if ((fabs(lambda1 * obj1value + lambda2 * obj2value - vTzstar) <= EPS/10)) 
		if ((fabs(lambda1 * obj1value + lambda2 * obj2value - vTzstar) <= 1.0e-6))
		{
			(*wx) = lambda1;
			(*wy) = lambda2;

			if ((lambda1 > EPS) && (lambda2 > EPS)) (*wknown) = true;

			break;
		}

		t.x = obj1value;
		t.y = obj2value;

		it++;
	}

	if (*z2known) if (print) printf("Line Gen|-1 z2 is known: (%lf, %lf)\n", z2.x, z2.y);
}

void SequentialAlgorithm::lineGenToTheLeft(NDP& z1, NDP& zstar, NDP& R, NDP& L, bool* wknown, bool* z1known, double* wx, double* wy)
{
	NDP t;
	double delta = LINEGENDELTA * EPS;

	if (!(*z1known))
	{
		numberOfLP += 2;
		timeLP.reset();
		timeLP.start();
		lexminObj1Obj2(t, zstar.x - delta, zstar.y + delta, zstar.x + delta, zstar.y - delta, true, zstar);
		//lexminObj1Obj2(t, zstar.x - delta, zstar.y + delta, zstar.x + EPS, zstar.y - EPS, true, zstar);
		timeLP.stop();
		totalTimeLP += timeLP.getCPUTotalSecs();

		if (print) printf("Line Gen|-2 t: (%lf, %lf)\n\n", t.x, t.y);
	}

	int it = 0;
	while (!(*wknown) && !(*z1known))
	{
		if (equalPoints(zstar, t))
		{
			if (print) printf("Line Gen| z1 equal zstar\n");
			z1 = zstar;
			(*z1known) = true;
			break;
		}

		double lambda1 = fabs(t.y - zstar.y);
		double lambda2 = fabs(zstar.x - t.x);

		if (print) printf("Line Gen|-2 [lambda1 = %lf; lambda2 = %lf]\n", lambda1, lambda2);

		numberOfLP++;
		timeLP.reset();
		timeLP.start();

		newSolver->createWeightedSumObjective(lambda1, lambda2);
		newSolver->addWeightedSumObjective();

		newSolver->addMIPStart(zstar.continuousSolution, zstar.integerSolution);

		if (!newSolver->run())
		{
			printf("ERROR13\n");
			exit(0);
		}

		double obj1value = newSolver->getObj1Value();
		double obj2value = newSolver->getObj2Value();

		newSolver->removeWeightedSumObjective();

		timeLP.stop();
		totalTimeLP += timeLP.getCPUTotalSecs();

		if (print) printf("Line Gen|-2 s: (%lf, %lf)\n", obj1value, obj2value);

		double vTzstar = lambda1 * zstar.x + lambda2 * zstar.y;

		if (print) printf("Line Gen|-2 [%lf | %lf]\n", lambda1 * obj1value + lambda2 * obj2value, vTzstar);

		//if ((fabs(lambda1 * obj1value + lambda2 * obj2value - vTzstar) <= EPS/10))
		if ((fabs(lambda1 * obj1value + lambda2 * obj2value - vTzstar) <= 1.0e-6))
		{
			(*wx) = lambda1;
			(*wy) = lambda2;

			if ((lambda1 > EPS) && (lambda2 > EPS))  (*wknown) = true;

			break;
		}

		t.x = obj1value;
		t.y = obj2value;

		it++;
	}

	if (*z1known) if (print) printf("Line Gen|-2 z1 is known: (%lf, %lf)\n\n", z1.x, z1.y);
}


bool SequentialAlgorithm::lineGenSameFrontierWithNoGood(NDP& ystar, NDP& z, NDP& L, NDP& R)
{
	newSolver->addFixingIntegerConsts2(L.integerSolution);

	if (print) printf("NoGoodNDPCheck| L: (%lf, %lf)\n", L.x, L.y);
	if (print) printf("NoGoodNDPCheck| R: (%lf, %lf)\n", R.x, R.y);

	if (!equalPoints(L, R))
	{
		if ((L.x > R.x) || (L.y < R.y))
		{
			printf("\nNoGoodNDPCheck| Error Inner Loop Same Frontier NoGood-1!\n");
			exit(0);
		}
	}
	else
	{
		printf("NoGoodNDPCheck| Error Inner Loop Same Frontier NoGood-2\n");
		exit(0);
	}

	double lambda1 = fabs(L.y - R.y);
	double lambda2 = fabs(R.x - L.x);

	if (print) printf("NoGoodNDPCheck| [lambda1 = %lf; lambda2 = %lf]\n", lambda1, lambda2);

	numberOfLP++;
	timeLP.reset();
	timeLP.start();

	newSolver->createWeightedSumObjective(lambda1, lambda2);
	newSolver->addWeightedSumObjective();

#ifdef ALLCONSTRAINTS
	newSolver->addConstObj(L.x, z.y, z.x, R.y);
#else
	newSolver->addConstObj1(z.x);
	newSolver->addConstObj2(z.y);
#endif

	newSolver->addMIPStart(R.continuousSolution, R.integerSolution);

	bool ok = false;
	if (!newSolver->run())
	{
		if (print) printf("NoGoodNDPCheck| Infeasible\n");
	}
	else
	{
		double result = newSolver->getObjValue();
		if (print) printf("NoGoodNDPCheck| result = %lf\n", result);

		//newSolver->printSolution();

		double obj1value = newSolver->getObj1Value();

		ystar.closed = true;
		ystar.x = obj1value;
		ystar.y = newSolver->getObj2Value();
		if (print) printf("NoGoodNDPCheck| ystar: (%lf %lf)\n", ystar.x, ystar.y);

		ystar.continuousSolution = newSolver->getContinuousVariablesSolution();
		ystar.integerSolution = newSolver->getIntegerVariablesSolution();

		ok = true;
	}

	newSolver->removeWeightedSumObjective();

#ifdef ALLCONSTRAINTS
	newSolver->removeConstObj();
#else
	newSolver->removeConstObj1();
	newSolver->removeConstObj2();
#endif

	timeLP.stop();
	totalTimeLP += timeLP.getCPUTotalSecs();

	newSolver->removeFixingIntegerConsts();

	return ok;
}

void SequentialAlgorithm::buildLPFrontierForNoGood(NDP& L, NDP& R)
{
	if (print) printf("\n");

	std::stack<NDP> stk;

	//L.connected = true;
	//ndpList.push_back(L);

	stk.push(R);

	NDP upperLeft = L;

	newSolver->addFixingIntegerConsts2(L.integerSolution);

	while (!stk.empty())
	{
		numberOfLP++;
		timeLP.reset();
		timeLP.start();

		NDP s;

		if (print) printf("Scalarize| Improvement ==> Solving LP\n");
		bool foundPoint = scalarize(upperLeft, stk.top(), s, upperLeft, R);
		if (print) printf("Scalarize| Improvement ==> Done!\n\n");

		timeLP.stop();
		totalTimeLP += timeLP.getCPUTotalSecs();

		if (!foundPoint)
		{
			upperLeft.connected = true;
			ndpList.push_back(upperLeft);

			upperLeft = stk.top();

			stk.pop();
		}
		else
		{
			stk.push(s);
		}
	}

	upperLeft.connected = false;
	ndpList.push_back(upperLeft);

	if (print) printf("\n");

	newSolver->removeFixingIntegerConsts();
}

void SequentialAlgorithm::buildLPFrontierForSoyluAlgorithm(NDP& L, NDP& R, std::vector<NDP>& frontierPoints)
{
	if (print) printf("\n");

	newSolver->addFixingIntegerConsts2(L.integerSolution);

	NDP t;

	numberOfLP += 2;
	timeLP.reset();
	timeLP.start();
	lexminObj2Obj1(t, L.x, L.y, R.x, R.y, true, L);
	timeLP.stop();
	totalTimeLP += timeLP.getCPUTotalSecs();

	if (print) printf("Soylu| t: (%lf, %lf)\n\n", t.x, t.y);


	if (equalPoints(L, t))
	{
		frontierPoints.push_back(L);

		newSolver->removeFixingIntegerConsts();

		return;
	}

	std::stack<NDP> stk;

	stk.push(t);

	NDP upperLeft = L;

	while (!stk.empty())
	{
		numberOfLP++;
		timeLP.reset();
		timeLP.start();

		NDP s;

		if (print) printf("Soylu| Improvement ==> Solving LP\n");
		bool foundPoint = scalarize(upperLeft, stk.top(), s, upperLeft, R);
		if (print) printf("Soylu| Improvement ==> Done!\n\n");

		timeLP.stop();
		totalTimeLP += timeLP.getCPUTotalSecs();

		if (!foundPoint)
		{
			frontierPoints.push_back(upperLeft);

			upperLeft = stk.top();

			stk.pop();
		}
		else
		{
			stk.push(s);
		}
	}

	frontierPoints.push_back(upperLeft);

	if (print) printf("\n");

	newSolver->removeFixingIntegerConsts();
}

bool SequentialAlgorithm::buildLPFrontierForMixed(NDP& zstar, NDP& L, NDP& R)
{
	if (print) printf("\n");

	std::stack<NDP> stk;

	//L.connected = true;
	//ndpList.push_back(L);

	stk.push(R);

	NDP upperLeft = L;

	newSolver->addFixingIntegerConsts2(L.integerSolution);

	while (!stk.empty())
	{
		numberOfLP++;
		timeLP.reset();
		timeLP.start();

		NDP s;

		if (print) printf("Scalarize| Improvement ==> Solving LP\n");
		bool foundPoint = scalarize(upperLeft, stk.top(), s, upperLeft, R);
		if (print) printf("Scalarize| Improvement ==> Done!\n\n");

		timeLP.stop();
		totalTimeLP += timeLP.getCPUTotalSecs();

		if (!foundPoint)
		{
			newSolver->removeFixingIntegerConsts();

			numberOfScalarizationIP++;
			timeScalarizationIP.reset();
			timeScalarizationIP.start();

			if (print) printf("Scalarize| Improvement ==> Solving IP\n");

			foundPoint = scalarize(upperLeft, stk.top(), s, upperLeft, R, true);

			if (print) printf("Scalarize| Improvement ==> Done!\n\n");

			timeScalarizationIP.stop();
			totalTimeScalarizationIP += timeScalarizationIP.getCPUTotalSecs();

			if (print) printf("Scalarize| Scal Time = %lf\n", timeScalarizationIP.getCPUTotalSecs());
			if (print) printf("Scalarize| Scal Area = %lf\n", scalArea);
			scalMinUpdate(timeScalarizationIP.getCPUTotalSecs(), scalArea);

			if (maxTimeScal < timeScalarizationIP.getCPUTotalSecs()) maxTimeScal = timeScalarizationIP.getCPUTotalSecs();

			if (foundPoint)
			{
				L = upperLeft;
				L.closed = true;
				L.connected = false;

				s.closed = true;
				s.connected = false;
				zstar = s;

				//upperLeft.connected = false; 
	//ndpList.push_back(L);

				if (print) printf("\n");

				return false;
			}
			newSolver->addFixingIntegerConsts2(L.integerSolution);

			if (!foundPoint)
			{
				upperLeft.closed = true;
				upperLeft.connected = true;
				ndpList.push_back(upperLeft);
			}

			upperLeft = stk.top();

			stk.pop();
		}
		else
		{
			stk.push(s);
		}
	}

	newSolver->removeFixingIntegerConsts();

	upperLeft.connected = false;
	ndpList.push_back(upperLeft);

	if (print) printf("\n");

	return true;
}

bool SequentialAlgorithm::scalarize(NDP& z1, NDP& z2, NDP& ystar, NDP& L, NDP& R, bool ipscal)
{
	scalArea = fabs(z2.x - L.x) * fabs(z1.y - R.y);

	if (print) printf("Scalarize| Improvement1 ==> z1: (%lf, %lf)\n", z1.x, z1.y);
	if (print) printf("Scalarize| Improvement ==> z2: (%lf, %lf)\n", z2.x, z2.y);

	if (!equalPoints(z1, z2))
	{
		if ((z1.x > z2.x) || (z1.y < z2.y))
		{
			printf("\nScalarize| Error Inner Loop Same Frontier NoGood-1!\n");
			exit(0);
		}
	}
	else
	{
		printf("Scalarize| Error Inner Loop Same Frontier NoGood-2\n");
		exit(0);
	}

	double lambda1 = fabs(z1.y - z2.y);
	double lambda2 = fabs(z2.x - z1.x);

	if (print) printf("Scalarize| Improvement ==> [lambda1 = %lf; lambda2 = %lf]\n", lambda1, lambda2);

	newSolver->createWeightedSumObjective(lambda1, lambda2);
	newSolver->addWeightedSumObjective();

#ifdef ALLCONSTRAINTS
	newSolver->addConstObj(L.x, z1.y, z2.x, R.y);
#else
	newSolver->addConstObj1(z2.x);
	newSolver->addConstObj2(z1.y);
#endif

#ifdef IMPROVEMENTMIXED
	newSolver->addMIPStart(z1.continuousSolution, z1.integerSolution);
#endif

#ifdef RESTRICTIVELINEALGORITHM
	newSolver->addMIPStart(z1.continuousSolution, z1.integerSolution);
#endif

	bool ok = false;
	if (!newSolver->run())
	{
		if (print) printf("Scalarize| Improvement ==> Infeasible\n");
	}
	else
	{
		double result = newSolver->getObjValue();
		if (print) printf("Scalarize| Improvement ==> result = %lf\n", result);

		//newSolver->printSolution();

			//double obj1value = newSolver->getObj1Value();

		ystar.closed = true;
		ystar.x = newSolver->getObj1Value();
		ystar.y = newSolver->getObj2Value();
		if (print) printf("Scalarize| Improvement ==> ystar: (%lf %lf)\n", ystar.x, ystar.y);

		ystar.continuousSolution = newSolver->getContinuousVariablesSolution();
		ystar.integerSolution = newSolver->getIntegerVariablesSolution();

		double wTzstar = lambda1 * z1.x + lambda2 * z1.y;
		if (print) printf("Scalarize| Improvement ==> wTzstar = %lf\n", wTzstar);

		if ((ipscal && ((ystar.integerSolution == z1.integerSolution) || (wTzstar <= result + EPS))) || (!ipscal && (wTzstar <= result + EPS))) { if (print) printf("Scalarize| wTzstar <= result\n"); }
		else ok = true;

		if (equalPoints(z1, ystar) || equalPoints(z2, ystar)) ok = false;
	}

	newSolver->removeWeightedSumObjective();

#ifdef ALLCONSTRAINTS
	newSolver->removeConstObj();
#else
	newSolver->removeConstObj1();
	newSolver->removeConstObj2();
#endif

	return ok;
}

void SequentialAlgorithm::restrictiveLine(NDP& z1, NDP& z2, NDP& zstar, NDP& L, NDP& R, std::vector<NDP>& ndpMLeft, std::vector<NDP>& ndpMRight)
{
	numberCallsInnerLoop++;

	if (print) printf("\n\nRestrictiveLine| Calling Line Gen...\n\n");

	if (print) printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	lineGen(z1, z2, zstar, L, R);
	if (print) printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	timeInnerLoop.reset();
	timeInnerLoop.start();

	if (print) printf("\n\nRestrictiveLine| Line Gen has finished!\n\n");

	if (print) printf("RestrictiveLine| z1: (%lf, %lf)\n", z1.x, z1.y);
	if (print) printf("RestrictiveLine| zstar: (%lf, %lf)\n", zstar.x, zstar.y);
	if (print) printf("RestrictiveLine| z2: (%lf, %lf)\n", z2.x, z2.y);

	if (!equalPoints(z1, z2))
	{
		if ((z1.x > z2.x) || (z1.y < z2.y))
		{
			printf("\nRestrictiveLine| Error Restrictive Line-1!\n");
			exit(0);
		}
	}
	else
	{
		if (print) printf("RestrictiveLine| z1 equal z2\n");

		zstar.closed = true;
		zstar.connected = false;
		z1 = zstar;
		z2 = zstar;

		ndpMLeft.push_back(z1);
		ndpMRight.push_back(z2);

		timeInnerLoop.stop();
		totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();

		return;
	}

	z1.closed = z2.closed = true;

	double lambda1 = fabs(z1.y - z2.y);
	double lambda2 = fabs(z2.x - z1.x);

	if (print) printf("RestrictiveLine| [lambda1 = %lf; lambda2 = %lf]\n", lambda1, lambda2);

	double wTzstar = lambda1 * zstar.x + lambda2 * zstar.y;
	if (print) printf("RestrictiveLine| wTzstar = %lf\n", wTzstar);

	newSolver->createAddWeightedSumConstObj(lambda1, lambda2, wTzstar);

	newSolver->addNoGoodConstraint(zstar.integerSolution);

	if (print) printf("RestrictiveLine| Searching for zR in r = (%lf %lf) (%lf %lf)\n", L.x, L.y, zstar.x, zstar.y);

	NDP zR;
	zR.closed = true;
	zR.connected = false;

	timeOfInnerLoopWithNoGoodIP.reset();
	timeOfInnerLoopWithNoGoodIP.start();

	bool zRfound;
	double triangleArea1 = (fabs(z1.y - zstar.y) * fabs(zstar.x - z1.x)) / 2;
	bool ok1 = false;
	if (!equalPoints(z1, zstar))
	{
		ok1 = true;
		zRfound = lexminObj2Obj1(zR, L.x, z1.y, zstar.x, zstar.y, false, L, true);

		if (zRfound) numberOfInnerLoopWithNoGoodIP += 2;
		else numberOfInnerLoopWithNoGoodIP++;
	}
	else zRfound = false;

	timeOfInnerLoopWithNoGoodIP.stop();
	totalTimeOfInnerLoopWithNoGoodIP += timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs();

	if (ok1)
	{
		if (print) printf("RestrictiveLine| Lex Time = %lf\n", timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs());
		if (print) printf("RestrictiveLine| Lex Area = %lf\n", fabs(lexArea - triangleArea1));
		lexMinUpdate(timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs(), fabs(lexArea - triangleArea1));

		if (maxTimeLex < timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs()) maxTimeLex = timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs();
	}

	if (zRfound) { if (print) printf("RestrictiveLine| zR found: (%lf %lf)\n", zR.x, zR.y); }
	else { if (print) printf("RestrictiveLine| zR NOT found\n"); }

	if (!zRfound)	//|| (wTzstar <= lambda1 * zR.x + lambda2 * zR.y + EPS)) 
	{
		ndpMLeft.push_back(z1);

		if (print) printf("RestrictiveLine| z1.closed = true\n");
	}
	else
	{
		if (fabs((lambda1 * zR.x + lambda2 * zR.y) - wTzstar) <= EPS)
		{
			z1.x = zR.x;
			z1.y = zR.y;

			if (print) printf("RestrictiveLine| z1.closed = true\n");
		}
		else
		{
			z1.closed = false;

			z1.x = (wTzstar - lambda2 * zR.y) / lambda1;
			z1.y = zR.y;

			if (print) printf("RestrictiveLine| z1.closed = false\n");
		}

		ndpMLeft.push_back(zR);
	}

	NDP zL;
	zL.closed = true;
	zL.connected = false;

	if (print) printf("RestrictiveLine| Searching for zL in r = (%lf %lf) (%lf %lf)\n", zstar.x, zstar.y, R.x, R.y);

	timeOfInnerLoopWithNoGoodIP.reset();
	timeOfInnerLoopWithNoGoodIP.start();

	bool zLfound;
	double triangleArea2 = (fabs(zstar.y - z2.y) * fabs(z2.x - zstar.x)) / 2;
	bool ok2 = false;
	if (!equalPoints(zstar, z2))
	{
		ok2 = true;
		zLfound = lexminObj1Obj2(zL, zstar.x, zstar.y, z2.x, R.y, false, R, true);

		if (zLfound) numberOfInnerLoopWithNoGoodIP += 2;
		else numberOfInnerLoopWithNoGoodIP++;
	}
	else zLfound = false;

	timeOfInnerLoopWithNoGoodIP.stop();
	totalTimeOfInnerLoopWithNoGoodIP += timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs();

	if (ok2)
	{
		if (print) printf("RestrictiveLine| Lex Time = %lf\n", timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs());
		if (print) printf("RestrictiveLine| Lex Area = %lf\n", fabs(lexArea - triangleArea2));
		lexMinUpdate(timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs(), fabs(lexArea - triangleArea2));

		if (maxTimeLex < timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs()) maxTimeLex = timeOfInnerLoopWithNoGoodIP.getCPUTotalSecs();
	}

	if (zLfound) { if (print) printf("RestrictiveLine| zL found: (%lf %lf)\n", zL.x, zL.y); }
	else { if (print) printf("RestrictiveLine| zL NOT found\n"); }

	if (!zLfound) // || (wTzstar <= lambda1 * zL.x + lambda2 * zL.y + EPS))  
	{
		ndpMRight.push_back(z2);

		if (print) printf("RestrictiveLine| z2.closed = true\n");
	}
	else
	{
		if (fabs((lambda1 * zL.x + lambda2 * zL.y) - wTzstar) <= EPS)
		{
			z2.x = zL.x;
			z2.y = zL.y;

			if (print) printf("RestrictiveLine| z2.closed = true\n");
		}
		else
		{
			z2.closed = false;

			z2.x = zL.x;
			z2.y = (wTzstar - lambda1 * zL.x) / lambda2;

			if (print) printf("RestrictiveLine| z2.closed = false\n");
		}

		ndpMRight.push_back(zL);
	}

	newSolver->removeWeightedSumConstObj();
	newSolver->removeNoGoodConstraint();

	timeInnerLoop.stop();
	totalTimeInnerLoop += timeInnerLoop.getCPUTotalSecs();
}




