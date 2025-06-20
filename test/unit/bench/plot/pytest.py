
import sys
import argparse


import utils.plot_utils as pu

lst = [5.194304, 2.194304, 4.194304, 4.194304, 4.194304, 4.194304, 4.194304, 4.194304, 4.194304, 4.194304]
lst_filtered = pu.filter_outliers(lst)

print("lst =")
print(lst)
print("lst_filtered =")
print(lst_filtered)

# print(len(sys.argv))

# print(sys.argv[0])

# parser = argparse.ArgumentParser(
#                     prog='Kiwaku benchmark plots',
#                     description='',
#                     epilog='Text at the bottom of help')

# parser.add_argument('filepath')           # positional argument
# # parser.add_argument('-c', '--count')      # option that takes a value
# parser.add_argument('-s', '--only_save_image',
#                     action='store_true')  # on/off flag

# args = parser.parse_args()
# print(args.filepath, args.only_save_image)