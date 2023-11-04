#!/usr/bin/python

# Hello world python program

# Lancer le script : python3 ./2022-02-08_sparseccl.py

import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math
import plot_utils as pu

# Plots diff between Lorentz standalone and Kiwaku, omitting Covfie


VERSION_ATTENDUE = 2

computer_name = "blop" # aussi utilisé pour le nom du fichier d'entrée
str_min_iteration_count = "10" # added to plot title only


# ============= Gestion de la taille
my_dpi = 96
output_image_name = "no_name"
field_type_name = "" # InterpolateNN_LayoutStride

image_ratio = 880 / 480

image_width = 1280 # 1280
image_height = 0
image_scale_factor = image_width / 640
line_width = image_scale_factor * 1.5
image_height = image_width / image_ratio #(image_width / 640) * 480

plt.figure(figsize=(image_width/my_dpi, image_height/my_dpi) , dpi=my_dpi)
# output_image_name = "lorentz_compare_" + computer_name + "_" + output_image_name + ".png"
output_image_name = "lorentz_compare_" + computer_name + "_" + output_image_name + ".png"

MY_SIZE = (10 * image_scale_factor)
MY_SIZE_SMALL = (6 * image_scale_factor)
TITLE_SIZE = (12 * image_scale_factor)

#plt.rc('font', size=MY_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=TITLE_SIZE)     # fontsize of the axes title
plt.rc('axes', labelsize=MY_SIZE)     # fontsize of the x and y labels
plt.rc('xtick', labelsize=MY_SIZE)    # fontsize of the tick labels
plt.rc('ytick', labelsize=MY_SIZE)    # fontsize of the tick labels
plt.rc('legend', fontsize=MY_SIZE)    # legend fontsize
#plt.rc('figure', titlesize=MY_SIZE)   # fontsize of the figure title
# plt.rcParams.update({'font.size': MY_SIZE})

# fin gestion de la taille =============

# Ici, une courbe n'est qu'une suite toute basique de points

# lorentz_external_v2_fname = "build/lorentz-euler_v2.txt"

def remove_newline(list):
  if (len(list) == 1) and (list[0] == "\n"):
    del list[0]
    # print("remove_newline newsize = " + str(len(list)))
  else:
    if (len(list) != 0):
      list[len(list)-1] = list[len(list)-1].rstrip("\n")
  return list

def remove_empty_words(list):
  if (len(list) == 0):
    return list
  
  ri = 0
  for i in range(0, len(list)):
    if (list[ri] == ''):
      del list[ri]
    else:
      ri += 1

  if (len(list) == 1) and (list[0] == "\n"):
    del list[0]
    # print("remove_newline newsize = " + str(len(list)))
  else:
    if (len(list) != 0):
      list[len(list)-1] = list[len(list)-1].rstrip("\n")
  return list

def list_str_to_int(list, divide_by = None):
  print("list_str_to_int len = " + str(len(list)) + " 1st value = "+ str(list[0]) + "values:")
  print(list)
  if len(list) == 0:
    list.append(0)
  else:
    if divide_by == None:
      list = [int(i) for i in list]
    else:
      list = [int(i)/divide_by for i in list]
  return list

# Charge le fichier de bench "path" et retourne la liste de ce qui a été lu.
def load_file(path):
  global VERSION_ATTENDUE
  bench_list = []

  with open(path) as fp:
    version = fp.readline() # version du fichier actuel (doit être 106 et non plus 105)
    print("Version du fichier : {}".format(version))

    if (int(version) != VERSION_ATTENDUE):
      sys.exit("ERREUR, VERSION DU FICHIER NON COMPATIBLE : " + str(int(version)) + ".  VERSION ATTENDUE = " + str(VERSION_ATTENDUE))

    divide_factor = 1

    line = fp.readline()
    while line:

      res = {}

      res["array_length"] = int(line)

      res["raw_host_alloc"]      = list_str_to_int(remove_empty_words(remove_newline(fp.readline().split(" "))), divide_factor)
      res["raw_copy_and_kernel"] = list_str_to_int(remove_empty_words(remove_newline(fp.readline().split(" "))), divide_factor)
      res["raw_checksum"]        = list_str_to_int(remove_empty_words(remove_newline(fp.readline().split(" "))), divide_factor)

      res["no-outier_host_alloc"]      = pu.filter_outliers(res["raw_host_alloc"]     )
      res["no-outier_copy_and_kernel"] = pu.filter_outliers(res["raw_copy_and_kernel"])
      res["no-outier_checksum"]        = pu.filter_outliers(res["raw_checksum"]       )

      res["med_host_alloc"]      = stat.median(res["raw_host_alloc"]     )
      res["med_copy_and_kernel"] = stat.median(res["raw_copy_and_kernel"])
      res["med_checksum"]        = stat.median(res["raw_checksum"]       )

      bench_list.append(res)

      line = fp.readline()

  return bench_list

# dpcpp kwk_axpy_bench.cpp -o e.exe -O3 -std=c++20 -I/home/data_sync/academique/These/kiwaku_2023/kiwaku/include/ && ./e.exe 
# python3 plot_distribution.py 

# suffix = "_copy"

# suffix = "_heavy_sh$t_blop_cuda"

is_heavy = True

if is_heavy:
  suffix = "_heavy_blop"
  suffix_title = "transform: [](auto e) { return very_complicated_stuff; }"

  lcpu      = load_file("heavy/cpu_context" + suffix + ".txt")
  lsycl_cpu = load_file("heavy/sycl_context_cpu" + suffix + ".txt")
  lsycl_gpu = load_file("heavy/sycl_context_gpu" + suffix + ".txt")
else:
  suffix = "_copy_blop"
  suffix_title = "transform: [](auto e) { return e; }"

  lcpu      = load_file("copy/cpu_context" + suffix + ".txt")
  lsycl_cpu = load_file("copy/sycl_context_cpu" + suffix + ".txt")
  lsycl_gpu = load_file("copy/sycl_context_gpu" + suffix + ".txt")

# values1 = [lcpu["med_host_alloc"],      lomp["med_host_alloc"],      lsycl["med_host_alloc"]      ]
# values2 = [lcpu["med_copy_and_kernel"], lomp["med_copy_and_kernel"], lsycl["med_copy_and_kernel"] ]
# values3 = [lcpu["med_checksum"],        lomp["med_checksum"],        lsycl["med_checksum"]        ]

# array_length_list = []
# for item in lcpu:
#   array_length_list.append(item["array_length"])

array_length_list = pu.make_1D_list_every_1line_divided(lcpu, "array_length", 1, 1000000)


# Data
# groups = ['G1', 'G2', 'G3', 'G4', 'G5']
# values1 = [12, 19, 14, 27, 16]
# values2 = [21, 30, 15, 17, 20]

# fig, ax = plt.subplots()

color_host_alloc = "blue"
color_kernel = "red"
color_checksum = "green"

# Stacked bar chart
# plt.bar(groups, values1, label = "host alloc", color=color_host_alloc)
# plt.bar(groups, values2, bottom = values1, label = "copies + kernel", color=color_kernel)
# plt.bar(groups, values3, bottom = np.add(values1, values2), label = "checksum", color=color_checksum)


# Create a figure instance
# fig = plt.figure()

# Create an axes instance
# ax = fig.add_axes([0,0,1,1])

# Create the boxplot
# plt.violinplot(positions=[0, 1, 2], dataset=[values1, values2, values3])

display_speedup = False

min_value = 0
max_value = 0

if display_speedup:
  start_index = 0

  array_length_list_update = []
  for i in range(start_index, len(array_length_list)):
    array_length_list_update.append(array_length_list[i])

  # cpu_diff  = pu.make_absolute_list(lcpu , "med_copy_and_kernel")
  # sycl_diff = pu.make_absolute_list(lsycl, "med_copy_and_kernel")
  sycl_cpu = pu.make_div_list(lsycl_cpu, lcpu, start_index, 100, "med_copy_and_kernel")
  sycl_gpu = pu.make_div_list(lsycl_gpu, lcpu, start_index, 100, "med_copy_and_kernel")

  ref_list = []
  for i in sycl_cpu:
    ref_list.append(100)
  
  min_value = min(100, min(sycl_cpu), min(sycl_gpu))
  max_value = max(100, max(sycl_cpu), max(sycl_gpu))


  plt.plot  (range(1, len(sycl_cpu)+1), ref_list, color="grey", label="context::cpu - i3-4360 CPU @ 3.70GHz", linestyle="solid", linewidth=line_width)
  plt.plot  (range(1, len(sycl_cpu)+1), sycl_cpu, color="blue", label="context::sycl - i3-4360 CPU @ 3.70GHz", linestyle="dashdot", linewidth=line_width)
  plt.plot  (range(1, len(sycl_gpu)+1), sycl_gpu, color="green", label="context::sycl - NVIDIA GeForce GTX 1050", linestyle="dashed", linewidth=line_width)

  plt.title("Algo " + suffix_title)

  plt.xticks(range(1, len(array_length_list_update)+1), array_length_list_update)

  plt.xlabel('Array length, 10^6')
  plt.ylabel('Relative duration (%) (lower is better)')
else:

  # plot_diff = False
  # if plot_diff:
    # ldiff0 = pu.make_diff_list(compare_to_list, lorentz_standalone_list, plot_keyword)
  # else:
  cpu_list  = pu.make_absolute_list(lcpu , "med_copy_and_kernel")
  sycl_cpu = pu.make_absolute_list(lsycl_cpu, "med_copy_and_kernel")
  sycl_gpu = pu.make_absolute_list(lsycl_gpu, "med_copy_and_kernel")

  min_value = min(min(cpu_list), min(sycl_cpu), min(sycl_gpu))
  max_value = max(max(cpu_list), max(sycl_cpu), max(sycl_gpu))

  #  (opti matrix)
  plt.plot  (range(1, len(cpu_list)+1), cpu_list, color="grey", label="context::cpu - i3-4360 CPU @ 3.70GHz" , linestyle="solid"  , linewidth=line_width)
  plt.plot  (range(1, len(sycl_cpu)+1), sycl_cpu, color="blue", label="context::sycl - i3-4360 CPU @ 3.70GHz", linestyle="dashdot", linewidth=line_width)
  plt.plot  (range(1, len(sycl_gpu)+1), sycl_gpu, color="green", label="context::sycl - NVIDIA GeForce GTX 1050", linestyle="dashed", linewidth=line_width)

  plt.title("Algo " + suffix_title)

  plt.xticks(range(1, len(array_length_list)+1), array_length_list)

  plt.xlabel('Array length, 10^6')
  plt.ylabel('Elapsed time (ms) (lower is better)')


# Ascending number of particles (and imom), in millions (10^6)
#plt.ylim([-5, 100])
plt.legend()

plt.rcParams['grid.linestyle'] = "-"
plt.rcParams['grid.alpha'] = 0.15
plt.rcParams['grid.color'] = "black" ##cccccc
plt.grid(linewidth=line_width/10)

# global_drawn_x_variables_number+1
# plt.xticks(range(1, 6), x_list_curve_drawn) # = x_list_shared et x_list_acc

plt.ylim([- (max_value - min_value) * 0.1, max_value * 1.1])

# plt.savefig(out_fname, format='png') #, dpi=my_dpi)

plt.show()
print ("Hello World!")
