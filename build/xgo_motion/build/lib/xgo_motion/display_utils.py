from PIL import Image
from luma.core.interface.serial import spi
from luma.lcd.device import st7789

class Display:
    """
    显示工具类，用于通过 SPI 屏幕显示 PIL 图像。

    硬件连接假设：
      - SPI 设备: /dev/spidev1.0
      - MOSI -> GPIO10
      - SCLK -> GPIO11
      - CS   -> GPIO8
      - DC   -> GPIO25
      - RST  -> GPIO27
      - BL   -> 3.3V 背光
    """

    def __init__(self, width=320, height=240, rotate=0, bus_speed_hz=40000000):
        # 初始化 SPI 接口和 LCD 设备
        self.serial = spi(port=1,
                          device=0,
                          gpio_DC=25,
                          gpio_RST=27,
                          gpio_CS=8,
                          bus_speed_hz=bus_speed_hz)
        self.device = st7789(self.serial,
                             width=width,
                             height=height,
                             rotate=rotate)

    def resize_image_keep_aspect(self, image: Image.Image, max_width: int, max_height: int) -> Image.Image:
        """
        等比例缩放图片，最大尺寸不超过 max_width x max_height。
        兼容 Pillow 不同版本的重采样参数。
        """
        try:
            resample = Image.Resampling.LANCZOS  # Pillow >= 10
        except AttributeError:
            resample = Image.ANTIALIAS  # Pillow < 10

        img_ratio = image.width / image.height
        screen_ratio = max_width / max_height

        if img_ratio > screen_ratio:
            # 图片较宽，按宽度缩放
            new_width = max_width
            new_height = int(max_width / img_ratio)
        else:
            # 图片较高，按高度缩放
            new_height = max_height
            new_width = int(max_height * img_ratio)

        return image.resize((new_width, new_height), resample)

    def show_image(self, image: Image.Image):
        """
        显示图片，自动等比缩放并居中。
        """
        img_rgb = image.convert("RGB")
        resized_img = self.resize_image_keep_aspect(img_rgb, self.device.width, self.device.height)

        # 创建黑色背景画布
        background = Image.new("RGB", (self.device.width, self.device.height), (0, 0, 0))

        # 计算居中偏移
        x = (self.device.width - resized_img.width) // 2
        y = (self.device.height - resized_img.height) // 2

        background.paste(resized_img, (x, y))
        self.device.display(background)

    def clear(self, color=(0, 0, 0)):
        """
        清空屏幕，填充纯色。默认黑屏。
        """
        blank = Image.new("RGB", (self.device.width, self.device.height), color)
        self.device.display(blank)
