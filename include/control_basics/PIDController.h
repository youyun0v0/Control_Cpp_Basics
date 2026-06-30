#pragma once

namespace control_basics {

class PIDController {
public:
    PIDController(double kp, double ki, double kd);

    double update(double setpoint, double measurement, double dt);
    void reset();

    void set_output_limits(double min_output, double max_output);
    void set_integral_limits(double min_integral, double max_integral);

    double integral() const;
    double previous_error() const;

private:
    double kp_;
    double ki_;
    double kd_;
    double integral_;
    double previous_error_;
    bool has_previous_error_;
    double output_min_;
    double output_max_;
    double integral_min_;
    double integral_max_;
};

}  // namespace control_basics