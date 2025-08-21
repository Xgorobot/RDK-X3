import sys
import time
import os
from PIL import Image
from subprocess import Popen

from xgolib import XGO
from xgo_motion.display_utils import Display

def main():
    # 初始化显示和运动控制
    display = Display()
    dog = XGO("xgolite")

    # 播放背景音乐（循环）
    music_path = os.path.join(os.path.dirname(__file__), 'music', 'Dream.mp3')
    if not os.path.exists(music_path):
        print(f"[错误] 音乐文件未找到: {music_path}")
        return
    proc = Popen(f"mplayer '{music_path}' -loop 0", shell=True)

    # 初始化狗狗状态
    dog.reset()
    dog.perform(1)

    # 表情资源路径
    pic_path = os.path.join(os.path.dirname(__file__), 'expression')

    def show(expression_name_cs, pic_num):
        for i in range(pic_num):
            img_path = os.path.join(pic_path, expression_name_cs, f"{i + 1}.png")
            if os.path.exists(img_path):
                exp = Image.open(img_path)
                display.show_image(exp)
                time.sleep(0.1)
            else:
                print(f"[警告] 缺失图像：{img_path}")

    # 播放循环动画
    try:
        while True:
            show("sad", 14)
            show("naughty", 14)
            show("boring", 14)
            show("angry", 13)
            show("shame", 11)
            show("surprise", 15)
            show("happy", 12)
            show("sleepy", 19)
            show("seek", 12)
            show("lookaround", 12)
            show("love", 13)
            show("awkwardness", 11)
            show("eyes", 15)
            show("guffaw", 8)
            show("query", 7)
            show("Shakehead", 7)
            show("Stun", 8)
            show("wronged", 14)
    except KeyboardInterrupt:
        print("中断退出...")
    # 关闭
    dog.perform(0)
    proc.kill()

if __name__ == '__main__':
    main()
