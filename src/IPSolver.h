#ifndef IPSOLVER_H
#define IPSOLVER_H

#include "Constants.h"
#include "NDP.h"

#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <time.h>
#include <limits>

#include <iostream>
#include <fstream>

#include <ilcplex/ilocplex.h>
#include <ilcplex/cplex.h>

class IPSolver
{
public:
	const std::string& getName() const { return name; }

	bool run();

	void addObjective1();
	void removeObjective1();

	void addObjective2();
	void removeObjective2();

	void addConstObj(double p1x, double p1y, double p2x, double p2y);
	void removeConstObj();

	void addConstObj1(double rhs);
	void addConstObj2(double rhs);

	void removeConstObj1();
	void removeConstObj2();

	void addConstObj1Greater(double rhs);
	void addConstObj2Greater(double rhs);

	void removeConstObj1Greater();
	void removeConstObj2Greater();

	void addFixingIntegerConsts2(std::vector<int>& integerSolution);
	void removeFixingIntegerConsts();

	void createWeightedSumObjective(double lambda1, double lambda2);
	void createAddWeightedSumConstObj(double lambda1, double lambda2, double rhs);

	void addWeightedSumObjective();
	void removeWeightedSumObjective();

	void addWeightedSumConstObj(double rhs);
	void removeWeightedSumConstObj();

	double getObjValue() const { return objValue; }

	void printSolution();

	double getObj1Value();

	double getObj2Value();

	std::vector<double> getContinuousVariablesSolution();
	std::vector<int> getIntegerVariablesSolution();

	void addMIPStart(std::vector<double>& continuousVariablesSolution, std::vector<int>& integerVariablesSolution);

	void addNoGoodConstraint(std::vector<int>& integerSolution);
	void removeNoGoodConstraint();

	void addNoGoodConstraint2(std::vector<int>& integerSolution);
	void removeNoGoodConstraint2();

	void removeNoGoodConstraint3();

	void createAddIsParetoConstObj(double z1x, double z1y, double z2x, double z2y);
	void removeIsParetoConstObj();

	void printModel();

	void createAddFrontierConst(std::vector<NDP>& frontierPoints, std::vector<int>& binarySolution);
	void removeFrontierConst();

	void createAddConvexCombination(double z1x, double z1y, double z2x, double z2y);

	void removeConvexCombination();

	inline void writeLP(const char* fname) { cplex.exportModel(fname); }

	void read(int argc, char** argv);

	static IPSolver* getInstance();
	static void destroy();

private:
	std::string name;

	IloEnv env;
	IloModel model;
	IloCplex cplex;

	IloModel modelCopy;

	IloIntVarArray integerVariables;
	IloNumVarArray continuousVariables;

	IloNumVar lambda;
	IloNumVar beta1;
	IloNumVar beta2;

	IloNumVarArray lambdaMip;
	IloIntVarArray intervals;
	IloRangeArray lambdaIntervals;
	IloRange convexity;
	IloRange intervalSum;
	IloRange obj1Const;
	IloRange obj2Const;

	IloRange obj2Const1;
	IloRange obj2Const2;

	IloObjective objective1;
	IloObjective objective2;

	IloExpr constObj1;
	IloExpr constObj2;

	IloRange constObj1range1;
	IloRange constObj1range2;
	IloRange constObj2range1;
	IloRange constObj2range2;

	IloNumVarArray var;
	IloRangeArray rng;

	IloExpr weightedSumConstObj;
	IloRange weightedSumConstObjrange;

	IloObjective objectiveWeightedSum;
	IloRangeArray fixingIntegerConsts;

	IloRange noGoodConstraint;
	IloRange noGoodConstraint2;
	IloRange noGoodConstraint3;

	IloObjective objIsPareto;
	IloRange constIsPareto1;
	IloRange constIsPareto2;

	std::vector<int> continuousVariablesIndex;
	std::vector<int> integerVariablesIndex;

	double objValue;

	static IPSolver* singleton;
	IPSolver();
	~IPSolver();
};

#endif
