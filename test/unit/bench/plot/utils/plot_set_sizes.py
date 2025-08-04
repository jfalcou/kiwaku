
import matplotlib.pyplot as plt

def set_sizes(manuscript_version = True):
  # ============= Gestion de la taille
  my_dpi = 96
  # image_ratio = 640 / 480
  # image_ratio = 640 / 120
  image_ratio = 1280 / 268
  # image_ratio = 880 / 480

  if manuscript_version:
    image_ratio = 1280 / 268
  else:
    image_ratio = 1280 / 480

  image_width = 1280 # 1280
  image_scale_factor = image_width / 1024
  image_height = image_width / image_ratio

  plt.figure(figsize=(image_width/my_dpi, image_height/my_dpi) , dpi=my_dpi)

  font_factor = image_scale_factor

  MY_SIZE = (10 * font_factor)
  # MY_SIZE_SMALL = (6 * font_factor)
  TITLE_SIZE = (15 * font_factor)
  AXES_SIZE  = (13 * font_factor)

  #plt.rc('font', size=MY_SIZE)          # controls default text sizes
  plt.rc('axes', titlesize=TITLE_SIZE)     # fontsize of the axes title
  plt.rc('axes', labelsize=AXES_SIZE)     # fontsize of the x and y labels
  plt.rc('xtick', labelsize=MY_SIZE)    # fontsize of the tick labels
  plt.rc('ytick', labelsize=MY_SIZE)    # fontsize of the tick labels
  plt.rc('legend', fontsize=MY_SIZE)    # legend fontsize
  plt.rc('figure', titlesize=TITLE_SIZE)   # fontsize of the figure title