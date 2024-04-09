#pragma once

class TimeManager
{
public:
    static double get_delta_time();
    static double get_camera_lerp();
    static double get_last_frame();
    static double get_current_frame();
    static double get_door_lerp();
    static double get_active_time();
    static void set_camera_lerp(double lerp);
    static void set_delta_time(double time);
    static void set_last_frame(double time);
    static void set_current_frame(double time);
    static void set_door_lerp(double lerp);
};
