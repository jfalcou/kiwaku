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

use_acts_field = True

plot_diff = True # Plot values relative to covfie, or plot absolute values

plot_cycles_instead_of_time = False 

per_particle_values = True # Divide the values by the number of particles and steps 


computer_name = "blop" # aussi utilisé pour le nom du fichier d'entrée
str_min_iteration_count = "10" # added to plot title only


# ============= Gestion de la taille
my_dpi = 96
# output_image_name = "no_name"
field_type_name = "" # InterpolateNN_LayoutStride

if use_acts_field:
  field_type_name = "acts-field"
else:
  field_type_name = "constant-field"

output_image_name = field_type_name # InterpolateNN_LayoutStride
output_image_ver  = "v1"


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

def list_str_to_int(list, divide_by = None):
  # print("list_str_to_int len = " + str(len(list)) + " 1st value = "+ str(list[0]))
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

    line = fp.readline()

    while line:
      words = remove_newline(line.split(" "))
      # words[len(words)-1] = words[len(words)-1].rstrip("\n")
      # words.remove('\n')
      # print(words)

      # Autant de fois qu'il y a d'évènements (nouvelle ligne) :
      # v1:
      # particles | steps | imom | repeat_count | elapsed_time | check_string
      
      # v2:
      # particles | steps | imom | repeat_count | check_string
      # elapsed_time1 | ... | elapsed_timeN

      header = {} # dictionnaire vide
      header["particles"]     = int(words[0])
      header["steps"]         = int(words[1])
      header["imom"]          = int(words[2])
      header["repeat_count"]  = int(words[3])
      header["check_string"]  = words[4]
      header["ptotal"]        = header["particles"] * header["steps"]

      # times_l  = []
      # cycles_l = []
      divide_factor = 1
      if per_particle_values:
        if plot_cycles_instead_of_time:
          divide_factor = header["ptotal"]
        else:
          divide_factor = header["ptotal"] / 1000
      else:
        if plot_cycles_instead_of_time:
          divide_factor = 1e9
        else:
          divide_factor = 1e6

      times_int   = pu.filter_outliers(list_str_to_int(remove_newline(fp.readline().split(" ")), divide_factor))
      cycles_int  = pu.filter_outliers(list_str_to_int(remove_newline(fp.readline().split(" ")), divide_factor))

      header["times_l"]   = times_int
      header["cycles_l"]  = cycles_int

      header["elapsed_time"]  = stat.median(times_int)
      header["cycle"]         = stat.median(cycles_int)
      

      # print(header["times_l"])

      bench_list.append(header)
  
      # Lecture de la prochaine ligne
      line = fp.readline()

  return bench_list




# Constant field: blop-debian11
# lorentz_covfie_list     = load_file("data/lorentz-euler_covfie_blop-debian11_2023-04-17_4096-65536_1024-2048_128-131072_constant-field.txt")
# lorentz_standalone_list = load_file("data/lorentz_standalone_blop-debian11_2023-04-16_4096-65536_1024-2048_128-131072_constant-field.txt")
# lorentz_kiwaku_list     = load_file("data/lorentz_kiwaku_blop-debian11_2023-04-16_4096-65536_1024-2048_128-131072_constant-field.txt")

if use_acts_field:
                                                    
  lorentz_standalone_list = load_file("data/d9_blop/lorentz_opti_sclock_it10_standalone_blop-debian11_512-65536_1024_128_acts-field.txt")
  lorentz_kiwaku_list     = load_file("data/d9_blop/lorentz_opti_sclock_it10_kiwaku_blop-debian11_512-65536_1024_128_acts-field.txt")



# TEMPORARILY DISABLE ERROR CHECKING
# pu.check_same_results(lorentz_standalone_list, lorentz_kiwaku_list)
# Error with lorentz_kiwaku_list, so it seems.
# pu.check_same_results(lorentz_standalone_list, lorentz_covfie_list)

# external_1D = make_1D_list(lorentz_external_list, "elapsed_time")

# ptotal_1D   = pu.make_1D_list_every_1line(lorentz_standalone_list, "particles", "steps", 1)
ptotal_1D   = pu.make_1D_list_every_1line(lorentz_standalone_list, "particles", 1)

def make_violin_plot_list(bench_list, keyword, compare_to_list = None):
  l2_res = []

  for header in bench_list:
    l2_res.append(header[keyword].copy())
  
  if compare_to_list != None:
    l2_compare = []

    for header in compare_to_list:
      l2_compare.append(header[keyword]) # <- ! not a copy !
    
    for il in range(0, len(l2_res)):
      l1_res = l2_res[il]
      l1_compare = l2_compare[il]
      compare_value = stat.median(l1_compare)

      for i in range(0, len(l1_res)):
        old = l1_res[i]
        l1_res[i] = 100 * l1_res[i] / compare_value # l1_compare[i]
      l2_res[il] = l1_res

  return l2_res


compare_to_list = None
if plot_diff:
  compare_to_list = lorentz_standalone_list

if plot_cycles_instead_of_time:
  violin_keyword = "cycles_l"
  plot_keyword = "cycle"
else:
  violin_keyword = "times_l"
  plot_keyword = "elapsed_time"

# pu.draw_violin_plot("grey", make_violin_plot_list(lorentz_standalone_list, violin_keyword, compare_to_list))
# pu.draw_violin_plot("blue", make_violin_plot_list(lorentz_kiwaku_list, violin_keyword, compare_to_list))


if plot_diff:
  ldiff0 = pu.make_diff_list(compare_to_list, lorentz_standalone_list, plot_keyword)
  ldiff2 = pu.make_diff_list(compare_to_list, lorentz_kiwaku_list, plot_keyword)
else:
  ldiff0 = pu.make_absolute_list(lorentz_standalone_list, plot_keyword)
  ldiff2 = pu.make_absolute_list(lorentz_kiwaku_list, plot_keyword)

#  (opti matrix)
plt.plot  (range(1, len(ldiff0)+1), ldiff0, color="grey", label="Standalone", linestyle="dashdot", linewidth=line_width)
plt.plot  (range(1, len(ldiff2)+1), ldiff2, color="blue", label="Kiwaku", linestyle="solid", linewidth=line_width)


plt.rcParams['grid.linestyle'] = "-"
plt.rcParams['grid.alpha'] = 0.15
plt.rcParams['grid.color'] = "black" ##cccccc
plt.grid(linewidth=line_width/20)

plt.xticks(range(1, len(ldiff0)+1), ptotal_1D)

unit_name = ""
if plot_diff:
  unit_name = "relative "
else:
  unit_name = ""

if plot_cycles_instead_of_time:
  unit_name += "cpu cycles"
else:
  unit_name += "time"

plt.title("Lorentz-Euler - " + computer_name + " - " + field_type_name + " - " + unit_name + " - iter/pt: " + str_min_iteration_count)

pdiff_str = ""
if plot_diff:
  pdiff_str = "diff"
else:
  pdiff_str = "absolute"

unit_name_file = unit_name.replace(" ", "_")
if per_particle_values:
  unit_name_file += "_per-particle"
out_fname = "2023-05-01_" + computer_name + "_" + field_type_name + "_" + unit_name_file + "_" + pdiff_str + ".png"


# plt.ylabel('Elapsed time (µs)')
if plot_diff:
  plt.ylabel('Relative performance (%), lower is better')
else:
  if per_particle_values:
    if plot_cycles_instead_of_time:
      plt.ylabel('CPU cycles per (particle*steps)')
    else:
      plt.ylabel('Elapsed time (nanoseconds) per (particle*steps)')
  else:
    if plot_cycles_instead_of_time:
      plt.ylabel('CPU cycles in billions (1e9)')
    else:
      plt.ylabel('Elapsed time in seconds')




plt.xlabel('Number of particles')
# Ascending number of particles (and imom), in millions (10^6)
#plt.ylim([-5, 100])
plt.legend()
# global_drawn_x_variables_number+1
# plt.xticks(range(1, 6), x_list_curve_drawn) # = x_list_shared et x_list_acc

plt.ylim([0, 113])

plt.savefig(out_fname, format='png') #, dpi=my_dpi)

plt.show()
print ("Hello World!")
