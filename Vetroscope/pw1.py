import numpy as np
import cv2
import time
import sys

skip_factor = 1
resulting_size = 512

if len(sys.argv) != 2:
    print("Not enough arguments")
    exit(1)

start = time.time_ns() // 1000

image = cv2.imread(sys.argv[1])
cv2.imshow("Original", image)

end = time.time_ns() // 1000
print(f"Image read in {end - start}ms")

width, height, depth = image.shape

start = time.time_ns() // 1000

R, G, B = image[::skip_factor,::skip_factor,0], image[::skip_factor,::skip_factor,1], image[::skip_factor,::skip_factor,2]

end = time.time_ns() // 1000
print(f"Image decomposition done in {end - start}ms")

start = time.time_ns() // 1000

scale_factor = resulting_size / 256

Y = (0.299 * R + 0.587 * G + 0.114 * B) * scale_factor + resulting_size / 2
Cb = (-0.169 * R - 0.331 * G + 0.500 * B) * scale_factor + resulting_size / 2 
Cr = (0.500 * R - 0.418 * G - 0.082 * B) * scale_factor + resulting_size / 2

end = time.time_ns() // 1000
print(f"YCbCr conversion done in {end - start}ms")

start = time.time_ns() // 1000

vetroscope = np.zeros((resulting_size, resulting_size, 3), dtype=np.uint8)

for i, (xcol, ycol) in enumerate(zip(Cb[::skip_factor], Cr[::skip_factor])):
    for j, (x, y) in enumerate(zip(xcol, ycol)):
        x = int(x)
        y = int(y)
        vetroscope[x, y][0] = R[i,j]
        vetroscope[x, y][1] = G[i,j]
        vetroscope[x, y][2] = B[i,j]

end = time.time_ns() // 1000
print(f"Vetroscope drawn in {end - start}ms")

cv2.imshow("Vetroscope", vetroscope)

while (cv2.waitKey() != 27):
    pass
