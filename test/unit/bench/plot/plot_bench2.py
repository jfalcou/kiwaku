import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import utils.plot_utils as pu
import utils.plot_set_sizes as plot_set_sizes
import textwrap
import os.path
import argparse
import matplotlib.colors as mcolors


VERSION_ATTENDUE = 3

def set_axis_style(ax, labels):
  ax.set_xticks(np.arange(0, len(labels)), labels=labels)
  # ax.set_xlim(0.25, len(labels) + 0.75)
  # ax.set_ylim(0, 115)
  # ax.set_xlabel('Sample name')


# title_font_size = 16  # fontsize

plot_set_sizes.set_sizes()

max_text_width_on_axis = 16 # Legends such as "Kiwaku SYCL GPU"
# TODO: enlever Kiwaku carrément ? SYCL c'est assez clair si je le dis dans le texte.

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

# global_colors_reduce_only = [
#   'tab:olive',
#   'tab:blue',
#   'tab:orange',
#   'tab:green',
#   'tab:red',
#   'tab:purple',
#   'tab:brown',
#   'tab:pink',
#   'tab:gray',
#   'tab:cyan'
# ]
# global_colors = global_colors_reduce_only

# 2. Generate the darker versions
# We will scale the RGB values of each color by a factor to make them darker.
# A factor of 0.6 makes them noticeably darker.
darkening_factor = 0.8
darker_colors = []

for color_name in global_colors:
    # Convert the named color to its RGB representation (a tuple of floats from 0 to 1)
    rgb = mcolors.to_rgb(color_name)
    
    # Create the darker RGB by scaling each component
    darker_rgb = (rgb[0] * darkening_factor, 
                  rgb[1] * darkening_factor, 
                  rgb[2] * darkening_factor)
    
    darker_colors.append(darker_rgb)


# Dessiner un plot sur deux sur le dernier plot
DRAW_OVER = False
ALTERNATE_DRAW_OVER = True



# python3 plot_bench2.py final_files/reduce/parsys-legend_2025-07-30_19h51m56s_reduce_L2_-mavx2_-mfma_L2-256.bench
# python3 plot_bench2.py final_files/reduce/parsys-legend_2025-07-30_19h52m43s_reduce_RAM_-mavx2_-mfma_L2-6291456.bench




unit_name = ""

IS_MEMORY_BOUND  = False
IS_COMPUTE_BOUND = False
IS_KERNEL_DURATION = False

CURRENT_VERSION = 0
bench_list = []
bench_list2 = []

# Charge le fichier de bench "path" et retourne la liste de ce qui a été lu.
def load_file(path):
  global VERSION_ATTENDUE, CURRENT_VERSION, global_name, measured_variable, kwk_array_size, unit_name
  global IS_MEMORY_BOUND, IS_COMPUTE_BOUND, IS_KERNEL_DURATION, DRAW_OVER, bench_list, bench_list2
  

  with open(path) as fp:
    version = fp.readline() # version du fichier actuel (doit être 106 et non plus 105)
    print("File version: {}".format(version))

    if ((int(version) != VERSION_ATTENDUE) and not ((int(version) == 4) and (VERSION_ATTENDUE == 3))):
      sys.exit("ERROR, NON-COMPATIBLE FILE VERSION : " + str(int(version)) + ".  REQUIRED VERSION = " + str(VERSION_ATTENDUE))
    
    CURRENT_VERSION = int(version)

    global_name = fp.readline().rstrip("\n")
    measured_variable = fp.readline().rstrip("\n")

    # À ajouter à la main dans les fichiers de bench
    if (measured_variable == "DRAW_OVER"):
      DRAW_OVER = True
      measured_variable = fp.readline().rstrip("\n")


    kwk_array_size = int(fp.readline().rstrip("\n"))
    bound_type = fp.readline().rstrip("\n") # Memory or compute-bound

    if (bound_type == "compute-bound"):
      IS_COMPUTE_BOUND = True

    if (bound_type == "memory-bound"):
      IS_MEMORY_BOUND = True

    if (bound_type == "kernel-duration"):
      IS_KERNEL_DURATION = True


    all_medians = []
    all_medians2 = []

    line = fp.readline()
    while line:

      # number of items per second = 1000 * number of elements per ms
      # number of elements per ms = number of items / time (ms) to process these items
      # number of elements per second = 1000 * number of items / time (ms) to process these items

      res = {}
      res["bench_name"]         = line.rstrip("\n")
      res["raw_values"]         = pu.list_str_to_float(pu.remove_empty_words(pu.remove_newline(fp.readline().split(" ")))) #, 1 / kwk_array_size)
      res["no-outlier_values"]  = pu.filter_outliers(res["raw_values"])
      res["median"]             = stat.median(res["raw_values"])

      if DRAW_OVER:
        if ALTERNATE_DRAW_OVER:
          all_medians.append(res["median"])
          bench_list.append(res)
        else:
          all_medians2.append(res["median"])
          bench_list2.append(res)
        ALTERNATE_DRAW_OVER = not ALTERNATE_DRAW_OVER
      else:
        all_medians.append(res["median"])
        bench_list.append(res)

      line = fp.readline()

      if (line == "Elapsed (ms):\n"): # skip times header
        # print("EQUALS ELAPSED!")
        fp.readline()               # skip times
        line = fp.readline()
      # else:
        # print("NOT NOT NOT EQUALS ELAPSED.")

  # bench_list is now a global variable
  # return bench_list

#  python3 plot_bench.py files/2025-06-19-legend/renamed/parsys-legend_2025-06-19_23h07m47s_for_each_memory-bound.bench

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
load_file(input_fpath) # absolute_path + ifname



labels = []
med_values = []
med_values2 = []
bw_method_ = 0.04


for i in range(0, len(bench_list)):
  position = i
  bench = bench_list[i]

  wrapped = textwrap.fill(
    bench["bench_name"],
    width=max_text_width_on_axis,
    break_long_words=False,
    subsequent_indent=""
  )

  labels.append(wrapped)

  med_values.append(bench["median"])
  pu.draw_violin_plot_pos_ext(global_colors[i], global_colors[i], bench["no-outlier_values"], [position]) # no-outlier_


  if DRAW_OVER:
    bench2 = bench_list2[i]
    med_values2.append(bench2["median"])
    pu.draw_violin_plot_pos_ext(darker_colors[i], darker_colors[i], bench2["no-outlier_values"], [position])




plt.ylabel(measured_variable + unit_name)

plt.title(global_name + "") # , fontsize=title_font_size)

# set style for the axes
set_axis_style(plt.gca(), labels)

# plt.yscale('log') # log scale

line_width = 1

plt.rcParams['grid.linestyle'] = "-"
plt.rcParams['grid.alpha'] = 1
plt.rcParams['grid.color'] = "black" ##cccccc
plt.grid(linewidth=line_width)

plt.bar(labels, med_values, label=labels, color=global_colors, alpha=0.3)
if DRAW_OVER:
  plt.bar(labels, med_values2, label=labels, color=darker_colors, alpha=0.3)


# output_fname = global_name.replace(" ", "_").replace(",", "").lower() + ".png"
output_fname = input_fpath + ".png"
plt.savefig(output_fname, format="png") #, dpi=my_dpi)

if (not args.only_save_image):
  
  for i in range(0, len(bench_list)):
    bench = bench_list[i]
    unit_name = ""
    if IS_COMPUTE_BOUND: unit_name = " cycles per element"
    if IS_MEMORY_BOUND: unit_name = " GB/s"
    if IS_KERNEL_DURATION: unit_name = " ms"
    if (not IS_COMPUTE_BOUND) and (not IS_COMPUTE_BOUND) and (not IS_KERNEL_DURATION): unit_name = " !!UNKNOWN_UNIT!!"
    print(bench["bench_name"] + ": " + str(round(bench["median"])) + unit_name)
  
  plt.show()



