import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math
import utils.plot_utils as pu
import utils.plot_set_sizes as plot_set_sizes
import textwrap
import os.path
import argparse


# def draw_violin_plot(color, y_list):
#   c = color
#   bp_ = plt.violinplot(y_list, showextrema=False)
#   for pc in bp_['bodies']:
#       pc.set_facecolor(c)
#       pc.set_edgecolor(c) # #D43F3A black
#       pc.set_alpha(1)

VERSION_ATTENDUE = 3

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



kwk_array_size = 1
global_name = ""
measured_variable = ""
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

unit_name = ""


# Charge le fichier de bench "path" et retourne la liste de ce qui a été lu.
def load_file(path):
  global VERSION_ATTENDUE, global_name, measured_variable, kwk_array_size, unit_name
  bench_list = []

  with open(path) as fp:
    version = fp.readline() # version du fichier actuel (doit être 106 et non plus 105)
    print("File version: {}".format(version))

    if (int(version) != VERSION_ATTENDUE):
      sys.exit("ERROR, NON-COMPATIBLE FILE VERSION : " + str(int(version)) + ".  REQUIRED VERSION = " + str(VERSION_ATTENDUE))

    global_name = fp.readline().rstrip("\n")
    measured_variable = fp.readline().rstrip("\n")
    kwk_array_size = int(fp.readline().rstrip("\n"))

    all_medians = []

    line = fp.readline()
    while line:

      # number of items per second = 1000 * number of elements per ms
      # = 1000 * ()

      # number of elements per ms = number of items / time (ms) to process these items
      # number of elements per second = 1000 * number of items / time (ms) to process these items

      res = {}
      res["bench_name"]      = line.rstrip("\n")
      res["raw_time"]        = pu.list_str_to_int(pu.remove_empty_words(pu.remove_newline(fp.readline().split(" ")))) #, 1 / kwk_array_size)
      res["elements_per_second"] = []
      for time in res["raw_time"]:
        if (time == 0):
          print("ERROR\nERROR: time = 0 for " + res["bench_name"] + "\nERROR")
        else:
          eps = 1000 * kwk_array_size / time
          res["elements_per_second"].append(eps)
          # print("add " + str(eps))

      # res["no-outlier_time"] = pu.filter_outliers(res["raw_time"])
      # res["med_time"]        = stat.median(res["raw_time"])
      all_medians.append(stat.median(res["elements_per_second"]))
      bench_list.append(res)

      line = fp.readline()

    global_med = stat.median(all_medians)

    divide_by = 1

    if global_med / 1000 >= 1:
      divide_by = 1000
      unit_name = " (thousands)"

    if global_med / 1000000 >= 1:
      divide_by = 1000000
      unit_name = " (millions)"

    if global_med / 1000000000 >= 1:
      divide_by = 1000000000
      unit_name = " (billions)"

    for res in bench_list:
      for i in range(0, len(res["elements_per_second"])):
        res["elements_per_second"][i] = res["elements_per_second"][i] / divide_by

      res["no-outlier_elements_per_second"] = pu.filter_outliers(res["elements_per_second"])
      res["med_elements_per_second"]        = stat.median(res["elements_per_second"])
      # print()
      # print(res["bench_name"])
      # print(res["raw_time"])
      # print(res["elements_per_second"])
      # print(res["no-outlier_elements_per_second"])
      # print()

  return bench_list

# absolute_path = "/home/data_evo/data_sync/academique/These/kiwaku_2025-02/test/bench/bench_files/pata/"
# absolute_path = "/home/data_not_sync/These/kiwaku_build/"

# print(sys.argv[1])

# absolute_path = "/home/data_evo/data_sync/academique/These/kiwaku_2025-02/test/bench/plot/files/"
# bench_list = load_file(absolute_path + "test.txt")

# ifname = "find-if_compute-bound_2025-06-05_01h45_last-pos.txt"
# ifname = "find-if_compute-bound_2025-06-05_01h45_middle.txt"
# ifname = "find-if_memory-bound_2025-06-05_01h45_last-pos.txt"
# ifname = "sylvain-ThinkPad-T580_2025-06-09_23h54m57s_reduce_compute-bound.txt"
# ifname = "sylvain-ThinkPad-T580_2025-06-09_23h55m10s_reduce_memory-bound.txt"
# ifname = "parsys-legend_2025-06-11_01h07m32s_reduce_compute-bound.txt"

only_save = False

if len(sys.argv) == 2:
  if (sys.argv[1] == "only_save"):
    only_save = True


# if len(sys.argv) <= 1:
#   print("ERROR: Please provide a valid file path.")
#   sys.exit()

# fname = sys.argv[1]



parser = argparse.ArgumentParser(
                    prog='Kiwaku benchmark plots',
                    description='',
                    epilog='Text at the bottom of help')

parser.add_argument('filepath')           # positional argument
# parser.add_argument('-c', '--count')      # option that takes a value
parser.add_argument('-s', '--only_save_image',
                    action='store_true')  # on/off flag

args = parser.parse_args()
print(args.filepath, args.only_save_image)

input_fpath = args.filepath

if (not os.path.isfile(input_fpath)):
  print("ERROR: File does not exist. Please provide a valid file path.")
  print("ERROR: Missing file:")
  print(input_fpath)
  sys.exit()


# bench_list = load_file(absolute_path + "reduce_1d_2025-06-04_19h15_compute_bound.txt")
bench_list = load_file(input_fpath) # absolute_path + ifname






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

  # data  = bench["no-outlier_elements_per_second"] # raw_time
  # data2 = []
  # # Convert from ms to operations per second
  # for val in data:
  #   d = 1000 / val
  #   data2.append(d) 

  med_values.append(bench["med_elements_per_second"])

  position = i

  # pu.draw_violin_plot('red', data2)
  pu.draw_violin_plot_pos_ext(global_colors[i], global_colors[i], bench["no-outlier_elements_per_second"], [position]) # no-outlier_

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

plt.ylabel(measured_variable + unit_name)

plt.title(global_name + "") # , fontsize=title_font_size)
# + ", operations per second (higher is better)"

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


# for i in range(0, len(bench_list)):
#   bench = bench_list[i]
#   print(bench["bench_name"] + ": " + str(bench["med_time"]))


# output_fname = global_name.replace(" ", "_").replace(",", "").lower() + ".png"
output_fname = input_fpath + ".png"
plt.savefig(output_fname, format="png") #, dpi=my_dpi)

if (not args.only_save_image):
  plt.show()


