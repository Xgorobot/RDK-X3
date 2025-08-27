import os
import sys
import subprocess
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from PIL import Image, ImageDraw, ImageFont
import xgoscreen.LCD_2inch as LCD_2inch

from pathlib import Path
try:
    from ament_index_python.packages import get_package_share_directory
except Exception:
    get_package_share_directory = None

class DoubaoASRNode(Node):
    def __init__(self):
        super().__init__('doubao_asr_node')
        # params
        self.declare_parameter('arecord_device', 'hw:0,1')
        self.declare_parameter('arecord_rate', 48000)
        self.declare_parameter('arecord_channels', 2)
        self.declare_parameter('arecord_duration', 5)
        self.declare_parameter('audio_file', 'test.wav')
        self.declare_parameter('enable_wake', True)

        self.pub_ = self.create_publisher(String, 'doubao_asr/text', 10)

        # LCD init
        self.display = LCD_2inch.LCD_2inch()
        self.display.Init()
        self.splash_color = (15,21,46)
        self.splash = Image.new("RGB", (self.display.height, self.display.width), self.splash_color)
        self.draw = ImageDraw.Draw(self.splash)
        try:
            self.font = ImageFont.truetype("/home/pi/model/msyh.ttc", 20)
        except Exception:
            self.font = ImageFont.load_default()
        self.display.ShowImage(self.splash)

        # images directory from package share if available
        if get_package_share_directory is not None:
            try:
                share_dir = Path(get_package_share_directory('doubao_asr'))
            except Exception:
                share_dir = Path(__file__).resolve().parent.parent
        else:
            share_dir = Path(__file__).resolve().parent.parent
        self.image_dir = share_dir / 'resource' / 'image'

        self.timer = self.create_timer(0.1, self.loop)
        self.busy = False

    def show_image_if_exists(self, names):
        for name in names:
            p = self.image_dir / name
            if p.exists():
                try:
                    img = Image.open(str(p)).convert('RGB').resize((self.display.height, self.display.width))
                    self.display.ShowImage(img)
                    return True
                except Exception as e:
                    self.get_logger().warn(f'Failed to load image {p}: {e}')
        return False

    def show_text(self, text: str):
        self.draw.rectangle((0,0,self.display.height,self.display.width), fill=self.splash_color)
        # mimic wake_word.py visual block
        gray = (128,128,128)
        rx = (self.display.width - 120) // 2
        ry = 110
        rw, rh = 200, 80
        self.draw.rectangle((rx, ry, rx+rw, ry+rh), fill=gray)
        # wrap lines
        y = 115
        line_h = 22
        max_width = 190
        x = 70
        for line in self.wrap_text(text, max_width):
            self.draw.text((x,y), line, fill=(255,255,255), font=self.font)
            y += line_h
        self.display.ShowImage(self.splash)

    def wrap_text(self, text, max_width):
        out = []
        cur = ''
        for ch in text:
            test = cur + ch
            if self.font.getlength(test) <= max_width:
                cur = test
            else:
                if cur:
                    out.append(cur)
                cur = ch
        if cur:
            out.append(cur)
        return out[:5]

    def ensure_workspace_on_path(self):
        # add workspace root to sys.path so we can import wake_word and language_recognize
        ws_root = Path(__file__).resolve().parents[3]
        if str(ws_root) not in sys.path:
            sys.path.append(str(ws_root))
        return ws_root

    def wait_for_wake(self):
        try:
            self.ensure_workspace_on_path()
            from wake_word import is_wake
            self.get_logger().info('Waiting for wake word...')
            self.show_image_if_exists(['listening.png', 'listening.jpg'])
            ok = is_wake()
            return ok
        except Exception as e:
            self.get_logger().warn(f'Wake word unavailable or failed: {e}, proceeding without wake.')
            return True

    def loop(self):
        if self.busy:
            return
        self.busy = True
        try:
            # optional wake gating
            if self.get_parameter('enable_wake').get_parameter_value().bool_value:
                if not self.wait_for_wake():
                    self.busy = False
                    return

            audio_file = self.get_parameter('audio_file').get_parameter_value().string_value
            device = self.get_parameter('arecord_device').get_parameter_value().string_value
            rate = self.get_parameter('arecord_rate').get_parameter_value().integer_value
            channels = self.get_parameter('arecord_channels').get_parameter_value().integer_value
            duration = self.get_parameter('arecord_duration').get_parameter_value().integer_value

            # arecord exactly as required
            cmd = ['arecord','-D', device, '-f','S16_LE','-r', str(rate), '-c', str(channels), '-d', str(duration), audio_file]
            self.get_logger().info('Recording audio: ' + ' '.join(cmd))
            self.show_image_if_exists(['recording.png', 'recording.jpg'])
            try:
                subprocess.run(cmd, check=True)
            except subprocess.CalledProcessError as e:
                self.get_logger().error(f'arecord failed: {e}')
                self.busy = False
                return

            # call existing recognizer (language_recognize.py) which prints text
            ws_root = self.ensure_workspace_on_path()
            script = str(ws_root / 'language_recognize.py')
            if not os.path.exists(script):
                self.get_logger().error('language_recognize.py not found')
                self.busy = False
                return
            # ensure the recognizer reads our recorded file
            # language_recognize.py uses audio_path = "recorded_audio.wav"; copy
            if audio_file != 'recorded_audio.wav':
                try:
                    import shutil
                    shutil.copyfile(audio_file, str(ws_root / 'recorded_audio.wav'))
                except Exception as e:
                    self.get_logger().warn(f'copy audio failed: {e}')

            result = subprocess.run(['python3', script], capture_output=True, text=True)
            text = result.stdout.strip()
            if not text:
                self.get_logger().warn('No text recognized')
                self.busy = False
                return

            self.show_text(text)
            msg = String()
            msg.data = text
            self.pub_.publish(msg)
            self.get_logger().info('Published: ' + text)
        finally:
            self.busy = False


def main():
    rclpy.init()
    node = DoubaoASRNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()