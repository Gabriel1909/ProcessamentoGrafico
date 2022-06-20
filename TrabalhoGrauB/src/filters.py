import copy

import cv2 as cv
import numpy as np


def is_gray(img):
    return len(img.shape) < 3


def gaussian(img):
    return cv.GaussianBlur(img, (5, 5), 0)


def gray(img):
    if is_gray(img):
        return img

    return cv.cvtColor(img, cv.COLOR_BGR2GRAY)


def sharpen(img):
    kernel = np.array([[-1, -1, -1], [-1, 9.5, -1], [-1, -1, -1]])
    return cv.filter2D(img, -1, kernel)


def bright(img):
    return cv.convertScaleAbs(img, beta=-60)


def sepia(img):
    img_sepia = np.array(img, dtype=np.float64)
    img_sepia = cv.transform(img_sepia, np.matrix([[0.272, 0.534, 0.131],
                                                   [0.349, 0.686, 0.168],
                                                   [0.393, 0.769, 0.189]]))
    img_sepia[np.where(img_sepia > 255)] = 255
    return np.array(img_sepia, dtype=np.uint8)


def hdr(img):
    return cv.detailEnhance(img, sigma_s=12, sigma_r=0.15)


def hist(img):
    if is_gray(img):
        return cv.equalizeHist(img)
    return cv.equalizeHist(gray(img))


def canny(img):
    return cv.Canny(img, 25, 75)


def sobel(img):
    grad_x = cv.Sobel(img, cv.CV_16S, 1, 0, ksize=3, scale=1, delta=0, borderType=cv.BORDER_DEFAULT)
    grad_y = cv.Sobel(img, cv.CV_16S, 0, 1, ksize=3, scale=1, delta=0, borderType=cv.BORDER_DEFAULT)
    abs_grad_x = cv.convertScaleAbs(grad_x)
    abs_grad_y = cv.convertScaleAbs(grad_y)
    return cv.addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0)


def invert(img):
    return cv.bitwise_not(img)


def overlay_transparent(background, overlay, x, y):
    try:
        new_img = copy.deepcopy(background)

        background_width = background.shape[1]
        background_height = background.shape[0]

        if x >= background_width or y >= background_height:
            return background

        h, w = overlay.shape[0], overlay.shape[1]

        if x + w > background_width:
            w = background_width - x
            overlay = overlay[:, :w]

        if y + h > background_height:
            h = background_height - y
            overlay = overlay[:h]

        if overlay.shape[2] < 4:
            overlay = np.concatenate(
                [
                    overlay,
                    np.ones((overlay.shape[0], overlay.shape[1], 1), dtype=overlay.dtype) * 255
                ],
                axis=2,
            )

        overlay_image = overlay[..., :3]
        mask = overlay[..., 3:] / 255.0

        new_img[y:y + h, x:x + w] = (1.0 - mask) * background[y:y + h, x:x + w] + mask * overlay_image

        return new_img
    except:
        return background
