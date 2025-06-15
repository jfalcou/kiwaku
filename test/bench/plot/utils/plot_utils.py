
import matplotlib.pyplot as plt
import numpy as np
import statistics as stat
import sys
import math



def make_1D_list(bench_list, field_name):
  res = []
  ind = 0
  for i in bench_list:
    res.append(i[field_name]) # field_name = "elapsed_time"
    ind += 1
  return res



# def make_1D_list_every(bench_list, field_name, take_every = 0):
#   res = []
#   ind = 0
#   for i in bench_list:
#     if (ind % take_every == 0):
#       res.append(int(i[field_name] / 1000000)) # field_name = "elapsed_time"
#       # res.append(int(i[field_name] / 1000000)) # field_name = "elapsed_time"
#     else:
#       res.append("")
#     ind += 1
#   return res



def make_1D_list_every_3lines(bench_list, field_name1, field_name2, take_every = 0):
  res = []
  ind = 0
  for i in bench_list:
    if (ind % take_every == 0):
      v1 = i[field_name1]
      v2 = i[field_name2]
      tot = v1 * v2
      res.append(str(v1) + "\n" + str(v2) + "\n" + str(int(tot/1000000)) + "M") # field_name = "elapsed_time"
      # res.append(int(i[field_name] / 1000000)) # field_name = "elapsed_time"
    else:
      res.append("")
    ind += 1
  return res

def make_1D_list_every_1line(bench_list, field_name1, take_every = 0):
  res = []
  ind = 0
  for i in bench_list:
    if (ind % take_every == 0):
      v1 = i[field_name1]
      res.append(str(v1))
    else:
      res.append("")
    ind += 1
  return res

def make_1D_list_every_1line_divided(bench_list, field_name1, take_every, multiply_by = 1):
  res = []
  ind = 0
  for i in bench_list:
    if (ind % take_every == 0):
      v1 = i[field_name1] * multiply_by
      res.append(str(v1))
    else:
      res.append("")
    ind += 1
  return res



def make_diff_list(l1, l2, keyword = None):
  res  = []
  if (len(l1) != len(l2)):
    sys.exit("ERROR @make_diff_list. Number of points not the same in both lists.")

  if keyword == None:
    keyword = "elapsed_time"

  for i in range(0, len(l1)):
    v1 = l1[i][keyword]
    v2 = l2[i][keyword]
    diff = (100 * v2 / v1) # / l1[i]["ptotal"]
    # diff = 100 + (v2 - v1) / ((abs(v1) + abs(v2))/2) # Abs not necessary since v1 and v2 are positive time values 
    res.append(diff) # Percentage
  return res

# l1 / l2 from start_index (to avoid huge initial values)
def make_div_list(numerator_l, divisor_l, start_index, multiply_by, keyword = None):
  res  = []
  max_len = min(len(numerator_l), len(divisor_l))
  # if (len(numerator_l) != len(divisor_l)):
  #   sys.exit("ERROR @make_div_list. Number of points not the same in both lists.")

  if keyword == None:
    keyword = "elapsed_time"

  for i in range(start_index, max_len):
    n = numerator_l[i][keyword]
    d = divisor_l[i][keyword]
    diff = (n / d) * multiply_by
    res.append(diff)
  return res


def make_absolute_list(l1, keyword = None):
  if keyword == None:
    keyword = "elapsed_time"
  res  = []
  for i in range(0, len(l1)):
    res.append(l1[i][keyword])
  return res

def make_absolute_list_divided(l1, keyword, multiply_by):
  if keyword == None:
    keyword = "elapsed_time"
  res  = []
  for i in range(0, len(l1)):
    res.append(float(l1[i][keyword]) * multiply_by)
  return res

# def make_absolute_list(l1):
#   return make_absolute_list(l1, "elapsed_time")




def check_same_results(l1, l2):
  if (len(l1) != len(l2)):
    sys.exit("ERROR @check_same_results. Number of points not the same in both lists.")

  for i in range(0, len(l1)):
    v1 = l1[i]["check_string"]
    v2 = l2[i]["check_string"]
    if (v1 != v2):
      sys.exit("ERROR @check_same_results. Value at position "
      + str(i) + " differs: \n    " + str(v1) + " \n != " +str(v2))
  print("OK check_same_results, " + str(len(l1)) + " value(s) checked.")
  return


def draw_violin_plot(color, y_list):
  c = color
  bp_ = plt.violinplot(y_list, showextrema=False)
  for pc in bp_['bodies']:
      pc.set_facecolor(c)
      pc.set_edgecolor(c) # #D43F3A black
      pc.set_alpha(1)

def draw_violin_plot_pos(color_face, color_edge, dataset_, positions_):
  bp_ = plt.violinplot(dataset=dataset_, showextrema=False, positions=positions_)
  for pc in bp_['bodies']:
      pc.set_facecolor(color_face)
      pc.set_linewidth(4)
      pc.set_edgecolor(color_edge) # #D43F3A black
      pc.set_alpha(1)


def draw_violin_plot_pos_ext(color_face, color_edge, dataset_, positions_):
  bw_method_=0.04
  bp_ = plt.violinplot(dataset=dataset_, showextrema=False, positions=positions_,
                        showmedians=False, bw_method=bw_method_, points=600, widths=0.7, showmeans=False)
  for pc in bp_['bodies']:
      pc.set_facecolor(color_face)
      pc.set_linewidth(4)
      pc.set_edgecolor(color_edge) # #D43F3A black
      pc.set_alpha(1)

      # data2, [position], points=600, widths=0.7, showmeans=False,
  #                               showextrema=True, showmedians=True, bw_method=bw_method_)


# Old version, kept for direct comparison with the newer version
def filter_outliers_deprecated(list_arg):

  res = list_arg.copy()

  if len(res) <= 2:
    return res

  n_quantiles = 10 ## must be >= 4
  
  q = stat.quantiles(res, n=n_quantiles)
  print("quartiles:")
  print(q)

  index = 0
  for i in range(len(res)):
    elem = res[index]
    print(elem)
    print("q[0] = " + str(q[0]) + "  q[n_quantiles - 2] = " + str(q[n_quantiles - 2])) 
    if (elem < q[0]) or (elem > q[n_quantiles - 2]):
      res.pop(index)
    else:
      index += 1

  return res

# New version, this is a somewhat inadequate but suits the basic expected benchmark time distribution
def filter_outliers(list_arg):
  res = list_arg.copy()
  if len(res) <= 2:
    return res

  med = stat.median(list_arg)
  index = 0
  for i in range(len(res)):
    elem = res[index]
    distance = abs(elem - med)
    # No more that 20% variation allowed
    if (distance > abs(med * 0.2)):
      res.pop(index)
    else:
      index += 1

  return res



def remove_newline(list):
  if (len(list) == 1) and (list[0] == "\n"):
    del list[0]
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
  else:
    if (len(list) != 0):
      list[len(list)-1] = list[len(list)-1].rstrip("\n")
  return list

def list_str_to_int(list, multiply_by = None):
  # print("list_str_to_int len = " + str(len(list)) + " 1st value = "+ str(list[0]) + "values:")
  # print(list)
  if len(list) == 0:
    list.append(0)
  else:
    if multiply_by == None:
      list = [round(int(i)) for i in list]
    else:
      list = [round(int(i) * multiply_by) for i in list]
  # print(list)
  return list