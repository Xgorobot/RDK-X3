#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from PIL import Image, ImageDraw, ImageFont
import socket
import os
import time

from xgolib import XGO
from xgo_device.display_utils import Display

FONT_PATH = "/home/sunrise/xgo_demo/src/xgo_device/xgo_device/font/msyh.ttf"
VERSION = "1.0"

class DeviceInfoNode(Node):
    def __init__(self):
        super().__init__('device_info_node')

        # 初始化机器人控制与显示
        self.robot = XGO('x3')  
        self.display = Display()

        # 加载字体
        try:
            self.font = ImageFont.truetype(FONT_PATH, 24)
        except Exception as e:
            self.get_logger().error(f"字体加载失败: {e}")
            self.font = ImageFont.load_default()

        self.timer = self.create_timer(3.0, self.update_display)  # 每3秒刷新
        self.get_logger().info("设备信息显示节点已启动")

    def get_ip_address(self):
        try:
            # 获取当前默认出口 IP（不等于联网，只是系统网关）
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect(("8.8.8.8", 80))
            ip = s.getsockname()[0]
            s.close()

            # 检查是否真的连通外网（非仅连接路由器或虚拟网卡）
            if os.system("ping -c 1 -W 1 8.8.8.8 > /dev/null 2>&1") == 0:
                return ip
            else:
                return "127.0.0.1 (未连接外网)"
        except Exception:
            return "127.0.0.1 (获取失败)"


    def update_display(self):
        battery = self.robot.read_battery()
        ip_addr = self.get_ip_address()

        # 创建图片
        image = Image.new("RGB", (320, 240), (0, 0, 0))
        draw = ImageDraw.Draw(image)

        draw.text((10, 40), f"版本: {VERSION}", font=self.font, fill=(255, 255, 255))
        draw.text((10, 90), f"电量: {battery}%", font=self.font, fill=(0, 255, 0))
        draw.text((10, 140), f"IP: {ip_addr}", font=self.font, fill=(0, 200, 255))

        self.display.show_image(image)

def main(args=None):
    rclpy.init(args=args)
    node = DeviceInfoNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.display.clear()
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
