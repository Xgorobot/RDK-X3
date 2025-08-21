#!/usr/bin/env python3


# import rclpy
# from rclpy import Node
from xgolib import XGO

dog = XGO('ttyS3')

version = dog.read_firmware()

if version[0] == 'M':
    print('XGO-MINI')
else:
    print('XGO-LITE')