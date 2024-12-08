# scan_reflector_filter
## 레이저 반사도를 고려한 라이다 SLAM 기반 실내 지도작성 소프트웨어
===========================================================================


## Scan filter
> * This filter is made to filter out a reflective object (e.g. mirrors, glasses) in LiDAR measurements.
> * This software is based on Velodyne VLP-16 LiDAR


## Example
![Alt text](docs/example.jpg)


## Requirements
> * Based on Ubuntu 20.04 and ROS Noetic (ROS1), Ubuntu 22.04 and ROS Humble (ROS2)
> * 
> * Require Database for intensities of reflectors of the user's own environments ( In my cases, 15~20 is threshold )



## How to use    
    
> * First, set(change) subscribe topic name and threshold
    
    cd ~/<user's workspace>/src
    git clone https://github.com/ROUN-LEE-KOR/LiDAR-based-Mapping-Considering-Laser-Reflectivity-in-Indoor-Environments.git
    
    
>    > *Open .cpp file in src (scan_filter.cpp)
>    > 
>    > *Check subscribe topic name and change to your lidar topic
>    > 
>    > *also check filtering function and change criteria based on your own database


> * Next, compile the source
    (ROS2)
    cd .. (Move to <user's workspace>)
    colcon build --symlink-install --packages-select scan_filter
    source install/local_setup.bash
>   *
>   (ROS1)
>   catkin_make
>   source devel/setup.bash

    
    
> * Then, run the code
    rosrun scan_filter scan_filter_node
    ros2 run scan_filter scan_filter_node


## Reference
> 이로운, 박정홍 and 홍성훈. "실내 환경에서의 레이저 반사도를 고려한 라이다 기반 지도 작성" 로봇학회 논문지 18, no.2 (2023) : 135-142.doi: 10.7746/jkros.2023.18.2.135
    
    
## Contact

    rounlee.kor@gmail.com
    

