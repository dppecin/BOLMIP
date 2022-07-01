#ifndef SEQUENTIALALGORITHM_H 
#define SEQUENTIALALGORITHM_H

#include "Constants.h"
#include "NDP.h"
#include "Rectangle.h"
#include "IPSolver.h"
#include "CPUTimer.h"

#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>
#include <time.h>
#include <fstream>
#include <queue>
#include <stack>


struct CompareByArea 
{
	bool operator()(const Rectangle& left, const Rectangle& right) const
	{
		return (left._area < right._area);
	}
};

typedef struct approxSol 
{
	double stepTime;
	double totalTime;
	double areaLargestBox;
	int numberIsolatedPoints;
	int numberNDPs;
	double lengthNDLS;
	double lengthVerticalGap;
	double lengthHorizontalGap;
	double proportionSolvedArea;
	int numberOfIntegerSolutions;		

} approxSol;

typedef struct LexMin
{
	double time;
	double area;
} lexMin;

typedef struct ScalMin
{
	double time;
	double area;
} scalMin;

class SequentialAlgorithm 
{
public:
    SequentialAlgorithm();
    ~SequentialAlgorithm();

    void run();

	void printStatistics();
    
private:
    IPSolver* newSolver; 
    std::vector<Rectangle> stack;
	std::priority_queue<Rectangle, std::vector<Rectangle>, CompareByArea> pqueue;

    std::vector<NDP> ndpList;

	std::vector<NDP> ndpList2;

	bool print;

	std::vector<approxSol> approxSolVector;
	std::vector<std::vector<NDP> > approxNDPMatrix;

	std::vector<lexMin> lexMinVector;
	std::vector<scalMin> scalMinVector;

	int numberOfRectanglesSolved;
	int numberOfRectanglesEqualIPs;

	int numberOfRectanglesZstarOnCutLine;
	int numberOfRectanglesZstarNotOnCutLine;

	double totalArea;  
	double totalAreaRectanglesSolved; 
	double remainingArea; 
	int noGoodSuccessRate;
	int noGoodNumberOfCalls;

	int numberOfLexicographicIP;
	int numberOfMinIP;
	int numberOfScalarizationIP; 
	int numberOfScalarizationWithNoGoodIP; 
	int numberOfLP; 
	int numberOfNDPs; 
	int numberOfInnerLoopWithNoGoodIP; 
	int innerLoopNumberOfLP;  

	int numberOfLineSegments;
	int numberOfOpenLineSegments;
	int numberOfHalfOpenLineSegments;
	int numberOfClosedLineSegments;
	int numberOfPoints;

	int numberOfRectanglesProducedEqualZero;
	int numberOfRectanglesProducedEqualOne;
	int numberOfRectanglesProducedEqualTwo;

	int numberOfIntegerFrontiers;

	int numberCallsInnerLoop;
	int numberProvedOptByOneScalInnerLoop;
	int numberCallsOuterLooptoInnerLoop;
	int maximumdepth;

	CPUTimer timeLexicographicIP;
	CPUTimer timeMinIP;
	CPUTimer timeScalarizationIP;
	CPUTimer timeScalarizationWithNoGoodIP; 
	CPUTimer timeLP;
	CPUTimer timeOfInnerLoopWithNoGoodIP; 

	CPUTimer timeOuterLoop;
	CPUTimer timeInnerLoop;
	CPUTimer timeLineGen; 
	CPUTimer timeTotalTime;
	CPUTimer timeSoFar;

	double totalTimeLexicographicIP;
	double totalTimeMinIP;
	double totalTimeScalarizationIP;
	double totalTimeScalarizationWithNoGoodIP; 
	double totalTimeLP;
	double totalTimeOfInnerLoopWithNoGoodIP;
	double innerLoopTotalTimeLP; 
	double maxTimeScal;
	double maxTimeLex;

	double totalTimeSoFar;
	double totalTime;

	double totalTimeOuterLoop;
	double totalTimeInnerLoop;
	double totalTimeLineGen; 

	double approxTotalTime;

	double lexArea;
	double scalArea;

	int numberOfPieceWiseLinearSolves;
	int numberOfSegmentsPieceWiseLinear;
	int segmentSolutionWasFound; 
	int numberOfPieceWiseLinearSolvesIfFeasible;
	 
	
	bool lexminObj1Obj2(NDP& p, double p1x, double p1y, double p2x, double p2y, bool addMIPStart, NDP& s, bool nogoodinnerloop = false);
	bool lexminObj2Obj1(NDP& p, double p1x, double p1y, double p2x, double p2y, bool addMIPStart, NDP& s, bool nogoodinnerloop = false);

	void outerLoop();

   	bool equalPoints(NDP& p1, NDP& p2);
	double minPointsCoord(NDP& p1, NDP& p2);
	double maxPointsCoord(NDP& p1, NDP& p2);

	void innerLoop(NDP& z1, NDP& z2, NDP& zstart, NDP& L, NDP& R, std::vector<NDP>& ndpMLeft, std::vector<NDP>& ndpMRight);

	void restrictiveLine(NDP& z1, NDP& z2, NDP& zstart, NDP& L, NDP& R, std::vector<NDP>& ndpMLeft, std::vector<NDP>& ndpMRight);

	void innerLoopRecursive(NDP& z1, NDP& z2, NDP& zstar, NDP& L, NDP& R, std::vector<NDP>& ndpMLeft, std::vector<NDP>& ndpMRight, std::vector<std::pair<NDP, NDP> >& List, int depth, bool lineGenToTheRight);

	void lineTrim(NDP& z1, NDP& z2, NDP& zstar, std::vector<std::pair<NDP, NDP> >& L);

	void lineGen(NDP& z1, NDP& z2, NDP& zstart, NDP& L, NDP& R, bool toTheRightFirst = true);

	void lineGenToTheRight(NDP& z2, NDP& zstar, NDP& R, NDP& L, bool *wknown, bool *z2known, double *wx, double *wy);

	void lineGenToTheLeft(NDP& z1, NDP& zstar, NDP& R, NDP& L, bool *wknown, bool *z1known, double *wx, double *wy);

	bool lineGenSameFrontierWithNoGood(NDP& p, NDP& z, NDP& L, NDP& R);

	void buildLPFrontierForNoGood(NDP& L, NDP& R);

	void buildLPFrontierForSoyluAlgorithm(NDP& L, NDP& R, std::vector<NDP>& frontierPoints);

	bool buildLPFrontierForMixed(NDP& zstar, NDP& L, NDP& R);

	bool scalarize(NDP& z1, NDP& z2, NDP& ystar, NDP& L, NDP& R, bool ipscal = false);

	bool isSmallRectangle(double p1x, double p1y, double p2x, double p2y); 

	bool isSameFrontier(NDP &p1, NDP &p2);

	bool restrictiveLineToTheRight(NDP& zL, NDP& z1, NDP& z2, NDP& L, NDP& R);
	void restrictiveLineAlgorithm();

	void restrictiveFrontierAlgorithm();

	bool restrictiveFrontierToTheRight(NDP& zL, std::vector<NDP>& frontierPoints, NDP& L, NDP& R, NDP& previous);  

	void approximateSolution(double totalTimeSoFar);

	void scalMinUpdate(double time, double area);

	void lexMinUpdate(double time, double area);

};

#endif 
