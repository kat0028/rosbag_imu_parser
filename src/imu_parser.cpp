#include <iostream>
#include <sensor_msgs/Imu.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <string>
#include <cmath>
#include <fstream>

int main()
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Configuration Variables -------------------------------------------------------------------
    std::string path = "/home/kyle/Data/rosbag/";
    std::string bag_filename = path + "june_15_hg1700_long_duration_2023-06-15-16-58-48.bag";
    std::string csv_filename = path + "data.csv";
    // -------------------------------------------------------------------------------------------

    rosbag::Bag bag;

    bag.open(bag_filename);

    std::vector<std::string> topics;

    // Change /imu to the imu topic from the robag------------------------------------------------
    topics.push_back("/imu");
    // -------------------------------------------------------------------------------------------

    rosbag::View view(bag, rosbag::TopicQuery(topics));

    sensor_msgs::Imu::ConstPtr msg;

    double header_stamp_sec;
    double header_stamp_nsec;
    double time;
    double angular_velocity_x;
    double angular_velocity_y;
    double angular_velocity_z;
    double linear_acceleration_x;
    double linear_acceleration_y;
    double linear_acceleration_z;

    std::ofstream csv_file;
    csv_file.open(csv_filename, std::ofstream::out | std::ofstream::trunc);
    std::string output_line;

    output_line = "time,gx,gy,gz,ax,ay,az\n";
    csv_file << output_line;

    for (rosbag::MessageInstance const m : rosbag::View(bag))
    {
        msg = m.instantiate<sensor_msgs::Imu>();
        if (msg != NULL)
        {
            header_stamp_sec = msg->header.stamp.sec;
            header_stamp_nsec = msg->header.stamp.nsec;
            time = header_stamp_sec + header_stamp_nsec * pow(10, -9);
            angular_velocity_x = msg->angular_velocity.x;
            angular_velocity_y = msg->angular_velocity.y;
            angular_velocity_z = msg->angular_velocity.z;
            linear_acceleration_x = msg->linear_acceleration.x;
            linear_acceleration_y = msg->linear_acceleration.y;
            linear_acceleration_z = msg->linear_acceleration.z;

            output_line = std::to_string(time) + ',' + std::to_string(angular_velocity_x) + ',' + std::to_string(angular_velocity_y) + ',' + std::to_string(angular_velocity_z) + ',' + std::to_string(linear_acceleration_x) + ',' + std::to_string(linear_acceleration_y) + ',' + std::to_string(linear_acceleration_z) + '\n';
            csv_file << output_line;
        }
    }

    csv_file.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Run time: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " seconds" << std::endl;
    return 0;
}