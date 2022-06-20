from typing import List

import cv2 as cv
import numpy as np

import index
from filters import overlay_transparent


class Image:

    def __init__(self):
        self.filters = index.index()
        self.sticker_selected = None
        self.states: List[np.ndarray] = []

    def draw_sticker(self, event, x, y, flags, param):
        if event == cv.EVENT_LBUTTONDOWN and self.sticker_selected is not None:
            img = cv.imread(self.sticker_selected, cv.IMREAD_UNCHANGED)

            i_y, i_x = img.shape[0], img.shape[1]

            self.states.append(overlay_transparent(self.states[-1], img, int(x - (i_x / 2)), int(y - (i_y / 2))))

    def execute(self, path):
        src = cv.imread(path, cv.IMREAD_COLOR)
        src = cv.resize(src, (0, 0), fx=0.4, fy=0.4)

        self.states.append(src)

        cv.namedWindow('Trabalho grau B')
        cv.setMouseCallback('Trabalho grau B', self.draw_sticker)

        print('1: Gaussian\n2: Grayscale\n3: Sharpen')
        print('A: Bob Esponja\nB: Gato de botas\nC: Óculos')

        while 1:

            img = self.states[-1]

            k = cv.waitKey(20) & 0xFF

            if k == 27:
                break

            elif index.is_sticker(k):
                self.sticker_selected = self.filters[k].path

            elif index.is_filter(k):
                self.states.append(self.filters[k].function(img))

            elif k == 8:
                if len(self.states) > 1:
                    self.states.pop()

            cv.imshow('Trabalho grau B', img)

