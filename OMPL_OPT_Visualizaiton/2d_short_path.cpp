#include <ompl/base/SpaceInformation.h>
#include <ompl/base/objectives/PathLengthOptimizationObjective.h>
#include <ompl/base/objectives/StateCostIntegralObjective.h>
#include <ompl/base/objectives/MaximizeMinClearanceObjective.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
// For ompl::msg::setLogLevel
#include "ompl/util/Console.h"

// The supported optimal planners, in alphabetical order
#include <ompl/geometric/planners/informedtrees/AITstar.h>
#include <ompl/geometric/planners/informedtrees/BITstar.h>
#include <ompl/geometric/planners/cforest/CForest.h>
#include <ompl/geometric/planners/fmt/FMT.h>
#include <ompl/geometric/planners/fmt/BFMT.h>
#include <ompl/geometric/planners/prm/PRMstar.h>
#include <ompl/geometric/planners/rrt/InformedRRTstar.h>
#include <ompl/geometric/planners/rrt/RRTstar.h>
#include <ompl/geometric/planners/rrt/SORRTstar.h>


// For boost program options
#include <boost/program_options.hpp>
// For string comparison (boost::iequals)
#include <boost/algorithm/string.hpp>
// For std::make_shared
#include <memory>
#include <iostream>
#include <string>
#include <fstream>

namespace ob = ompl::base;
namespace og = ompl::geometric;


ob::OptimizationObjectivePtr getPathLengthObjective(const ob::SpaceInformationPtr& si);

class ValidityChecker : public ob::StateValidityChecker
{
public:
    ValidityChecker(const ob::SpaceInformationPtr& si) :
        ob::StateValidityChecker(si) {}
 
    // Returns whether the given state's position overlaps the
    // circular obstacle
    bool isValid(const ob::State* state) const
    {
        return this->clearance(state) > 0.0;
    }
 
    // Returns the distance from the given state's position to the
    // boundary of the circular obstacle.
    double clearance(const ob::State* state) const
    {
        // We know we're working with a RealVectorStateSpace in this
        // example, so we downcast state into the specific type.
        const ob::RealVectorStateSpace::StateType* state2D =
            state->as<ob::RealVectorStateSpace::StateType>();
 
        // Extract the robot's (x,y) position from its state
        double x = state2D->values[0];
        double y = state2D->values[1];
 
        // Distance formula between two points, offset by the circle's
        // radius
        return sqrt((x-0.5)*(x-0.5) + (y-0.5)*(y-0.5)) - 0.25;
    }
};



int main(){
// Construct the robot state space in which we're planning. We're
// planning in [0,1]x[0,1], a subset of R^2.
ob::StateSpacePtr space(new ob::RealVectorStateSpace(2));
 
// Set the bounds of space to be in [0,1].
space->as<ob::RealVectorStateSpace>()->setBounds(0.0, 1.0);
 
// Construct a space information instance for this state space
ob::SpaceInformationPtr si(new ob::SpaceInformation(space));
 
// Set the object used to check which states in the space are valid
si->setStateValidityChecker(ob::StateValidityCheckerPtr(new ValidityChecker(si)));
 
si->setup();
 
// Set our robot's starting state to be the bottom-left corner of
// the environment, or (0,0).
ob::ScopedState<> start(space);
start->as<ob::RealVectorStateSpace::StateType>()->values[0] = 0.2;
start->as<ob::RealVectorStateSpace::StateType>()->values[1] = 0.3;
 
// Set our robot's goal state to be the top-right corner of the
// environment, or (1,1).
ob::ScopedState<> goal(space);
goal->as<ob::RealVectorStateSpace::StateType>()->values[0] = 1.0;
goal->as<ob::RealVectorStateSpace::StateType>()->values[1] = 1.0;
 
// Create a problem instance
ob::ProblemDefinitionPtr pdef(new ob::ProblemDefinition(si));
 
// Set the start and goal states
pdef->setStartAndGoalStates(start, goal);

// Construct our optimizing planner using the RRTstar algorithm.
ob::PlannerPtr optimizingPlanner(new og::RRTstar(si));
 
// Set the problem instance for our planner to solve
optimizingPlanner->setProblemDefinition(pdef);
optimizingPlanner->setup();
 
// attempt to solve the planning problem within one second of
// planning time
ob::PlannerStatus solved = optimizingPlanner->solve(1.0);

std::ostream &out = std::cout;

pdef-> getSolutionPath()->print(out);

std::string outputFile = "out.txt";

std::ofstream outFile(outputFile.c_str());
std::static_pointer_cast<og::PathGeometric>(pdef->getSolutionPath())->
        printAsMatrix(outFile);
outFile.close();

}


ob::OptimizationObjectivePtr getPathLengthObjective(const ob::SpaceInformationPtr& si)
{
    return ob::OptimizationObjectivePtr(new ob::PathLengthOptimizationObjective(si));
}