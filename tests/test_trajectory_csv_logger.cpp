#include "control_basics/TrajectoryCsvLogger.h"
#include <cassert>
#include <string>
#include <sstream>

namespace control_basics{

void test_csv_has_header(){
    TrajectoryCsvLogger logger;
    std::ostringstream output;
    TrajectoryPlanner::Trajectory2D trajectory;
    logger.write_2d(output, trajectory, 0);
    std::string csv = output.str();
    assert(csv.find("time,x,y,vx,vy") != std::string::npos);
}

void test_csv_writes_one_point(){
        TrajectoryCsvLogger logger;
        std::ostringstream output;
        TrajectoryPlanner::Trajectory2D trajectory{};

        trajectory[0].time = 0.1;
        trajectory[0].x = 1.0;
        trajectory[0].y = 2.0;
        trajectory[0].vx = 3.0;
        trajectory[0].vy = 4.0;

        logger.write_2d(output, trajectory, 1);

        std::string csv = output.str();

        assert(csv.find("0.1,1,2,3,4") != std::string::npos);
    }

    void test_csv_writes_only_valid_count(){
        TrajectoryCsvLogger logger;
        std::ostringstream output;
        TrajectoryPlanner::Trajectory2D trajectory{};

        trajectory[0].time = 0.1;
        trajectory[0].x = 1.0;
        trajectory[0].y = 2.0;
        trajectory[0].vx = 3.0;
        trajectory[0].vy = 4.0;

        trajectory[1].time = 0.2;
        trajectory[1].x = 10.0;
        trajectory[1].y = 20.0;
        trajectory[1].vx = 30.0;
        trajectory[1].vy = 40.0;

        logger.write_2d(output, trajectory, 1); //count = 1 ,故不能放入第二个点

        std::string csv = output.str();

        assert(csv.find("0.1,1,2,3,4") != std::string::npos);
        assert(csv.find("0.2,10,20,30,40") == std::string::npos);
    }   

}



int main(){
    control_basics::test_csv_has_header();
    control_basics::test_csv_writes_one_point();
    control_basics::test_csv_writes_only_valid_count();
    return 0;
}