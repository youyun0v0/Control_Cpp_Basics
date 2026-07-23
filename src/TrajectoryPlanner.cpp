#include "control_basics/TrajectoryPlanner.h"

#include <cmath>

namespace control_basics {

std::size_t TrajectoryPlanner::generate_1d(
    double start,
    double target,
    double max_velocity,
    double max_acceleration,
    double dt,
    Trajectory& output) const{
        if(max_velocity <= 0.0 || max_acceleration <= 0.0 || dt <= 0.0) return 0;
        TrajectoryPlanner::ProfileInfo pi = TrajectoryPlanner::calculate_profile(start, target, max_velocity, max_acceleration);
        if(pi.distance == 0.0){
            output[0] = {0, start, 0};
            return 1;
        }
        std::size_t count = 0;
        double time = 0.0;
        while(time < pi.total_time && count + 1 < output.size()){
            double speed, progress;
            if(time < pi.acceleration_time) //加速段
            {
                speed = time * max_acceleration;
                progress = 0.5 * max_acceleration * time * time;
                
            }
            else if(time >= pi.acceleration_time && time <= pi.acceleration_time + pi.constant_time) //匀速段
            {
                speed = max_velocity;
                progress = pi.acceleration_distance + (time - pi.acceleration_time) * pi.peak_velocity;
            }
            else{ //减速段
                speed = (pi.total_time - time) * max_acceleration;
                progress = pi.distance - 0.5 * max_acceleration * (pi.total_time - time) * (pi.total_time - time);
            }
            output[count].time = time;
            output[count].position = start + pi.direction * progress;
            output[count].velocity = pi.direction * speed;
            count ++;
            time += dt;
        }
        output[count].time = pi.total_time;
        output[count].position = target;
        output[count].velocity = 0.0;
        return count + 1;
    }

std::size_t TrajectoryPlanner::generate_line_2d(
    double start_x,
    double start_y,
    double target_x,
    double target_y,
    double max_velocity,
    double max_acceleration,
    double dt,
    Trajectory2D& output) const{
        if(max_velocity <= 0.0 || max_acceleration <= 0.0 || dt <= 0.0) return 0;
        double dx = target_x - start_x;
        double dy = target_y - start_y;
        double distance = std::sqrt(dx * dx + dy * dy);
        if(distance == 0){
            output[0] = {0.0, start_x, start_y, 0.0, 0.0};
            return 1;
        }
        Trajectory output1d;
        std::size_t cnt = generate_1d(0.0, distance, max_velocity, max_acceleration, dt, output1d);
        if(cnt == 0) return 0;
        for(std::size_t i = 0; i < cnt - 1; i ++){
            double pos = output1d[i].position, speed = output1d[i].velocity;
            double ratio = pos / distance;
            output[i].time = output1d[i].time;
            output[i].x = start_x + ratio * (target_x - start_x);
            output[i].y = start_y + ratio * (target_y - start_y);
            output[i].vx = speed * dx / distance;
            output[i].vy = speed * dy / distance;
        }
        output[cnt - 1].time = output1d[cnt - 1].time;
        output[cnt - 1].x = target_x;
        output[cnt - 1].y = target_y;
        output[cnt - 1].vx = 0.0;
        output[cnt - 1].vy = 0.0;
        return cnt;
    }

    TrajectoryPlanner::ProfileInfo TrajectoryPlanner::calculate_profile(
    double start,
    double target,
    double max_velocity,
    double max_acceleration) const{
        ProfileInfo pi;
        if(max_velocity <= 0.0 || max_acceleration <= 0.0) return pi;
        pi.distance = std::fabs(start - target);
        pi.direction = start < target ? 1.0 : -1.0 ;
        if(pi.distance == 0.0){
            pi.is_valid = true;
            return pi;
        }
        pi.is_valid = true;
        double time_to_max = max_velocity / max_acceleration;
        double distance_to_max = 0.5 * time_to_max * max_velocity;
        if(2 * distance_to_max <= pi.distance) //梯形
        {
            pi.is_trapezoid = true;
            pi.peak_velocity = max_velocity;
            pi.acceleration_time = time_to_max;
            pi.constant_time = ( pi.distance - 2 * distance_to_max ) / max_velocity;
            pi.total_time = 2 * pi.acceleration_time + pi.constant_time;
            pi.acceleration_distance = distance_to_max;
            pi.constant_distance = pi.distance - 2 * distance_to_max;
        }
        else{ //三角
            pi.is_trapezoid = false;
            pi.peak_velocity = std::sqrt(pi.distance * max_acceleration);
            pi.acceleration_time = pi.peak_velocity / max_acceleration;
            pi.constant_time = 0.0;
            pi.total_time = 2 * pi.acceleration_time;
            pi.acceleration_distance = pi.distance / 2;
            pi.constant_distance = 0.0;
        }
        return pi;
    }

}  // namespace control_basics