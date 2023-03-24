import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import sys

def Parse(fname, df, Function, metric):
  copy = df.loc[df['Function'] == Function]
  plot(fname, copy, Function, metric)

def plot(fname, df, Function, metric):
  plt.figure()
  plt.semilogx(df['Size(Bytes)'], df[metric])
  plt.xlabel('Size (Bytes)')
  plt.ylabel('GBytes/s')
  plt.title(fname.split(sep='.')[0]+ "_" + Function)
  plt.legend(metric)
  plt.savefig(fname.split(sep='.')[0]+ "_" + Function)
  # plt.show()

def main():
  n_arg = len(sys.argv)

  if(n_arg < 2):
    print("Please input the file\n")
    return -1
  else:
    for Arg in sys.argv[1:]:
      fname = Arg

      df = pd.read_csv(fname, sep=";")
      Parse(fname, df, 'Copy', ['Mean Babel(GBytes/sec)','Mean Nano(GBytes/sec)'])
      Parse(fname, df, 'Mul', ['Mean Babel(GBytes/sec)','Mean Nano(GBytes/sec)'])
      Parse(fname, df, 'Add', ['Mean Babel(GBytes/sec)','Mean Nano(GBytes/sec)'])
      Parse(fname, df, 'Triad', ['Mean Babel(GBytes/sec)','Mean Nano(GBytes/sec)'])
      Parse(fname, df, 'Dot', ['Mean Babel(GBytes/sec)','Mean Nano(GBytes/sec)'])
      Parse(fname, df, 'Nstream', ['Mean Babel(GBytes/sec)','Mean Nano(GBytes/sec)'])
   
if __name__ == "__main__":
  sys.exit(main())

