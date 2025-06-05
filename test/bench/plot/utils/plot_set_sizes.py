
import matplotlib.pyplot as plt

def set_sizes():
  # ============= Gestion de la taille
  my_dpi = 96
  image_ratio = 640 / 480
  # image_ratio = 880 / 480

  image_width = 1280 # 1280
  image_scale_factor = image_width / 1024
  image_height = image_width / image_ratio

  plt.figure(figsize=(image_width/my_dpi, image_height/my_dpi) , dpi=my_dpi)

  MY_SIZE = (10 * image_scale_factor)
  # MY_SIZE_SMALL = (6 * image_scale_factor)
  TITLE_SIZE = (15 * image_scale_factor)

  #plt.rc('font', size=MY_SIZE)          # controls default text sizes
  plt.rc('axes', titlesize=TITLE_SIZE)     # fontsize of the axes title
  plt.rc('axes', labelsize=MY_SIZE)     # fontsize of the x and y labels
  plt.rc('xtick', labelsize=MY_SIZE)    # fontsize of the tick labels
  plt.rc('ytick', labelsize=MY_SIZE)    # fontsize of the tick labels
  plt.rc('legend', fontsize=MY_SIZE)    # legend fontsize
  plt.rc('figure', titlesize=TITLE_SIZE)   # fontsize of the figure title