import cv2
import numpy as np
from matplotlib import pyplot as plt


image_path = '/home/shel/workspace/work_tasks/plates_recog/data/opencv_test2.bmp'
# image_path = '/home/shel/workspace/work_tasks/plates_recog/data/opencv_test3.bmp'


def center_by_diagonals_intersection(cnt):
    x1 = cnt[0][0][0]
    y1 = cnt[0][0][1]
    x3 = cnt[1][0][0]
    y3 = cnt[1][0][1]
    x2 = cnt[2][0][0]
    y2 = cnt[2][0][1]
    x4 = cnt[3][0][0]
    y4 = cnt[3][0][1]
    Cx = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / \
         ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    Cy = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / \
         ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))

    return (Cx, Cy)


def center_by_average(cnt):
    Cx, Cy = np.average(cnt[0, :]), np.average(cnt[1, :])
    return (Cx, Cy)


def center_by_moments(contour):
    M = cv2.moments(contour)
    Cx = int(M['m10'] / M['m00'])
    Cy = int(M['m01'] / M['m00'])
    return (Cx, Cy)


def euclid_dist(vec):
    return np.sqrt(np.sum(np.square(vec)))


def distance_between_contours(cntA, cntB):
    print("distance_between_contours")
    dist = 0

    # Find distance.
    min_dist = euclid_dist((cntA[0], cntB[0]))
    for point in cntA:
        print("point = ", point[0])
        print("cntB = ", cntB)
        dist = np.abs(cv2.pointPolygonTest(cntB, (point[0][0], point[0][1]), True))
        print("dist = ", dist)
        if min_dist > dist:
            min_dist = dist
    dist = min_dist

    return dist

# def distance_between_contours(cntA, cntB):
#     dist = 0
#     # Check for intersections.
#     #blob1 = cv2.drawContours(img_orig, rect_contours, -1, (0, 0, 255), 3)
#     blank = np.zeros(img_orig.shape[0:2])
#     img1 = cv2.drawContours(blank.copy(), cntA, -1, 1)
#     print("img1.shape =,", img1.shape ," img1.max() = ", img1.max())
#     img2 = cv2.drawContours(blank.copy(), cntB, -1, 1)
#     print("img2.shape =,", img2.shape ," img2.max() = ", img2.max())
#     plt.imshow(img1)
#     plt.show()
#     plt.imshow(img2)
#     plt.show()
#     intersection = np.logical_and(img1, img2)
#     print("intersection.shape =,", intersection.shape ," intersection.max() = ", intersection.max())
#
#     if intersection.max() == False:
#         # Find distance.
#         min_dist = euclid_dist((cntA[0], cntB[0]))
#         for point in cntA:
#             print("point = ", point[0])
#             print("cntB = ", cntB)
#             dist = cv2.pointPolygonTest(cntB, (point[0][0], point[0][1]), True)
#             if min_dist > dist:
#                 min_dist = dist
#         dist = min_dist
#     return dist


def find_rectangles(simplified_contours):
    # Find objects with right angles.
    # Let's try ret = cv2.matchShapes(cnt1,cnt2,1,0.0)
    cos_eps = 0.4
    # Loop over contours.
    rect_contours = []
    ids = []

    # Firstly find objects with 4 points.
    for i in range(len(simplified_contours)):
        cnt = simplified_contours[i]
        if len(cnt) == 4:
            # Loop over points.
            sum_of_cos = 0
            for j in range(4):
                vec1 = cnt[j - 1] - cnt[j]
                vec2 = cnt[j] - cnt[(j + 1) % 4]
                norm1 = np.sqrt(np.sum(np.square(vec1)))
                norm2 = np.sqrt(np.sum(np.square(vec2)))
                print("norm1, norm2: ", norm1, norm2)
                print("vec1, vec2: ", vec1, vec2)
                print("np.dot(vec1, vec2): ", np.dot(vec1, vec2.transpose()))
                dot_cos = np.dot(vec1, vec2.transpose()) / (norm1 * norm2)
                sum_of_cos += np.abs(dot_cos)
            print("Sum of cos: ", sum_of_cos)
            if sum_of_cos < cos_eps:
                rect_contours.append(cnt)
                ids.append(i)
    return rect_contours, ids


# def find_rectangles(contours):
#     # Find objects with 4 points.
#     four_angles_contours = []
#     for i in range(len(approx)):
#         cnt = approx[i]
#         if len(cnt) == 4:
#             four_angles_contours.append(cnt)
#
#     # Find objects with right angles.
#     # Let's try ret = cv2.matchShapes(cnt1,cnt2,1,0.0)
#     cos_eps = 0.3
#     # Loop over contours.
#     rect_contours = []
#     ids = []
#     # for cnt in four_angles_contours:
#     for i in range(len(four_angles_contours)):
#         cnt = four_angles_contours[i]
#         # Loop over points.
#         sum_of_cos = 0
#         for j in range(4):
#             vec1 = cnt[j - 1] - cnt[j]
#             vec2 = cnt[j] - cnt[(j + 1) % 4]
#             norm1 = np.sqrt(np.sum(np.square(vec1)))
#             norm2 = np.sqrt(np.sum(np.square(vec2)))
#             print("norm1, norm2: ", norm1, norm2)
#             print("vec1, vec2: ", vec1, vec2)
#             print("np.dot(vec1, vec2): ", np.dot(vec1, vec2.transpose()))
#             dot_cos = np.dot(vec1, vec2.transpose()) / (norm1 * norm2)
#             sum_of_cos += np.abs(dot_cos)
#         print("Sum of cos: ", sum_of_cos)
#         if sum_of_cos < cos_eps:
#             rect_contours.append(cnt)
#             ids.append(i)
#     return rect_contours, ids


def binarization(img):
    img_gray = cv2.cvtColor(img_orig, cv2.COLOR_RGBA2GRAY)

    #img_thresh = cv2.adaptiveThreshold(img_gray, 255, cv2.THRESH_OTSU, cv2.THRESH_BINARY, 7, 2)

    # img_thresh = cv2.adaptiveThreshold(img_gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C,
                                    #    cv2.THRESH_BINARY, 11, 2)
    # Smooth edges.
    blur = cv2.GaussianBlur(img_gray, (25, 25), 0)

    # Binarize image with Otsu's threshold.
    #ret, img_bw = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    #print("ret = ", ret)
    img_bw = cv2.adaptiveThreshold(blur, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
                                   cv2.THRESH_BINARY, 113, 1) #np.int(ret)
    return img_bw


# Read image.
img_orig = cv2.imread(image_path)

img_thresh = binarization(img_orig)

# Show segmentation result.
plt.imshow(img_thresh)
plt.show()

# Find contours.
image, contours, hierarchy = cv2.findContours(img_thresh, cv2.RETR_TREE,
                                              cv2.CHAIN_APPROX_NONE)
                                              #cv2.CHAIN_APPROX_SIMPLE)

# Get rid of too small objects.
contours_thr = []
area_thresh = 10000
for cnt in contours:
    if len(cnt) > 3:
        if cv2.contourArea(cnt) > area_thresh:
            contours_thr.append(cnt)

# cv2.drawContours(img_orig, contours, -1, (0,255,0), 3)

# Contour approximation. Remove redundancy.
approx = []
for i in range(len(contours_thr)):
    epsilon = 0.1 * cv2.arcLength(contours_thr[i], True)
    cnt_apprx = cv2.approxPolyDP(contours_thr[i], epsilon, True)
    if len(cnt_apprx) > 3:
        approx.append(cnt_apprx)

cv2.drawContours(img_orig, approx, -1, (0, 255, 0), 3)

# Find rectangles.
rect_contours, rect_ids = find_rectangles(approx)

# Highlite rectangles.
cv2.drawContours(img_orig, rect_contours, -1, (0, 0, 255), 3)
# cv2.drawContours(img_orig, contours, rect_ids[0], (255, 0, 255), 9)
# cv2.drawContours(img_orig, contours, rect_ids[1], (255, 0, 255), 9)

plt.imshow(img_orig)
plt.show()

# Estimate intersections of rectangles' diagonals.
centers = []
# for i in range(len(rect_contours)):
for cnt in rect_contours:
    centers.append(center_by_moments(cnt))
    # centers.append(center_by_diagonals_intersection(rect_contours))

d = np.sqrt(np.sum(np.square((centers[0][0] - centers[1][0], centers[0][1] - centers[1][1]))))

# min_inter_dist = distance_between_contours(rect_contours[0], rect_contours[1])
min_inter_dist = distance_between_contours(contours[rect_ids[0]], contours[rect_ids[1]])

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
