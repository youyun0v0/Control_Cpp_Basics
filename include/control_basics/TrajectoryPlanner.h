#pragma once

#include <array>
#include <cstddef>

namespace control_basics {

struct TrajectoryPoint {
    double time = 0.0;
    double position = 0.0;
    double velocity = 0.0;
};

struct Trajectory2DPoint {
    double time = 0.0;
    double x = 0.0;
    double y = 0.0;
    double vx = 0.0;
    double vy = 0.0;
};

class TrajectoryPlanner {
public:
    static constexpr std::size_t MaxPoints = 128;
    using Trajectory = std::array<TrajectoryPoint, MaxPoints>;//一维轨迹数组 命名为缩写
    using Trajectory2D = std::array<Trajectory2DPoint, MaxPoints>;//二维轨迹数组
    
    std::size_t generate_1d(double start,
    double target,
    double max_velocity,
    double max_acceleration,
    double dt,
    Trajectory& output) const;

    std::size_t generate_line_2d(
    double start_x,
    double start_y,
    double target_x,
    double target_y,
    double max_velocity,
    double max_acceleration,
    double dt,
    Trajectory2D& output) const;

private:
    struct ProfileInfo {
        bool is_valid = false;
        bool is_trapezoid = false;
        double distance = 0.0;
        double direction = 1.0;
        double peak_velocity = 0.0;
        double acceleration_time = 0.0;
        double constant_time = 0.0;
        double total_time = 0.0;
        double acceleration_distance = 0.0;
        double constant_distance = 0.0;
    };
    ProfileInfo calculate_profile(
    double start,
    double target,
    double max_velocity,
    double max_acceleration) const;
};

}  // namespace control_basics