from control import Control
from filters import gaussian, gray, hist, bright, sepia, sharpen, sobel, hdr, invert, canny

MIN_STICKER = 97
MAX_STICKER = 99
MIN_FILTER = 48
MAX_FILTER = 57


def index():
    sticker_path = '../assets/stickers/'

    return {
        97: Control(sticker_path + 'bob.png'),
        98: Control(sticker_path + 'gato.png'),
        99: Control(sticker_path + 'oculos.png'),
        48: Control(invert),
        49: Control(gaussian),
        50: Control(gray),
        51: Control(sharpen),
        52: Control(bright),
        53: Control(sepia),
        54: Control(hdr),
        55: Control(hist),
        56: Control(canny),
        57: Control(sobel)
    }


def is_sticker(index):
    return MIN_STICKER <= index <= MAX_STICKER


def is_filter(index):
    return MIN_FILTER <= index <= MAX_FILTER
