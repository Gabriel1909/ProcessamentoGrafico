import cv2 as cv

import index
from filters import overlay_transparent


class Video:

    def __init__(self):
        self.filter = index.index()
        self.sticker_selected = None

    def update_sticker(self, event, x, y, flags, param):

        if event == cv.EVENT_LBUTTONDOWN and self.sticker_selected is not None:
            image = cv.imread(self.sticker_selected.path, cv.IMREAD_UNCHANGED)
            i_y, i_x = image.shape[0], image.shape[1]
            self.sticker_selected.x = int(x - (i_x / 2))
            self.sticker_selected.y = int(y - (i_y / 2))
            self.sticker_selected.active = True

    def execute(self, path):
        cv.namedWindow('Trabalho grau B')
        cv.setMouseCallback('Trabalho grau B', self.update_sticker)

        captura = cv.VideoCapture(path)

        index.print_index()

        while 1:

            ret, frame = captura.read()

            k = cv.waitKey(20) & 0xFF

            if k == 27:
                break

            elif index.is_filter(k):
                self.filter[k].active = not self.filter[k].active

            elif index.is_sticker(k):
                self.sticker_selected = self.filter[k]

            for k, v in self.filter.items():
                if v.active:
                    if index.is_sticker(k):
                        img = cv.imread(v.path, cv.IMREAD_UNCHANGED)
                        img = cv.resize(img, None, fx=0.7, fy=0.7)
                        frame = overlay_transparent(frame, img, v.x, v.y)

                    elif index.is_filter(k):
                        frame = v.function(frame)

            cv.imshow('Trabalho grau B', frame)

        captura.release()
