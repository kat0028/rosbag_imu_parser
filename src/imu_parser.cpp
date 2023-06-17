#include <iostream>
#include "TinyMAT/tinymatwriter.h"
#include <sensor_msgs/Imu.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <string>

int main()
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Configuration Variables -------------------------------------------------------------------
    std::string path = "/home/kyle/Data/rosbag/";
    std::string bag_filename = path + "nick_hg1700ag75_syncIntern_2023-04-20-16-15-24.bag";
    std::string mat_name = path + "data.mat";
    // -------------------------------------------------------------------------------------------

    const char * mat_filename = mat_name.c_str();

    rosbag::Bag bag;

    bag.open(bag_filename);

    std::vector<std::string> topics;

    // Change /imu to the imu topic from the robag------------------------------------------------
    topics.push_back("/imu");
    // -------------------------------------------------------------------------------------------

    rosbag::View view(bag, rosbag::TopicQuery(topics));

    sensor_msgs::Imu::ConstPtr msg;

    std::vector<double> header_seq;
    std::vector<double> header_stamp_sec;
    std::vector<double> header_stamp_nsec;
    std::vector<double> orientation_x;
    std::vector<double> orientation_y;
    std::vector<double> orientation_z;
    std::vector<double> orientation_w;
    std::vector<double> orientation_covariance;
    std::vector<double> angular_velocity_x;
    std::vector<double> angular_velocity_y;
    std::vector<double> angular_velocity_z;
    std::vector<double> angular_velocity_covariance;
    std::vector<double> linear_acceleration_x;
    std::vector<double> linear_acceleration_y;
    std::vector<double> linear_acceleration_z;
    std::vector<double> linear_acceleration_covariance;


    for(rosbag::MessageInstance const m: rosbag::View(bag))
    {
        msg  = m.instantiate<sensor_msgs::Imu>();

        header_seq.push_back(msg->header.seq);
        header_stamp_sec.push_back(msg->header.stamp.sec);
        header_stamp_nsec.push_back(msg->header.stamp.nsec);

        orientation_x.push_back(msg->orientation.x);
        orientation_y.push_back(msg->orientation.y);
        orientation_z.push_back(msg->orientation.z);
        orientation_w.push_back(msg->orientation.w);

        angular_velocity_x.push_back(msg->angular_velocity.x);
        angular_velocity_y.push_back(msg->angular_velocity.y);
        angular_velocity_z.push_back(msg->angular_velocity.z);

        linear_acceleration_x.push_back(msg->linear_acceleration.x);
        linear_acceleration_y.push_back(msg->linear_acceleration.y);
        linear_acceleration_z.push_back(msg->linear_acceleration.z);

        for (int i = 0; i < orientation_covariance.size(); i++)
        {
            orientation_covariance.push_back(msg->orientation_covariance.at(i));
            angular_velocity_covariance.push_back(msg->angular_velocity_covariance.at(i));
            linear_acceleration_covariance.push_back(msg->angular_velocity_covariance.at(i));
        }
    }

    TinyMATWriterFile *mat_file = TinyMATWriter_open(mat_filename);
    TinyMATWriter_startStruct(mat_file, "imu");

    TinyMATWriter_writeDoubleVector(mat_file, "header_seq", header_seq, false);
    TinyMATWriter_writeDoubleVector(mat_file, "header_stamp_sec", header_stamp_sec, false);
    TinyMATWriter_writeDoubleVector(mat_file, "header_stamp_nsec", header_stamp_nsec, false);
    TinyMATWriter_writeDoubleVector(mat_file, "orientation_x", orientation_x, false);
    TinyMATWriter_writeDoubleVector(mat_file, "orientation_y", orientation_y, false);
    TinyMATWriter_writeDoubleVector(mat_file, "orientation_z", orientation_z, false);
    TinyMATWriter_writeDoubleVector(mat_file, "orientation_w", orientation_w, false);
    TinyMATWriter_writeDoubleVector(mat_file, "orientation_covariance", orientation_covariance, false);
    TinyMATWriter_writeDoubleVector(mat_file, "angular_velocity_x", angular_velocity_x, false);
    TinyMATWriter_writeDoubleVector(mat_file, "angular_velocity_y", angular_velocity_y, false);
    TinyMATWriter_writeDoubleVector(mat_file, "angular_velocity_z", angular_velocity_z, false);
    TinyMATWriter_writeDoubleVector(mat_file, "angular_velocity_covariance", angular_velocity_covariance, false);
    TinyMATWriter_writeDoubleVector(mat_file, "linear_acceleration_x", linear_acceleration_x, false);
    TinyMATWriter_writeDoubleVector(mat_file, "linear_acceleration_y", linear_acceleration_y, false);
    TinyMATWriter_writeDoubleVector(mat_file, "linear_acceleration_z", linear_acceleration_z, false);
    TinyMATWriter_writeDoubleVector(mat_file, "linear_acceleration_covariance", linear_acceleration_covariance, false);

    TinyMATWriter_endStruct(mat_file);
    TinyMATWriter_close(mat_file);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Run time: " << std::chrono::duration_cast<std::chrono::seconds>(end-begin).count() << " seconds" << std::endl;

    return 0;
}