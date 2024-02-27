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

# image_ratio = 640 / 480
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

# if is_heavy:
#   suffix = "_heavy_blop"
#   suffix_title = "transform: [](auto e) { return very_complicated_stuff; }"

#   l_context_cpu        = load_file("heavy/cpu_context" + suffix + ".txt")
#   l_native_cpu = load_file("heavy/cpu_native" + suffix + ".txt")
#   l_context_sycl_cpu   = load_file("heavy/sycl_context_cpu" + suffix + ".txt")
#   l_context_sycl_gpu   = load_file("heavy/sycl_context_gpu" + suffix + ".txt")
# else:
#   suffix = "_copy_blop"
#   suffix_title = "transform: [](auto e) { return e; }"

#   l_context_cpu        = load_file("copy/cpu_context" + suffix + ".txt")
#   l_native_cpu = load_file("copy/cpu_native" + suffix + ".txt")
#   l_context_sycl_cpu   = load_file("copy/sycl_context_cpu" + suffix + ".txt")
#   l_context_sycl_gpu   = load_file("copy/sycl_context_gpu" + suffix + ".txt")

if is_heavy:
  # suffix_title = "LEGEND - transform: [](auto e) { return very_complicated_stuff; }"

  # l_context_cpu        = load_file("transform/transform_parsys-legend_heavy_isc9_rp7_cpu_context.txt")
  # l_native_cpu = load_file("transform/transform_parsys-legend_heavy_isc9_rp7_cpu_native.txt")
  # l_context_sycl_cpu   = load_file("transform/transform_parsys-legend_heavy_isc9_rp7_sycl_cpu.txt")

  # l_native_cpu         = load_file("transform/transform_parsys-legend_heavy_isc6_rp7_cpu_native.txt")
  l_context_cpu        = load_file("transform/transform_parsys-legend_heavy_isc6_rp7_cpu_context.txt")
  l_context_sycl_cpu   = load_file("transform/transform_parsys-legend_heavy_isc6_rp7_sycl_cpu.txt")
  l_context_sycl_gpu   = load_file("transform/transform4_parsys-legend_heavy_isc6_rp7_sycl_gpu.txt")
                                              #  transform4_parsys-legend_heavy_isc6_rp7_sycl_gpu

  # l_native_cpu         = load_file("full_without_nd_range/transform3_parsys-legend_heavy_isc6_rp7_cpu_native.txt")
  # l_context_cpu        = load_file("full_without_nd_range/transform3_parsys-legend_heavy_isc6_rp7_cpu_context.txt")
  # l_context_sycl_cpu   = load_file("full_without_nd_range/transform3_parsys-legend_heavy_isc6_rp7_sycl_cpu.txt")
  # l_context_sycl_gpu   = load_file("full_without_nd_range/transform3_parsys-legend_heavy_isc6_rp7_sycl_gpu.txt")  
  
  # l_native_cpu         = load_file("full_with_nd_range/transform3_parsys-legend_heavy_isc6_rp7_cpu_native.txt")
  # l_context_cpu        = load_file("full_with_nd_range/transform3_parsys-legend_heavy_isc1_rp3_ndr0_cpu_context_ndr.txt")
  # l_context_sycl_cpu   = load_file("full_with_nd_range/transform3_parsys-legend_heavy_isc1_rp3_ndr0_sycl_cpu_ndr.txt")
  # l_context_sycl_gpu   = load_file("full_with_nd_range/transform3_parsys-legend_heavy_isc1_rp3_ndr0_sycl_gpu_ndr.txt")


  # l_context_cpu        = load_file("transform_ndr1024/transform3_parsys-legend_heavy_isc4_rp5_cpu_context.txt")
  # l_context_sycl_cpu   = load_file("transform_ndr1024/transform3_parsys-legend_heavy_isc4_rp5_sycl_cpu.txt")
  # l_context_sycl_gpu   = load_file("transform_ndr1024/transform3_parsys-legend_heavy_isc4_rp5_sycl_gpu.txt")
else:
  suffix_title = "LEGEND - transform: [](auto e) { return e; }"

  l_native_cpu = load_file("transform/transform_parsys-legend_copy_isc6_rp7_cpu_native.txt")
  l_context_cpu        = load_file("transform/transform_parsys-legend_copy_isc6_rp7_cpu_context.txt")
  l_context_sycl_cpu   = load_file("transform/transform_parsys-legend_copy_isc6_rp7_sycl_cpu.txt")
  l_context_sycl_gpu   = load_file("transform/transform_parsys-legend_copy_isc6_rp7_sycl_gpu.txt")

# array_length_list = pu.make_1D_list_every_1line(l_context_cpu, "array_length", 1)
array_length_list = pu.make_1D_list_every_1line_divided(l_context_cpu, "array_length", 1, 1000000)

color_host_alloc = "blue"
color_kernel = "red"
color_checksum = "green"

display_speedup = False

min_value = 0
max_value = 0

cpu_name = "AMD Ryzen 9 5950X"
gpu_name = "NVIDIA GeForce RTX 2080 Ti"
suffix_title = cpu_name

# cpu_name = "i3-4360 CPU @ 3.70GHz"
# gpu_name = "NVIDIA GeForce GTX 1050"

display_context_sycl_cpu = True
display_context_sycl_gpu = False
display_context_cpu = True
display_native_cpu = False

def update_min_max(condition, list, min_value, max_value, initialized):
  if condition:
    if initialized:
      min_value = min(min_value, min(list))
      max_value = max(max_value, max(list))
    else:
      min_value = min(list)
      max_value = max(list)
      initialized = True
  return (min_value, max_value, initialized)


if display_speedup:
  start_index = 0

  array_length_list_update = []
  for i in range(start_index, len(array_length_list)):
    array_length_list_update.append(array_length_list[i])

  # relative_list = l_native_cpu
  # relative_name = "native cpu - " + cpu_name

  # relative_list = l_context_cpu
  # relative_name = "cpu context - " + cpu_name

  relative_list = l_context_sycl_gpu
  relative_name = "context::sycl - " + gpu_name

  # relative_to = "context_sycl_cpu" # context_sycl_cpu context_sycl_gpu context_cpu native_cpu  
  # relative_list = l_context_sycl_gpu

  # cpu_diff  = pu.make_absolute_list(l_context_cpu , "med_copy_and_kernel")
  # sycl_diff = pu.make_absolute_list(lsycl, "med_copy_and_kernel")
  sycl_cpu   = pu.make_div_list(l_context_sycl_cpu , relative_list, start_index, 100, "med_copy_and_kernel")
  sycl_gpu   = pu.make_div_list(l_context_sycl_gpu , relative_list, start_index, 100, "med_copy_and_kernel")
  ctx_cpu    = pu.make_div_list(l_context_cpu      , relative_list, start_index, 100, "med_copy_and_kernel")

  ref_list = []
  for i in relative_list:
    ref_list.append(100)

  # min_value = min(100, min(sycl_cpu))#, min(sycl_gpu), min(ctx_cpu))
  # max_value = max(100, max(sycl_cpu))#, max(sycl_gpu), max(ctx_cpu))

  (min_value, max_value, initialized) = (False, 0, 0)
  (min_value, max_value, initialized) = update_min_max(display_context_sycl_cpu, sycl_cpu, min_value, max_value, initialized)
  (min_value, max_value, initialized) = update_min_max(display_context_sycl_gpu, sycl_gpu, min_value, max_value, initialized)
  (min_value, max_value, initialized) = update_min_max(display_context_cpu, ctx_cpu, min_value, max_value, initialized)
  min_value = min(100, min_value)
  max_value = max(100, max_value)


  # plt.plot  (range(1, len(ref_list)+1), ref_list, color="green", label=relative_name, linestyle="solid", linewidth=line_width)
  plt.plot  (range(1, len(ref_list)+1), ref_list, color="grey", label=relative_name, linestyle="solid", linewidth=line_width)
  # plt.plot  (range(1, len(ctx_cpu)+1), ctx_cpu  , color="black", label="context::cpu - " + cpu_name, linestyle="dotted", linewidth=line_width)
  plt.plot  (range(1, len(sycl_cpu)+1), sycl_cpu, color="blue", label="context::sycl - " + cpu_name, linestyle="dashdot", linewidth=line_width)
  plt.plot  (range(1, len(sycl_gpu)+1), sycl_gpu, color="green", label="context::sycl - " + gpu_name, linestyle="dashed", linewidth=line_width)

  plt.title("Algo " + suffix_title)

  plt.xticks(range(1, len(array_length_list_update)+1), array_length_list_update)

  plt.xlabel('Array length, 10^6')
  plt.ylabel('Relative duration (%)')
else:

  # plot_diff = False
  # if plot_diff:
    # ldiff0 = pu.make_diff_list(compare_to_list, lorentz_standalone_list, plot_keyword)
  # else:
  cpu_list   = pu.make_absolute_list_divided(l_context_cpu      , "med_copy_and_kernel", 1000)
  sycl_cpu   = pu.make_absolute_list_divided(l_context_sycl_cpu , "med_copy_and_kernel", 1000)
  sycl_gpu   = pu.make_absolute_list_divided(l_context_sycl_gpu , "med_copy_and_kernel", 1000)
  # native_cpu = pu.make_absolute_list(l_native_cpu       , "med_copy_and_kernel")

  (min_value, max_value, initialized) = (False, 0, 0)
  (min_value, max_value, initialized) = update_min_max(display_context_sycl_cpu , sycl_cpu  , min_value, max_value, initialized)
  print(min_value)
  print(max_value)
  print(initialized)
  (min_value, max_value, initialized) = update_min_max(display_context_sycl_gpu , sycl_gpu  , min_value, max_value, initialized)
  (min_value, max_value, initialized) = update_min_max(display_context_cpu      , cpu_list  , min_value, max_value, initialized)
  # (min_value, max_value, initialized) = update_min_max(display_native_cpu       , native_cpu, min_value, max_value, initialized)

  # min_value = min(min(cpu_list), min(sycl_cpu), min(sycl_gpu), min(native_cpu))
  # max_value = max(max(cpu_list), max(sycl_cpu), max(sycl_gpu), max(native_cpu))

  # if display_native_cpu:
  #   plt.plot  (range(1, len(native_cpu)+1), native_cpu, color="grey", label="native cpu - " + cpu_name , linestyle="solid"  , linewidth=line_width)
  if display_context_cpu:
    plt.plot  (range(1, len(cpu_list)+1), cpu_list, color="grey", label="kwk::context::cpu" , linestyle="solid"  , linewidth=line_width)
  if display_context_sycl_cpu:
    plt.plot  (range(1, len(sycl_cpu)+1), sycl_cpu, color="blue", label="kwk::context::sycl", linestyle="dashdot", linewidth=line_width)
  if display_context_sycl_gpu:
    plt.plot  (range(1, len(sycl_gpu)+1), sycl_gpu, color="green", label="kwk::context::sycl", linestyle="dashed", linewidth=line_width)

  plt.title(suffix_title)

  plt.xticks(range(1, len(array_length_list)+1), array_length_list)

  plt.xlabel('Array length, 10^6')
  plt.ylabel('Elapsed time (s)')


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
