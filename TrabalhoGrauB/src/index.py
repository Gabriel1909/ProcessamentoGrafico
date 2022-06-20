from control import Control
from filters import gaussian, gray, hist, bright, sepia, sharpen, sobel, hdr, invert, canny

MIN_STICKER = 97
MAX_STICKER = 105
MIN_FILTER = 48
MAX_FILTER = 57


def index():
    sticker_path = '../assets/stickers/'

    return {
        97: Control(sticker_path + 'bob.png'),
        98: Control(sticker_path + 'gato.png'),
        99: Control(sticker_path + 'oculos.png'),
        100: Control(sticker_path + 'cj.png'),
        101: Control(sticker_path + 'doge.png'),
        102: Control(sticker_path + 'pikachu.png'),
        103: Control(sticker_path + 'flork.png'),
        104: Control(sticker_path + 'madruga.png'),
        105: Control(sticker_path + 'julius.png'),
        48: Control(gaussian),
        49: Control(gray),
        50: Control(sharpen),
        51: Control(bright),
        52: Control(sepia),
        53: Control(hdr),
        54: Control(hist),
        55: Control(canny),
        56: Control(sobel),
        57: Control(invert),
    }


def print_index():
    print('Pressione os seguintes digitos para aplicar os filtros: \n0: Gaussian\n1: Grayscale\n2: Sharpen\n3: '
          'Bright\n4: Sepia\n5: HDR\n6: Histograma\n7: Canny\n8: Sobel\n9: Invert')

    print('Pressione os seguintes digitos e clique com o mouse para colar os stickers:\nA: Bob Esponja\nB: Gato '
          'de botas\nC: Óculos\nD: CJ\nE: Doge\nF: Pikachu\nG: Flork\nH: Seu madruga\nI: Julius')


def is_sticker(index):
    return MIN_STICKER <= index <= MAX_STICKER


def is_filter(index):
    return MIN_FILTER <= index <= MAX_FILTER
