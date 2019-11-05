from matplotlib import pyplot as plt
import numpy as np


C = np.fromfile('Original_RGB_avg411', dtype='uint8', sep="")
C = np.reshape(C, (512,512,3))
plt.subplot(2, 2, 1)
plt.title('barbara_gray_avg_411')
plt.imshow(C)

C = np.fromfile('barbara_gray_ORIG', dtype='uint8', sep="")
C = np.reshape(C, (512,512,3))
plt.subplot(2, 2, 2)
plt.title('barbara_gray_avg_411_orig')
plt.imshow(C)



###########################################
C = np.fromfile('Original_RGB_avg411lena_color_ORIG', dtype='uint8', sep="")
C = np.reshape(C, (512,512,3))
plt.subplot(2, 2, 3)
plt.title('avg411lena_color_ORIG')
plt.imshow(C)

C = np.fromfile('Original_RGB_avg411lena_color', dtype='uint8', sep="")
C = np.reshape(C, (512,512,3))
plt.subplot(2, 2, 4)
plt.title('avg411lena_color')
plt.imshow(C)

plt.tight_layout()
plt.show()