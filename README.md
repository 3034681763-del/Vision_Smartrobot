# Vision Smartrobot

基于 NXP i.MX RT1064 与逐飞 RT1064 开源库的三轮全向智能车控制工程。代码包含摄像头循迹、圆环/十字元素处理、IMU 航向角积分、三路编码器速度闭环、视觉目标定位、识别后旋转/推箱等任务流程。

## 工程概览

本仓库当前以 `control` 目录作为工程根目录，主要由两部分组成：

- `libraries/`：逐飞 RT1064 底层库、SDK、驱动、设备组件。
- `znxj111/`：用户工程、MDK/IAR 工程文件和三轮车业务控制代码。

核心用户代码位于：

- `znxj111/user/src/main.c`：系统初始化和主循环入口。
- `znxj111/user/src/isr.c`：PIT、UART、CSI、GPIO 等中断处理。
- `znxj111/code/`：电机、编码器、IMU、PID、循迹、视觉通信和任务状态机。
- `znxj111/code/ISLAND_NEW/`：圆环元素识别与补线状态机。

## 硬件与外设

- 主控：RT1064DVL6A。
- 运动结构：三轮全向底盘。
- 电机：3 路 PWM + 方向 GPIO。
- 编码器：3 路正交编码器。
- 摄像头：MT9V03X，用于赛道图像循迹。
- IMU：IMU660RA，用于 yaw 角积分。
- 视觉识别：OpenArt/MCX 通过 UART 上报目标框和类别。
- 调试显示：TFT180/IPS200。
- 任务触发输入：`C26` GPIO。

## 控制流程

程序启动后执行以下初始化：

1. 系统时钟初始化。
2. MT9V03X 摄像头初始化。
3. OpenArt/MCX 串口视觉初始化。
4. TFT 屏幕初始化。
5. 三路电机和三路编码器初始化。
6. PID 参数初始化。
7. IMU 初始化与零漂校准。
8. PIT 定时器初始化。

运行时分为两个主要周期：

- 主循环持续调用 `Tracking()`，完成图像二值化、搜线、十字检测和圆环检测。
- `PIT_CH0` 每 2 ms 调用 `my_motor_control()`，执行任务状态机和电机速度闭环。
- `PIT_CH1` 每 5 ms 读取 IMU，并积分得到航向角 `T_M`。

## 三轮运动模型

运动控制先将车体速度分解为三个轮子的目标速度，再分别做编码器速度闭环：

```c
Target_Speed_C = -2/3 * Vx + 1/3 * V_angle;
Target_Speed_A =  1/3 * Vx + sqrt(3)/3 * Vy + 1/3 * V_angle;
Target_Speed_B =  1/3 * Vx - sqrt(3)/3 * Vy + 1/3 * V_angle;
```

部分旋转/定位流程使用 `Kinematic_Analysis_new()`，其中旋转项比例与基础模型不同，用于适配当前底盘和调参结果。

## 任务状态机

`znxj111/code/my_true_control.c` 中的 `Control_Mode` 是整车主状态机：

- `0`：正常循迹。
- `12`：检测到目标区域后短暂停车。
- `1/2`：视觉定位，使目标框中心靠近设定位置。
- `7/3`：延时后根据目标方向旋转。
- `9/10/11`：二次定位与准备推箱。
- `4/5`：执行推箱动作，并通过 `C26` 输入判断阶段切换。
- `6`：航向回正，完成后回到循迹。

## 图像处理与赛道元素

`Tracking()` 的处理链路：

1. `get_image()` 转存摄像头图像。
2. `turn_to_bin()` 使用大津法二值化。
3. `search_reference_col()` 查找最长白列作为参考列。
4. `New_Search_Line()` 搜索左右边界和中线。
5. `Cross_Detect()` 检测十字。
6. `Island_Detect_new()` 检测圆环并补线。
7. `Err_Sum()` 生成循迹误差。

圆环逻辑使用左右丢线、边界连续性、单调性突变点和 IMU 角度变化共同判断状态。

## 构建方式

工程保留了两套常见嵌入式 IDE 配置：

- MDK 工程：`znxj111/mdk/rt1064.uvprojx`
- IAR 工程：`znxj111/iar/rt1064.eww`

建议使用逐飞 RT1064 库匹配的 MDK 或 IAR 版本打开工程。移动工程目录后，先执行 IDE 的 clean，再重新编译。

## 调试建议

- 上电后保持小车静止，等待 IMU 零漂校准完成。
- 若循迹不稳，优先观察 `Err_Sum()`、`reference_col`、左右丢线次数和 `Island_State`。
- 若定位不稳，观察 `MCX_uart_rx_buff`、`filtered_box_x`、`filtered_box_y`。
- 若旋转角度不准，重点检查 `T_M`、`T_M_case_1`、`Angle_w_recond`。
- 若电机响应发冲，检查三路 `Speed_pwm_*` 累加量与 PWM 限幅。

## 代码注意事项

- 串口视觉数据在中断中更新，控制代码中读取时要注意半包数据风险。
- 当前 `Speed_pwm_left/right/behind` 为全局累加式输出，调参时需要关注积分饱和。
- `T_M` 航向角在超过正负 360 度后会清零，角度跨界场景要谨慎处理。
- `my_rate` 用于触发任务状态切换，来源为 MCX 视觉框尺寸计算。

## 许可证

底层库来自逐飞 RT1064DVL6A 开源库，相关许可证文件保留在 `libraries/` 与 `libraries/doc/` 中。用户业务代码请按实际项目要求补充授权说明。
