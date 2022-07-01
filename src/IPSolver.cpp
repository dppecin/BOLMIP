#ifdef DEBUG
#include <vld.h>
#undef max
#endif

#include "IPSolver.h"

IPSolver* IPSolver::singleton = nullptr;

#define IFS

#include <iostream>

using namespace std;

//#define DEBUG

IPSolver::IPSolver() : model(env, "BOMIP"), cplex(env), var(env), rng(env), continuousVariables(env), integerVariables(env), objIsPareto(env)
{

}

IPSolver::~IPSolver()
{
	cplex.end();
	env.end();
}

IPSolver* IPSolver::getInstance()
{
	if (singleton == nullptr)
		singleton = new IPSolver();
	return singleton;
}

void IPSolver::destroy()
{

	if (singleton != nullptr)
		delete singleton;
	singleton = nullptr;
}

void IPSolver::read(int argc, char** argv)
{
	name = argv[1];

	cplex.importModel(model, argv[1], objective1, var, rng);

	constObj1 = objective1.getExpr();

	objective2 = IloMinimize(env, rng[rng.getSize() - 1].getExpr());

	constObj2 = objective2.getExpr();

	model.remove(rng[rng.getSize() - 1]);
	rng.remove(rng.getSize() - 1);

	model.remove(objective1);


	for (int v = 0; v < (int)var.getSize(); v++)
	{
		if ((var[v].getType() == IloNumVar::Bool) || (var[v].getType() == IloNumVar::Int)) integerVariables.add(var[v]);
		if (var[v].getType() == IloNumVar::Float) continuousVariables.add(var[v]);
	}

	cplex.extract(model);

	cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
	//cplex.setParam(IloCplex::RootAlg, IloCplex::Primal);
	//cplex.setParam(IloCplex::RootAlg, IloCplex::AutoAlg);

	//cplex.setParam(IloCplex::Param::Advance, 0);

	cplex.setParam(IloCplex::MIPDisplay, 0);
	cplex.setParam(IloCplex::CutUp, IloInfinity);
	cplex.setParam(IloCplex::EpGap, 1e-12);

	cplex.setParam(IloCplex::EpInt, 0);
	cplex.setParam(IloCplex::EpRHS, EPSCPL);
	cplex.setParam(IloCplex::EpOpt, EPSCPL);

	//cplex.setParam(IloCplex::ParallelMode, IloCplex::Opportunistic);
	//cplex.setParam(IloCplex::PolishAfterTime, 5);
	//cplex.setParam(IloCplex::TiLim, 10);	

	cplex.setParam(IloCplex::Threads, 1);
}

bool IPSolver::run()
{
	bool ok = false;

	cplex.solve();

	if (cplex.isPrimalFeasible())
	{
		objValue = cplex.getObjValue();
		ok = true;
	}

#ifdef DEBUG
	stringstream ss;
	string dbgName(name.begin(), name.end() - 3);
	ss << dbgName + "dbg" << ".lp";
	cplex.exportModel(ss.str().c_str());
#endif

	return ok;
}

void IPSolver::printModel()
{
	stringstream ss;
	string dbgName(name.begin(), name.end() - 3);
	ss << dbgName + "dbg" << ".lp";
	cplex.exportModel(ss.str().c_str());
}

void IPSolver::addObjective1()
{
	model.add(objective1);
}

void IPSolver::removeObjective1()
{
	model.remove(objective1);
}

void IPSolver::addObjective2()
{
	model.add(objective2);
}

void IPSolver::removeObjective2()
{
	model.remove(objective2);
}

void IPSolver::printSolution()
{
	IloNumArray conSol(env);
	cplex.getValues(conSol, continuousVariables);

	IloNumArray intSol(env);
	cplex.getValues(intSol, integerVariables);

	printf("\n--------------------------------------------------------------------\n");
	printf("Solution:\n");

	printf("Continuous Variables:\n");
	for (int v = 0; v < (int)continuousVariables.getSize(); v++)
		if (conSol[v] > 1.0e-5) printf("%s = %lf\n", continuousVariables[v].getName(), conSol[v]);
	printf("\n");

	printf("Integer Variables:\n");
	for (int v = 0; v < (int)integerVariables.getSize(); v++)
		if (intSol[v] > 1.0e-2) printf("%s = %lf\n", integerVariables[v].getName(), intSol[v]);
	printf("\n");

	printf("\n--------------------------------------------------------------------\n");

	conSol.end();
	intSol.end();
}

void IPSolver::addConstObj(double p1x, double p1y, double p2x, double p2y)
{
	constObj1range1 = (constObj1 <= p2x);
	constObj1range2 = (constObj1 >= p1x);

	constObj2range1 = (constObj2 <= p1y);
	constObj2range2 = (constObj2 >= p2y);

	model.add(constObj1range1);
	model.add(constObj1range2);
	model.add(constObj2range1);
	model.add(constObj2range2);
}

void IPSolver::removeConstObj()
{
	model.remove(constObj1range1);
	model.remove(constObj1range2);
	model.remove(constObj2range1);
	model.remove(constObj2range2);
}

void IPSolver::addConstObj1(double rhs)
{
	constObj1range1 = (constObj1 <= rhs);
	model.add(constObj1range1);
}

void IPSolver::addConstObj1Greater(double rhs)
{
	constObj1range2 = (constObj1 >= rhs);
	model.add(constObj1range2);
}

void IPSolver::addConstObj2(double rhs)
{
	constObj2range1 = (constObj2 <= rhs);
	model.add(constObj2range1);
}

void IPSolver::addConstObj2Greater(double rhs)
{
	constObj2range2 = (constObj2 >= rhs);
	model.add(constObj2range2);
}

void IPSolver::removeConstObj1()
{
	model.remove(constObj1range1);
}

void IPSolver::removeConstObj1Greater()
{
	model.remove(constObj1range2);
}

void IPSolver::removeConstObj2()
{
	model.remove(constObj2range1);
}

void IPSolver::removeConstObj2Greater()
{
	model.remove(constObj2range2);
}

void IPSolver::addMIPStart(std::vector<double>& continuousVariablesSolution, std::vector<int>& integerVariablesSolution)
{
	if (((int)continuousVariablesSolution.size() <= 0) || ((int)integerVariablesSolution.size() <= 0))
	{
		printf("ERROR: IPSolver::addMIPStart!\n");
		exit(0);
	}

#ifdef IFS
	IloNumVarArray startVar(env);
	IloNumArray startVal(env);

	for (int v = 0; v < (int)continuousVariablesSolution.size(); v++) startVar.add(continuousVariables[v]);
	for (int v = 0; v < (int)integerVariablesSolution.size(); v++) startVar.add(integerVariables[v]);

	for (int v = 0; v < (int)continuousVariablesSolution.size(); v++) startVal.add(continuousVariablesSolution[v]);

	for (int v = 0; v < (int)integerVariablesSolution.size(); v++) startVal.add(integerVariablesSolution[v]);

	cplex.addMIPStart(startVar, startVal);

	//startVal.end();
	//startVar.end();
#endif
}

std::vector<double> IPSolver::getContinuousVariablesSolution()
{
	IloNumArray conSol(env);
	cplex.getValues(conSol, continuousVariables);

	std::vector<double> conVarSol;
	for (int v = 0; v < (int)continuousVariables.getSize(); v++) conVarSol.push_back(conSol[v]);

	return conVarSol;
}

std::vector<int> IPSolver::getIntegerVariablesSolution()
{
	IloNumArray intSol(env);
	cplex.getValues(intSol, integerVariables);

	std::vector<int> intVarSol;
	for (int v = 0; v < (int)integerVariables.getSize(); v++) intVarSol.push_back((int)(intSol[v] + 0.5));

	return intVarSol;
}

void IPSolver::createWeightedSumObjective(double lambda1, double lambda2)
{
	IloExpr expr(env);

	expr += lambda1 * objective1 + lambda2 * objective2;

	weightedSumConstObj = expr;

	objectiveWeightedSum = IloMinimize(env, weightedSumConstObj);
	expr.end();
}

double IPSolver::getObj1Value()
{
	double value = 0.0;
	for (IloExpr::LinearIterator it = IloExpr(objective1.getExpr()).getLinearIterator(); it.ok(); ++it)
		value += (it.getCoef()) * (cplex.getValue(it.getVar()));

	return value;
}

double IPSolver::getObj2Value()
{
	double value = 0.0;
	for (IloExpr::LinearIterator it = IloExpr(objective2.getExpr()).getLinearIterator(); it.ok(); ++it)
		value += (it.getCoef()) * (cplex.getValue(it.getVar()));

	return value;
}

void IPSolver::addFixingIntegerConsts2(std::vector<int>& integerSolution)
{
	for (int c = 0; c < (int)integerVariables.getSize(); c++) integerVariables[c].setBounds(integerSolution[c], integerSolution[c]);
}

void IPSolver::removeFixingIntegerConsts()
{
	for (int c = 0; c < (int)integerVariables.getSize(); c++) integerVariables[c].setBounds(0, 1);
}

void IPSolver::addNoGoodConstraint(std::vector<int>& binarySolution)
{
	for (int i = 0; i < (int)binarySolution.size(); i++)
		if ((binarySolution[i] < 0) || (binarySolution[i] > 1))
		{
			printf("\nERROR: No-Good works just for the binary case\n");
			exit(0);
		}

	IloExpr expr(env);
	for (int c = 0; c < (int)integerVariables.getSize(); c++)
	{
		if (binarySolution[c] == 1) expr += (1 - integerVariables[c]);
		else expr += integerVariables[c];
	}

	noGoodConstraint = (expr >= 1);
	model.add(noGoodConstraint);

	expr.end();
}

void IPSolver::addNoGoodConstraint2(std::vector<int>& binarySolution)
{
	for (int i = 0; i < (int)binarySolution.size(); i++)
		if ((binarySolution[i] < 0) || (binarySolution[i] > 1))
		{
			printf("\nERROR: No-Good works just for the binary case\n");
			exit(0);
		}

	IloExpr expr(env);
	for (int c = 0; c < (int)integerVariables.getSize(); c++)
	{
		if (binarySolution[c] == 1) expr += (1 - integerVariables[c]);
		else expr += integerVariables[c];
	}

	noGoodConstraint2 = (expr >= 1);
	model.add(noGoodConstraint2);

	expr.end();
}

void IPSolver::removeNoGoodConstraint()
{
	model.remove(noGoodConstraint);
}

void IPSolver::removeNoGoodConstraint2()
{
	model.remove(noGoodConstraint2);
}

void IPSolver::removeNoGoodConstraint3()
{
	model.remove(noGoodConstraint3);
}

void IPSolver::addWeightedSumObjective()
{
	model.add(objectiveWeightedSum);
}

void IPSolver::removeWeightedSumObjective()
{
	model.remove(objectiveWeightedSum);
}

void IPSolver::addWeightedSumConstObj(double rhs)
{
	weightedSumConstObjrange = (weightedSumConstObj <= rhs);
	model.add(weightedSumConstObjrange);
}

void IPSolver::removeWeightedSumConstObj()
{
	model.remove(weightedSumConstObjrange);
}

void IPSolver::createAddWeightedSumConstObj(double lambda1, double lambda2, double rhs)
{
	weightedSumConstObjrange = (lambda1 * objective1 + lambda2 * objective2 <= rhs);
	model.add(weightedSumConstObjrange);
}

void IPSolver::createAddConvexCombination(double z1x, double z1y, double z2x, double z2y)
{
	IloNumVar beta1(env);
	IloNumVar beta2(env);
	IloNumVar lambda(env);

	lambda.setBounds(0, 1);

	constIsPareto1 = (constObj1 - (z1x * lambda + (1 - lambda) * z2x) <= 0);

	constIsPareto2 = (constObj2 - (z1y * lambda + (1 - lambda) * z2y) <= 0);

	model.add(constIsPareto1);

	model.add(constIsPareto2);
}

void IPSolver::removeConvexCombination()
{
	model.remove(constIsPareto1);
	model.remove(constIsPareto2);
}

void IPSolver::createAddIsParetoConstObj(double z1x, double z1y, double z2x, double z2y)
{
	IloNumVar beta1(env);
	IloNumVar beta2(env);
	IloNumVar lambda(env);

	beta1.setLB(0);
	beta2.setLB(0);
	lambda.setBounds(0, 1);

	objIsPareto = IloMinimize(env, -beta1 - beta2);

	constIsPareto1 = (constObj1 - (z1x * lambda + (1 - lambda) * z2x - beta1) <= 0);

	constIsPareto2 = (constObj2 - (z1y * lambda + (1 - lambda) * z2y - beta2) <= 0);

	model.add(constIsPareto1);

	model.add(constIsPareto2);

	model.add(objIsPareto);
}

void IPSolver::removeIsParetoConstObj()
{
	model.remove(constIsPareto1);

	model.remove(constIsPareto2);

	model.remove(objIsPareto);
}

void IPSolver::createAddFrontierConst(std::vector<NDP>& frontierPoints, std::vector<int>& binarySolution)
{
	lambdaMip = IloNumVarArray(env, (int)frontierPoints.size(), 0.0, 1.0);
	intervals = IloIntVarArray(env, (int)frontierPoints.size() - 1, 0, 1);

	lambdaIntervals = IloRangeArray(env);

	lambdaIntervals.add(lambdaMip[0] - intervals[0] <= 0);

	for (int it = 1; it < (int)frontierPoints.size() - 1; it++)
		lambdaIntervals.add(lambdaMip[it] - intervals[it - 1] - intervals[it] <= 0);
	lambdaIntervals.add(lambdaMip[(int)frontierPoints.size() - 1] - intervals[(int)frontierPoints.size() - 2] <= 0);

	IloExpr expr(env);
	for (int it = 0; it < (int)lambdaMip.getSize(); it++)
	{
		expr += lambdaMip[it];
	}
	convexity = (expr == 1.0);
	expr.end();

	IloExpr expr1(env);
	for (int it = 0; it < (int)intervals.getSize(); it++)
	{
		expr1 += intervals[it];
	}
	intervalSum = (expr1 == 1);
	expr1.end();

	IloExpr expr2(env);
	for (int it = 0; it < (int)lambdaMip.getSize(); it++)
		expr2 += frontierPoints[it].x * lambdaMip[it];

	obj1Const = (constObj1 - expr2 <= 0);
	// obj1Const = (constObj1 - expr2 == 0); What we should use (<= or ==)? 

	expr2.end();

	IloExpr expr3(env);
	for (int it = 0; it < (int)lambdaMip.getSize(); it++)
		expr3 += frontierPoints[it].y * lambdaMip[it];

	obj2Const = (constObj2 - expr3 <= 0);
	expr3.end();

	model.add(lambdaMip);
	model.add(intervals);
	model.add(lambdaIntervals);
	model.add(convexity);
	model.add(intervalSum);
	model.add(obj1Const);
	model.add(obj2Const);

	// required no-good for the first line segment of frontierPoints
	IloExpr expr4(env);
	for (int c = 0; c < (int)integerVariables.getSize(); c++)
	{
		if (binarySolution[c] == 1) expr4 += (1 - integerVariables[c]);
		else expr4 += integerVariables[c];
	}

	//noGoodConstraint2 = (expr4 >= 1);
	noGoodConstraint2 = (expr4 - intervals[0] >= 0);
	model.add(noGoodConstraint2);

	expr4.end();
}

void IPSolver::removeFrontierConst()
{
	model.remove(lambdaMip);
	model.remove(intervals);
	model.remove(lambdaIntervals);
	model.remove(convexity);
	model.remove(intervalSum);
	model.remove(obj1Const);
	model.remove(obj2Const);

	model.remove(noGoodConstraint2);
}
