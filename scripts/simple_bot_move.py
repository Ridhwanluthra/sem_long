#!/usr/bin/env python

'''
*
* project name:     visual perception for visually impaired 
* author list:      Pankaj Baranwal, Ridhwan Luthra, Shreyas Sachan, Shashwat Yashaswi
* filename:         listener.py
* functions:        callback, listener
* global variables: curr_frame, data_per_frame, check
*
'''
# import to_firebird as tf
import rospy
from std_msgs.msg import Float64MultiArray, MultiArrayLayout, MultiArrayDimension
import requests
import math

move = [False, False, False]
'''
*
* Function Name:    callback
* Input:        data -> data about the point cloud
* Output:       
* Logic:        
* Example Call:  callback function, manual calling not required.
*
'''
def callback(data):
    global move
    dat = list(data.data)
    
    for i in range(3):
        if dat[i] > 100:
            dat[i] = 0
        if dat[i] < 1:
            move[i] = False
        else:
            move[i] = True
    try:
        # tf.reset()
        if not move[0] and not move[1] and not move[2]:
            index = 4
        elif not move[0] and not move[1] and move[2]:
            index = 3
        elif not move[0] and move[1] and not move[2]:
            index = 1
        elif not move[0] and move[1] and move[2]:
            index = 3
        elif move[0] and not move[1] and not move[2]:
            index = 1
        elif move[0] and not move[1] and move[2]:
            index = 1
        elif move[0] and move[1] and not move[2]:
            index = 1
        elif move[0] and move[1] and move[2]:
            index = 2
        # tf.move_to_cell(index)
        print(index)
    finally:
        pass
        # tf.reset()

'''
*
* Function Name:    listener
* Input:        NIL
* Output:       NIL
* Logic:    Initializes node and makes sure that two nodes don't have the same name, 
*           also subscribes to the topic and calls the callback function.
* Example Call:  listener()
*
'''
def listener():
    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('simple_bot_move', anonymous=True)
    rospy.Subscriber("simple_distances", Float64MultiArray, callback)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

# runs the listener function if the file is run as a script
if __name__ == '__main__':
    listener()
    # try:
    #     listener()
    # except KeyboardInterrupt:
    #     tf.reset()
    #     tf.gpio.cleanup()
    # finally:
    #     tf.reset()
    #     tf.gpio.cleanup()
