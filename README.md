# Real-Time-Systems

This repository consists of the projects performed under the course unit “Real Time Systems”. It presents two different projects. The first project named “Scheduling, Priorities Assignment, and Measurement of Computation Times” and the second one named “Mutual Exclusion, Process Syncronization, Measurement of Computation Times and ROS “.

## Scheduling, Priorities Assignment, and Measurement of Computation Times

This project consists of the implementation of an application that makes use of POSIX routines, with the goal of studying the scheduling by priorities assignment and the measurement of the computation times. This project uses three functions, f1, f2, and f3 that are assigned to different periodic tasks. These functions are defined in the files func.c and func.h. The rest of the files have the following objectives:

1-	Measure the maximum computation time of each function using the POSIX functionalities.

2-	With the obtained results from the previous step, the second step consists of verifying if the system is schedulable for two priority orderings: RMPO (Rate Monotonic Priority Ordering) and its inverse.

3-	Implementation of an application that, during 6 seconds maintains the three tasks jointly running using RMPO. Each task (regarding functions 1 to 3) should be periodically activated with periods of 0.1, 0.2, and 0.3 seconds, respectively. 

4-	In this step, the code implemented in step 3 was further developed in order to change the priorities between RMPO and the inverse, during specific times.

5-	Development of the code func2.c and func2.c that imitates the functioning of the module func.c. This new code is tested using the application developed.

6-	In this step, the different tasks have equal priorities and is tested the application developed using Round and Robin scheduling method.

## Mutual Exclusion, Process Syncronization, Measurement of Computation Times and ROS:

This project consists of solving an engineering problem related to environment sensor-based perception, which finds applications in autonomous driving and mobile robotics navigation. Specifically, a ground/road detection system will be developed having as input data from a 3D LiDAR sensor. This project involves POSIX routines and has the objective to guarantee that multiple threads perform sequential tasks (and share common global memory) without performing read/ write data concurrently in the same variable (memory location). The rest of the files have the following objectives:

1-	This step consists of reading the values to be passed to a struct variable and perform some calculations, including the number of points, the minimum, the maximum, the average and standard deviation.

2-	In this step, a function is implemented, that receives as input a pointer to the struct variable created and makes a pre-processing stage to reduce the number of points according to three stages: a) removes all the points that are located in the back part of the car with respect to the sensor. b) detect and remove two groups of points that are located very close to the car. c)  remove the points that do not correspond to the ground/road.

3-	Development of a new function to identify the points that belong to the drivable area with respect to the car, i.e., LIDAR points that belong to the ground road, discarding the rest of the points.

4-	Calculate the time of computation of each function developed, guaranteeing that the total computation time is less than 95 ms.

5-	Development of a program that executes each function in three separate threads. Each function accesses the data at the same time, in order to avoid they read/write data concurrently in the same memory location associated with the struct variable. To perform this, synchronization primitives are used; in this case, mutex is used.
