/*
*
* Project Name:   Visual perception for the visually impaired
* Author List:    Pankaj Baranwal, Ridhwan Luthra, Shreyas Sachan, Shashwat Yashaswi
* Filename:     cluster_distances.cpp
* Functions:    get_distance, cloud_cb, main
* Global Variables: pub, arr_pub, j
*
*/
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl_ros/point_cloud.h>
#include <boost/foreach.hpp>
#include <stdio.h>
#include <limits>
#include <vector>
#include <math.h>
#include <iostream>


#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

#include "std_msgs/Float64MultiArray.h"

using namespace::std;

ros::Publisher pub, arr_pub, voxel_pub;

int j = 0;

int maxDistance = 2;

int minClusterSize = 100, maxClusterSize = 600, maxIterations = 100;
double leaf_size = 0.03, distanceThreshold = 0.1, clusterTolerance = 0.1;

void get_distance(const pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr& msg){
/*
*
* Function Name:  get_distance
* Input:    msg -> stores point cloud information to be processed
            counter -> maintains the frame in consideration, necessary to handle clusters of same frame. 
* Output:    Publishes the minimum distance to ROS publisher
* Logic:    Iterates through all points in the filtered point cloud and publishes the min distance,
*           its angles, its leftmost points distance and angles, its rightmost points distance and angles
* Example Call:  get_distance (cloud, j)
*
*/
  double rad_to_deg = 57.2958;
  rad_to_deg = 1;

  float center_threshold = 0.26;

  double distances[3] = {std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
  double y_angles[3] = {std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
  // int i = 0;
  // Angles are calculated in radians and can convert to degree by multpying it with 180/pi 
  //to iterate trough all the points in the filtered point cloud published by publisher
  BOOST_FOREACH (const pcl::PointXYZRGB& pt, msg->points){
    if(atan2(pt.z, pt.y) < 1.5) {
      continue;
    }
    double angle = atan2(pt.x, pt.z);
    // std::cout<<i++<<endl;
    if(angle > center_threshold){
      // keep updating the minimum Distant point
      if (hypot(pt.z, pt.x) < distances[0]) {
        distances[0] = hypot(pt.z, pt.x);
        y_angles[0] = atan2(pt.z, pt.y);
      }
    }
    // Angles are not accurately measured. SO, subtract 0.25 m from each distance.
    if(angle > -center_threshold && angle < center_threshold){
      // keep updating the minimum angle
      if (hypot(pt.z, pt.x) < distances[1]) {
        distances[1] = hypot(pt.z, pt.x);
        y_angles[1] = atan2(pt.z, pt.y);
      }
    }
    
    if(angle < -center_threshold){
      // keep updating the maximum angle
      if (hypot(pt.z, pt.x) < distances[2]) {
        distances[2] = hypot(pt.z, pt.x);
        y_angles[2] = atan2(pt.z, pt.y);
      }
    }
  }
  std_msgs::Float64MultiArray arr;

  arr.data.clear();
  arr.data.push_back(distances[0]);
  arr.data.push_back(distances[1]);
  arr.data.push_back(distances[2]);
  // arr.data.push_back(y_angles[0]);
  // arr.data.push_back(y_angles[1]);
  // arr.data.push_back(y_angles[2]);
  
  arr_pub.publish(arr);
}

void 
cloud_cb (const sensor_msgs::PointCloud2ConstPtr& input)
{
	// Leaf size == Distance between 2 pts in the point cloud.
	// maxClusterSize == Max number of points that are allowed to cluster together.
	// minClusterSize == Min number of points that should be there to form a cluster


  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_f (new pcl::PointCloud<pcl::PointXYZRGB>);
  
  pcl::PCLPointCloud2* cloud_blob = new pcl::PCLPointCloud2; 
  pcl::PCLPointCloud2ConstPtr cloudPtr(cloud_blob);
  pcl::PCLPointCloud2 cloud_filtered_blob;

  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_filtered_planar (new pcl::PointCloud<pcl::PointXYZRGB>);

  pcl_conversions::toPCL(*input, *cloud_blob);  
  
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZRGB>);
  // Perform the actual filtering
  pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
  sor.setInputCloud (cloudPtr);
  sor.setLeafSize (leaf_size, leaf_size, leaf_size);
  sor.filter (cloud_filtered_blob);

  voxel_pub.publish(cloud_filtered_blob);

  pcl::fromPCLPointCloud2 (cloud_filtered_blob, *cloud_filtered);

  // Create the segmentation object for the planar model and set all the parameters
  pcl::SACSegmentation<pcl::PointXYZRGB> seg;
  pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
  pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_plane (new pcl::PointCloud<pcl::PointXYZRGB> ());
  pcl::PCDWriter writer;
  seg.setOptimizeCoefficients (true);
  seg.setModelType (pcl::SACMODEL_PLANE);
  seg.setMethodType (pcl::SAC_RANSAC);
  seg.setMaxIterations (maxIterations);
  seg.setDistanceThreshold (distanceThreshold);

  int i=0, nr_points = (int) cloud_filtered->points.size ();
  while (cloud_filtered->points.size () > 0.3 * nr_points)
  {
    // Segment the largest planar component from the remaining cloud
    seg.setInputCloud (cloud_filtered);
    seg.segment (*inliers, *coefficients);
    if (inliers->indices.size () == 0)
    {
      std::cout << "Could not estimate a planar model for the given dataset." << std::endl;
      break;
    }

    // Extract the planar inliers from the input cloud
    pcl::ExtractIndices<pcl::PointXYZRGB> extract;
    extract.setInputCloud (cloud_filtered);
    extract.setIndices (inliers);
    // Remove the planar inliers, extract the rest
    extract.setNegative (true);
    extract.filter (*cloud_f);
    *cloud_filtered = *cloud_f;
  }

  // Creating the KdTree object for the search method of the extraction
  pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZRGB>);
  tree->setInputCloud (cloud_filtered);

  std::vector<pcl::PointIndices> cluster_indices;
  pcl::EuclideanClusterExtraction<pcl::PointXYZRGB> ec;
  ec.setClusterTolerance (clusterTolerance);
  ec.setMinClusterSize (minClusterSize);
  ec.setMaxClusterSize (maxClusterSize);
  ec.setSearchMethod (tree);
  ec.setInputCloud (cloud_filtered);
  ec.extract (cluster_indices);

  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_clust_remove (new pcl::PointCloud<pcl::PointXYZRGB>);
  pcl::PCLPointCloud2 cloud_final;
  pcl::PCLPointCloud2 temp;
  *cloud_clust_remove = *cloud_filtered;
  pcl::PointCloud<pcl::PointXYZRGB> cloud_xyzrgb;
  std::cout<<"-------NEW FRAME------"<<std::endl<<std::endl;
  for (int it = 0; it < cluster_indices.size(); ++it)
  {

    // Extract the planar inliers from the input cloud
    pcl::ExtractIndices<pcl::PointXYZRGB> ext;
    ext.setInputCloud (cloud_clust_remove);
    ext.setIndices(boost::shared_ptr<pcl::PointIndices> (new pcl::PointIndices(cluster_indices[it])));
    // to extract just the cluster
    ext.setNegative (false);
    ext.filter (*cloud_f);
    // to ignore far away clusters for brevity.
    cloud_xyzrgb = *cloud_f;

    // iteratively colors the cluster red, green or blue.
    for (size_t i = 0; i < cloud_xyzrgb.points.size(); i++) {
      if (it % 3 == 0) {
        cloud_xyzrgb.points[i].r = 255;
      }
      else if (it % 3 == 1) {
        cloud_xyzrgb.points[i].g = 255;
      }
      else if (it % 3 == 2) {
        cloud_xyzrgb.points[i].b = 255;  
      }
    }

    pcl::toPCLPointCloud2 (cloud_xyzrgb, temp);
    pcl::concatenatePointCloud(cloud_final, temp, cloud_final);
  }
  j++;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_for_dist (new pcl::PointCloud<pcl::PointXYZRGB>);
  pcl::fromPCLPointCloud2 (cloud_final, *cloud_for_dist);
  get_distance(cloud_for_dist);
  pub.publish (cloud_final);
}

int
main (int argc, char** argv)
{
  // Initialize ROS
  ros::init (argc, argv, "cluster_dist");
  ros::NodeHandle nh;
  
  ros::NodeHandle private_node_handle_("~");
  /*
	Example:
	rosrun obstacle_mapping cluster_distances _clusterTolerance:=0.03 _minClusterSize:=150 _maxClusterSize:=250 _distanceThreshold:=0.02
  */
  private_node_handle_.param("minClusterSize", minClusterSize, int(5));
  private_node_handle_.param("leaf_size", leaf_size, double(5));
  private_node_handle_.param("maxClusterSize", maxClusterSize, int(5));
  // private_node_handle_.param("maxIterations", maxIterations, int(5));
  private_node_handle_.param("distanceThreshold", distanceThreshold, double(5));
  private_node_handle_.param("clusterTolerance", clusterTolerance, double(5));
  // private_node_handle_.param("minClusterSize", minClusterSize, int(5));
  cout << clusterTolerance << endl;


  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("/camera/depth_registered/points", 1, cloud_cb);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("output", 1);

  voxel_pub = nh.advertise<sensor_msgs::PointCloud2> ("voxeled", 1);

  // publishing  details
  arr_pub = nh.advertise<std_msgs::Float64MultiArray> ("cluster_distances", 1);
  // pub = 

  // Spin
  ros::spin ();
}
