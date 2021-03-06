#ifndef _SRL_LASER_DETECTORS_ROS_INTERFACE_H
#define _SRL_LASER_DETECTORS_ROS_INTERFACE_H

#include <boost/shared_ptr.hpp>
#include <ros/ros.h>

#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>

#include <sensor_msgs/LaserScan.h>
#include <srl_laser_segmentation/LaserscanSegmentation.h>
#include <spencer_tracking_msgs/DetectedPersons.h>
#include <spencer_diagnostics/publisher.h>

#include <srl_laser_detectors/detector.h>


namespace srl_laser_detectors {

/// Connects a Detector to ROS via topics.
class ROSInterface {
public:
    /// Constructor
    ROSInterface(ros::NodeHandle& nodeHandle, ros::NodeHandle& privateNodeHandle);

    /// Connect the detector to ROS, this will subscribe to a sensor_msgs/LaserScan topic and a srl_laser_segmentation/LaserSegmentation topic
    /// and output spencer_tracking_msgs/DetectedPersons.
    /// Remember to call ros::spin() after invoking this method.
    void connect(Detector* detector, const std::string& laserTopic = "laser", const std::string& segmentationTopic = "laser_segmentation", const std::string& detectedPersonsTopic = "detected_persons");

private:
    void newLaserscanAndSegmentationAvailable(const sensor_msgs::LaserScan::ConstPtr& laserscan, const srl_laser_segmentation::LaserscanSegmentation::ConstPtr& segmentation);    

    /// ROS handles
    ros::NodeHandle m_nodeHandle, m_privateNodeHandle;
    spencer_diagnostics::MonitoredPublisher m_detectedPersonsPublisher;

    /// For synchronization of input laserscans and segmentations
    boost::shared_ptr<message_filters::Subscriber<sensor_msgs::LaserScan> > m_laserscanSubscriber;
    boost::shared_ptr<message_filters::Subscriber<srl_laser_segmentation::LaserscanSegmentation> > m_segmentationSubscriber;
    
    typedef message_filters::sync_policies::ExactTime<sensor_msgs::LaserScan, srl_laser_segmentation::LaserscanSegmentation> SyncPolicy;
    typedef message_filters::Synchronizer<SyncPolicy> Synchronizer;
    boost::shared_ptr<Synchronizer> m_inputSynchronizer;

    /// Detector-related stuff
    unsigned int m_lastDetectionId;
    int m_detectionIdIncrement;
    Detector* m_detector;
};


} // end of namespace srl_laser_detectors

#endif // _SRL_LASER_DETECTORS_ROS_INTERFACE_H
