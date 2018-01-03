import cv2
import numpy as np
from matplotlib import pyplot as plt

image_path = '/home/shel/workspace/work_tasks/plates_recog/data/opencv_test2.bmp'
# image_path = '/home/shel/workspace/work_tasks/plates_recog/data/opencv_test3.bmp'

# Read image.
img_orig = cv2.imread(image_path)

img_gray = cv2.cvtColor(img_orig, cv2.COLOR_RGBA2GRAY)

#img_thresh = cv2.adaptiveThreshold(img_gray, 255, cv2.THRESH_OTSU, cv2.THRESH_BINARY, 7, 2)

# img_thresh = cv2.adaptiveThreshold(img_gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C,
                                #    cv2.THRESH_BINARY, 11, 2)
# Smooth edges.
blur = cv2.GaussianBlur(img_gray, (25, 25), 0)

# Binarize image with Otsu's threshold.
ret, img_thresh = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

# Show segmentation result.
plt.imshow(img_thresh)
plt.show()

# Find contours.
image, contours, hierarchy = cv2.findContours(img_thresh, cv2.RETR_TREE,
                                              cv2.CHAIN_APPROX_SIMPLE)

# cv2.drawContours(img_orig, contours, -1, (0,255,0), 3)

# Contour approximation. Remove redundancy.
approx = []
for i in range(len(contours)):
    epsilon = 0.1 * cv2.arcLength(contours[i], True)
    approx.append(cv2.approxPolyDP(contours[i], epsilon, True))

cv2.drawContours(img_orig, approx, -1, (0,255,0), 3)

plt.imshow(img_orig)
plt.show()
