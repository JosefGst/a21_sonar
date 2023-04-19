#include "a21_sonar/a21_sonar.h"

// prototypes
void my_sleep(unsigned long milliseconds);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "Sonar");
    ros::NodeHandle nh;
    ros::Publisher sonar_pub = nh.advertise<sensor_msgs::Range>("range", 1000);
    ros::Rate loop_rate(10);

    // get params
    std::string port, frame_id;
    float min_range, max_range, field_of_view;
    int baud, radiation_type;

    // load parameters, inspired form: https://github.com/ros-planning/navigation_experimental/blob/noetic-devel/twist_recovery/src/twist_recovery.cpp
    // TODO: test it
    // bool found = true;
    // found = found && nh.getParam("/a21_sonar/port", port);
    // found = found && nh.getParam("/a21_sonar/baud", baud);
    // found = found && nh.getParam("/a21_sonar/frame_id", frame_id);
    // found = found && nh.getParam("/a21_sonar/radiation_type", radiation_type);
    // found = found && nh.getParam("/a21_sonar/field_of_view", field_of_view);
    // found = found && nh.getParam("/a21_sonar/min_range", min_range);
    // found = found && nh.getParam("/a21_sonar/max_range", max_range);
    // if (!found)
    // {
    //     ROS_FATAL_STREAM("Didn't find a21_sonar parameters in " << nh.getNamespace());
    //     ros::shutdown();
    // }

    if (ros::param::has("/a21_sonar/port"))
        ros::param::get("/a21_sonar/port", port);
    else
        nh.param<std::string>("/a21_sonar/port", port, "/dev/ttyUSB0");

    if (ros::param::has("/a21_sonar/baud"))
        ros::param::get("/a21_sonar/baud", baud);
    else
        nh.param<int>("/a21_sonar/baud", baud, 115200);

    if (ros::param::has("/a21_sonar/frame_id"))
        ros::param::get("/a21_sonar/frame_id", frame_id);
    else
        nh.param<std::string>("/a21_sonar/frame_id", frame_id, "sonar");

    if (ros::param::has("/a21_sonar/radiation_type"))
        ros::param::get("/a21_sonar/radiation_type", radiation_type);
    else
        nh.param<int>("/a21_sonar/radiation_type", radiation_type, 0);

    if (ros::param::has("/a21_sonar/field_of_view"))
        ros::param::get("/a21_sonar/field_of_view", field_of_view);
    else
        nh.param<float>("/a21_sonar/field_of_view", field_of_view, 0.15);

    if (ros::param::has("/a21_sonar/min_range"))
        ros::param::get("/a21_sonar/min_range", min_range);
    else
        nh.param<float>("/a21_sonar/min_range", min_range, 0.05);

    if (ros::param::has("/a21_sonar/max_range"))
        ros::param::get("/a21_sonar/max_range", max_range);
    else
        nh.param<float>("/a21_sonar/max_range", max_range, 2.0);

    A21 sonar;
    sonar.begin(port, baud);

    while (ros::ok())
    {
        // populate sonsor range data
        sonar.data.header.stamp = ros::Time::now();
        sonar.data.header.frame_id = frame_id;
        sonar.data.radiation_type = radiation_type;
        sonar.data.field_of_view = field_of_view;
        sonar.data.min_range = min_range;
        sonar.data.max_range = max_range;
        sonar.data.range = float(sonar.read_range()) / 1000;

        sonar_pub.publish(sonar.data);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

// helper functions ***********************************************

void my_sleep(unsigned long milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds); // 100 ms
#else
    usleep(milliseconds * 1000); // 100 ms
#endif
}