from matplotlib import pyplot as plt
import numpy as np


C = np.fromfile('lena_color_out_image', dtype='uint8', sep="")
C = np.reshape(C, (512,512,3))
plt.subplot(2, 2, 1)
plt.title('lena_color_out_image')
plt.imshow(C)

C = np.fromfile('barbara_gray_out_image', dtype='uint8', sep="")
C = np.reshape(C, (512,512,3))
plt.subplot(2, 2, 2)
plt.title('barbara_gray_out_image')
plt.imshow(C)



###########################################
C = np.fromfile('lena_color_out_image_afterICC', dtype='uint8', sep="")
C = np.reshape(C, (512,512,3))
plt.subplot(2, 2, 3)
plt.title('lena_color_out_image_afterICC')
plt.imshow(C)

C = np.fromfile('barbara_gray_out_image_afterICC', dtype='uint8', sep="")
C = np.reshape(C, (512,512,3))
plt.subplot(2, 2, 4)
plt.title('barbara_gray_out_image_afterICC')
plt.imshow(C)

plt.tight_layout()
plt.show()