import cv2
import numpy as np
from matplotlib import pyplot as plt

image_path = '/home/shel/workspace/work_tasks/plates_recog/data/opencv_test2.bmp'
# image_path = '/home/shel/workspace/work_tasks/plates_recog/data/opencv_test3.bmp'

def center_by_diagonals_intersection(contour):

def center_by_average(contour):

def center_by_moments(contour):

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
                                              #cv2.CHAIN_APPROX_NONE)
                                              cv2.CHAIN_APPROX_SIMPLE)

# cv2.drawContours(img_orig, contours, -1, (0,255,0), 3)

# Contour approximation. Remove redundancy.
approx = []
for i in range(len(contours)):
    epsilon = 0.1 * cv2.arcLength(contours[i], True)
    approx.append(cv2.approxPolyDP(contours[i], epsilon, True))

cv2.drawContours(img_orig, approx, -1, (0, 255, 0), 3)

# Find objects with 4 points.
four_angles_contours = []
for i in range(len(approx)):
    cnt = approx[i]
    if len(cnt) == 4:
        four_angles_contours.append(cnt)

# Find objects with right angles.
# Let's try ret = cv2.matchShapes(cnt1,cnt2,1,0.0)
cos_eps = 0.3
# Loop over contours.
rect_contours = []
for i in range(len(four_angles_contours)):
    rect = four_angles_contours[i]
    # Loop over points.
    sum_of_cos = 0
    for j in range(4):
        vec1 = rect[j - 1] - rect[j]
        vec2 = rect[j] - rect[(j + 1) % 4]
        norm1 = np.sqrt(np.sum(np.square(vec1)))
        norm2 = np.sqrt(np.sum(np.square(vec2)))
        print("norm1, norm2: ", norm1, norm2)
        print("vec1, vec2: ", vec1, vec2)
        print("np.dot(vec1, vec2): ", np.dot(vec1, vec2.transpose()))
        dot_cos = np.dot(vec1, vec2.transpose()) / (norm1 * norm2)
        sum_of_cos += np.abs(dot_cos)
    print("Sum of cos: ", sum_of_cos)
    if sum_of_cos < cos_eps:
        rect_contours.append(rect)

# Highlite rectangles.
cv2.drawContours(img_orig, rect_contours, -1, (0, 0, 255), 3)

# Estimate intersections of rectangles' diagonals.
centers = []
for i in range(len(rect_contours)):
    x1 = rect_contours[i][0][0][0]
    y1 = rect_contours[i][0][0][1]
    x3 = rect_contours[i][1][0][0]
    y3 = rect_contours[i][1][0][1]
    x2 = rect_contours[i][2][0][0]
    y2 = rect_contours[i][2][0][1]
    x4 = rect_contours[i][3][0][0]
    y4 = rect_contours[i][3][0][1]
    Cx = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / \
         ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    Cy = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / \
         ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    centers.append((Cx, Cy))

d = np.sqrt(np.sum(np.square((centers[0][0] - centers[1][0], centers[0][1] - centers[1][1]))))

# Calculate distance between rectangles.
# Distance between point and contour.
# dist = cv2.pointPolygonTest(cnt,(50,50),True)


# Calculate vectors.
# Calculate cosinuses between vectors


# Find corner points.
# for i in range(len(contours)):
#     cnt = approx[2]
#     leftmost = tuple(cnt[cnt[:,:,0].argmin()][0])
#     rightmost = tuple(cnt[cnt[:,:,0].argmax()][0])
#     topmost = tuple(cnt[cnt[:,:,1].argmin()][0])
#     bottommost = tuple(cnt[cnt[:,:,1].argmax()][0])


plt.imshow(img_orig)
plt.show()
