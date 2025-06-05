import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math
import utils.plot_utils as pu
import utils.plot_set_sizes as plot_set_sizes
import textwrap

# def draw_violin_plot(color, y_list):
#   c = color
#   bp_ = plt.violinplot(y_list, showextrema=False)
#   for pc in bp_['bodies']:
#       pc.set_facecolor(c)
#       pc.set_edgecolor(c) # #D43F3A black
#       pc.set_alpha(1)

VERSION_ATTENDUE = 1

def set_axis_style(ax, labels):
  ax.set_xticks(np.arange(0, len(labels)), labels=labels)
  # ax.set_xlim(0.25, len(labels) + 0.75)
  # ax.set_ylim(0, 115)
  # ax.set_xlabel('Sample name')


title_font_size = 16  # fontsize

plot_set_sizes.set_sizes()

# # ============= Gestion de la taille
# my_dpi = 96
# output_image_name = "no_name"
# field_type_name = "" # InterpolateNN_LayoutStride

# # image_ratio = 640 / 480
# image_ratio = 880 / 480

# image_width = 1280 # 1280
# image_height = 0
# image_scale_factor = image_width / 640
# line_width = image_scale_factor * 1.5
# image_height = image_width / image_ratio #(image_width / 640) * 480

# plt.figure(figsize=(image_width/my_dpi, image_height/my_dpi) , dpi=my_dpi)
# # output_image_name = "lorentz_compare_" + computer_name + "_" + output_image_name + ".png"
# output_image_name = "lorentz_compare_" + computer_name + "_" + output_image_name + ".png"

# MY_SIZE = (10 * image_scale_factor)
# MY_SIZE_SMALL = (6 * image_scale_factor)
# TITLE_SIZE = (12 * image_scale_factor)

# #plt.rc('font', size=MY_SIZE)          # controls default text sizes
# plt.rc('axes', titlesize=TITLE_SIZE)     # fontsize of the axes title
# plt.rc('axes', labelsize=MY_SIZE)     # fontsize of the x and y labels
# plt.rc('xtick', labelsize=MY_SIZE)    # fontsize of the tick labels
# plt.rc('ytick', labelsize=MY_SIZE)    # fontsize of the tick labels
# plt.rc('legend', fontsize=MY_SIZE)    # legend fontsize
# #plt.rc('figure', titlesize=MY_SIZE)   # fontsize of the figure title



global_name = ""
global_colors = [
  'tab:blue',
  'tab:orange',
  'tab:green',
  'tab:red',
  'tab:purple',
  'tab:brown',
  'tab:pink',
  'tab:gray',
  'tab:olive',
  'tab:cyan'
]



# Charge le fichier de bench "path" et retourne la liste de ce qui a été lu.
def load_file(path):
  global VERSION_ATTENDUE, global_name
  bench_list = []

  with open(path) as fp:
    version = fp.readline() # version du fichier actuel (doit être 106 et non plus 105)
    print("Version du fichier : {}".format(version))

    if (int(version) != VERSION_ATTENDUE):
      sys.exit("ERREUR, VERSION DU FICHIER NON COMPATIBLE : " + str(int(version)) + ".  VERSION ATTENDUE = " + str(VERSION_ATTENDUE))

    divide_factor = 1

    global_name = fp.readline().rstrip("\n")

    line = fp.readline()
    while line:

      res = {}
      res["bench_name"]     = line.rstrip("\n")
      res["raw_time"]       = pu.list_str_to_int(pu.remove_empty_words(pu.remove_newline(fp.readline().split(" "))), divide_factor)
      res["no-outier_time"] = pu.filter_outliers(res["raw_time"])
      res["med_time"]       = stat.median(res["raw_time"])
      bench_list.append(res)

      line = fp.readline()

  return bench_list

absolute_path = "/home/data_evo/data_sync/academique/These/kiwaku_2025-02/test/bench/bench_files/pata/"
# bench_list = load_file(absolute_path + "test.txt")

# ifname = "find-if_compute-bound_2025-06-05_01h45_last-pos.txt"
# ifname = "find-if_compute-bound_2025-06-05_01h45_middle.txt"
# ifname = "find-if_memory-bound_2025-06-05_01h45_last-pos.txt"
ifname = "find-if_memory-bound_2025-06-05_01h45_middle.txt"

# bench_list = load_file(absolute_path + "reduce_1d_2025-06-04_19h15_compute_bound.txt")
bench_list = load_file(absolute_path + ifname)






labels = []
med_values = []
bw_method_ = 0.04



for i in range(0, len(bench_list)):
  bench = bench_list[i]

  wrapped = textwrap.fill(
    bench["bench_name"],
    width=18,
    break_long_words=False,
    subsequent_indent=""
  )

  labels.append(wrapped)

  data  = bench["no-outier_time"] # raw_time
  data2 = []
  # Convert from ms to operations per second
  for val in data:
    d = 1000 / val
    data2.append(d) 

  med_values.append(stat.median(data2))

  position = i

  # pu.draw_violin_plot('red', data2)
  pu.draw_violin_plot_pos_ext(global_colors[i], global_colors[i], data2, [position])

  # bp_ = plt.violinplot(y_list, showextrema=False)
  # for pc in bp_['bodies']:
  #     pc.set_facecolor(color)
  #     pc.set_edgecolor(color) # #D43F3A black
  #     pc.set_alpha(1)

  # violin_parts = plt.violinplot(data2, [position], points=600, widths=0.7, showmeans=False,
  #                               showextrema=True, showmedians=True, bw_method=bw_method_)

  # color='blue'
  # for pc in violin_parts['bodies']:
  #   pc.set_facecolor(color)
  #   pc.set_edgecolor(color)
  #   pc.set_alpha(1)

plt.title(global_name + ", operations per second (higher is better)") # , fontsize=title_font_size)

# set style for the axes
set_axis_style(plt.gca(), labels)

line_width = 1

plt.rcParams['grid.linestyle'] = "-"
plt.rcParams['grid.alpha'] = 1
plt.rcParams['grid.color'] = "black" ##cccccc
plt.grid(linewidth=line_width)


# bar_colors = ['tab:red', 'tab:blue', 'tab:red', 'tab:orange']
# # bar_colors = ['tab:red', 'tab:blue', 'tab:red', 'tab:orange']

plt.bar(labels, med_values, label=labels, color=global_colors, alpha=0.3)


for i in range(0, len(bench_list)):
  bench = bench_list[i]
  print(bench["bench_name"] + ": " + str(bench["med_time"]))


output_fname = global_name.replace(" ", "_").replace(",", "").lower() + ".png"
plt.savefig(output_fname, format='png') #, dpi=my_dpi)

plt.show()


