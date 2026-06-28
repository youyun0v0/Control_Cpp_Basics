#include "control_basics/MathUtils.h"
namespace control_basics
{
    double clamp(double value, double min_value, double max_value) //把数限制在某个范围内
    {
        if (value < min_value)
            return min_value;
        else if (value > max_value)
            return max_value;
        else
            return value;
    }
    double mean(const double* data, std::size_t size) //计算平均值 //这里的const保证了函数不会修改数组的数据
    {
        if (size == 0 || data == nullptr) //数组是否为空？是否为空指针？
            return 0.0;

        double sum = 0.0;
        for (std::size_t i = 0; i < size; ++i)
        {
            sum += data[i];
        }
        return sum / static_cast<double>(size);
    }
    double max_value(const double* data, std::size_t size) //计算最大值
    {
        
        if (data == nullptr || size == 0) 
            return 0.0;
            
        double maxn = data[0];
        for(std::size_t i = 1; i < size; i++)
        {
            if (data[i] > maxn) maxn = data[i];
        }
        return maxn;
    }
    int normalize_pwm(double command) //反归一化，映射给pwm
    {
        const double limited = clamp(command , -1.0, 1.0);
        return static_cast<int>(limited*1000); //强制类型转换
    }
    double range(const double *data ,std::size_t size)
    {
        if(data == nullptr || size == 0) return 0.0;
        double maxn = data[0], minn = data[0];
        for(std::size_t i=1; i < size; i++)
        {
            if(data[i] > maxn) maxn = data[i];
            else if(data[i] < minn) minn = data[i];
        }
        return maxn - minn;
    }
}