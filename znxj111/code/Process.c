#include "zf_common_headfile.h"
#include <math.h>
#include "Process.h"

// 全局变量
float filtered_box_x = 0;
float filtered_box_y = 0;



// 进行滤波
void update_vision_data(float new_x, float new_y) {
    // 一阶低通滤波
    float alpha = 0.3; // 滤波系数，越小越平滑
    filtered_box_x = alpha * new_x + (1 - alpha) * filtered_box_x;
    filtered_box_y = alpha * new_y + (1 - alpha) * filtered_box_y;
}
