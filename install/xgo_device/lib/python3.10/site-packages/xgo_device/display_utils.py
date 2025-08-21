from PIL import Image
from luma.core.interface.serial import spi
from luma.lcd.device import st7789

class Display:
    def __init__(self, width=320, height=240, rotate=0, bus_speed_hz=40000000):
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
        try:
            resample = Image.Resampling.LANCZOS
        except AttributeError:
            resample = Image.ANTIALIAS

        img_ratio = image.width / image.height
        screen_ratio = max_width / max_height

        if img_ratio > screen_ratio:
            new_width = max_width
            new_height = int(max_width / img_ratio)
        else:
            new_height = max_height
            new_width = int(max_height * img_ratio)

        return image.resize((new_width, new_height), resample)

    def show_image(self, image: Image.Image):
        img_rgb = image.convert("RGB")
        resized_img = self.resize_image_keep_aspect(img_rgb, self.device.width, self.device.height)
        background = Image.new("RGB", (self.device.width, self.device.height), (0, 0, 0))
        x = (self.device.width - resized_img.width) // 2
        y = (self.device.height - resized_img.height) // 2
        background.paste(resized_img, (x, y))
        self.device.display(background)

    def clear(self, color=(0, 0, 0)):
        blank = Image.new("RGB", (self.device.width, self.device.height), color)
        self.device.display(blank)
