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
  bench_list = {}

  with open(path) as fp:
    version = fp.readline() # version du fichier actuel (doit être 106 et non plus 105)
    print("Version du fichier : {}".format(version))

    if (int(version) != VERSION_ATTENDUE):
      sys.exit("ERREUR, VERSION DU FICHIER NON COMPATIBLE : " + str(int(version)) + ".  VERSION ATTENDUE = " + str(VERSION_ATTENDUE))

    divide_factor = 1

    bench_list["raw_host_alloc"]      = list_str_to_int(remove_empty_words(remove_newline(fp.readline().split(" "))), divide_factor)
    bench_list["raw_copy_and_kernel"] = list_str_to_int(remove_empty_words(remove_newline(fp.readline().split(" "))), divide_factor)
    bench_list["raw_checksum"]        = list_str_to_int(remove_empty_words(remove_newline(fp.readline().split(" "))), divide_factor)

    bench_list["no-outier_host_alloc"]      = pu.filter_outliers(bench_list["raw_host_alloc"]     )
    bench_list["no-outier_copy_and_kernel"] = pu.filter_outliers(bench_list["raw_copy_and_kernel"])
    bench_list["no-outier_checksum"]        = pu.filter_outliers(bench_list["raw_checksum"]       )

    bench_list["med_host_alloc"]      = stat.median(bench_list["raw_host_alloc"]     )
    bench_list["med_copy_and_kernel"] = stat.median(bench_list["raw_copy_and_kernel"])
    bench_list["med_checksum"]        = stat.median(bench_list["raw_checksum"]       )


  return bench_list

# dpcpp kwk_axpy_bench.cpp -o e.exe -O3 -std=c++20 -I/home/data_sync/academique/These/kiwaku_2023/kiwaku/include/ && ./e.exe 
# python3 plot_distribution.py 

lcpu  = load_file("kwk_buffers_cpu.txt")
lomp  = load_file("kwk_buffers_omp.txt")
lsycl = load_file("kwk_buffers_sycl.txt")

values1 = [lcpu["med_host_alloc"],      lomp["med_host_alloc"],      lsycl["med_host_alloc"]      ]
values2 = [lcpu["med_copy_and_kernel"], lomp["med_copy_and_kernel"], lsycl["med_copy_and_kernel"] ]
values3 = [lcpu["med_checksum"],        lomp["med_checksum"],        lsycl["med_checksum"]        ]
# values2 = [med1_copy_and_kernel, med2_copy_and_kernel, med3_copy_and_kernel]
# values3 = [med1_checksum,        med2_checksum,        med3_checksum       ]


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

# pu.draw_violin_plot("black", [values1, values2, values3])
ds = [lcpu["raw_host_alloc"], lomp["raw_host_alloc"], lsycl["raw_host_alloc"]]
pu.draw_violin_plot_pos(color_face=color_host_alloc, color_edge="black", dataset_=ds, positions_=[0, 1, 2])


l1 = [x + lcpu ["med_host_alloc"] for x in lcpu ["raw_copy_and_kernel"]]
l2 = [x + lomp ["med_host_alloc"] for x in lomp ["raw_copy_and_kernel"]]
l3 = [x + lsycl["med_host_alloc"] for x in lsycl["raw_copy_and_kernel"]]
ds = [l1, l2, l3]
pu.draw_violin_plot_pos(color_face=color_kernel, color_edge="black", dataset_=ds, positions_=[0, 1, 2])


l1 = [x + lcpu ["med_host_alloc"] + lcpu ["med_copy_and_kernel"] for x in lcpu ["raw_checksum"]]
l2 = [x + lomp ["med_host_alloc"] + lomp ["med_copy_and_kernel"] for x in lomp ["raw_checksum"]]
l3 = [x + lsycl["med_host_alloc"] + lsycl["med_copy_and_kernel"] for x in lsycl["raw_checksum"]]
ds = [l1, l2, l3]
pu.draw_violin_plot_pos(color_face=color_checksum, color_edge="black", dataset_=ds, positions_=[0, 1, 2])



plt.xlabel('Number of particles')
# Ascending number of particles (and imom), in millions (10^6)
#plt.ylim([-5, 100])
plt.legend()
# global_drawn_x_variables_number+1
# plt.xticks(range(1, 6), x_list_curve_drawn) # = x_list_shared et x_list_acc

# plt.ylim([0, 113])

# plt.savefig(out_fname, format='png') #, dpi=my_dpi)

plt.show()
print ("Hello World!")
