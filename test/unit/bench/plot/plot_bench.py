import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import utils.plot_utils as pu
import utils.plot_set_sizes as plot_set_sizes
import textwrap
import os.path
import argparse


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
      res["med_time"]        = stat.median(res["raw_time"])
      all_medians.append(stat.median(res["elements_per_second"]))
      bench_list.append(res)

      line = fp.readline()

    global_ref = max(all_medians)

    divide_by = 1

    if global_ref / 1000 >= 1:
      divide_by = 1000
      unit_name = " (thousands)"

    if global_ref / 1000000 >= 1:
      divide_by = 1000000
      unit_name = " (millions)"

    if global_ref / 1000000000 >= 1:
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
bench_list = load_file(input_fpath) # absolute_path + ifname



labels = []
med_values = []
bw_method_ = 0.04


for i in range(0, len(bench_list)):
  bench = bench_list[i]

  wrapped = textwrap.fill(
    bench["bench_name"],
    width=max_text_width_on_axis,
    break_long_words=False,
    subsequent_indent=""
  )

  labels.append(wrapped)

  med_values.append(bench["med_elements_per_second"])

  position = i

  # pu.draw_violin_plot('red', data2)
  pu.draw_violin_plot_pos_ext(global_colors[i], global_colors[i], bench["no-outlier_elements_per_second"], [position]) # no-outlier_


plt.ylabel(measured_variable + unit_name)

plt.title(global_name + "") # , fontsize=title_font_size)

# set style for the axes
set_axis_style(plt.gca(), labels)

line_width = 1

plt.rcParams['grid.linestyle'] = "-"
plt.rcParams['grid.alpha'] = 1
plt.rcParams['grid.color'] = "black" ##cccccc
plt.grid(linewidth=line_width)

plt.bar(labels, med_values, label=labels, color=global_colors, alpha=0.3)


# output_fname = global_name.replace(" ", "_").replace(",", "").lower() + ".png"
output_fname = input_fpath + ".png"
plt.savefig(output_fname, format="png") #, dpi=my_dpi)

if (not args.only_save_image):
  
  for i in range(0, len(bench_list)):
    bench = bench_list[i]
    print(bench["bench_name"] + ": " + str(round(bench["med_elements_per_second"] * 100) / 100) + " elements per second")
  
  print(" ====== ")

  for i in range(0, len(bench_list)):
    bench = bench_list[i]
    print(bench["bench_name"] + ": " + str(round(bench["med_time"])) + " ms per function call")
  
  plt.show()



