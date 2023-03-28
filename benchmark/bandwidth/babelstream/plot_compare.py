import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import sys

def Parse(fname1, fname2, df1, df2, Function, metric, plot_type, save):
    for f in Function:
      plt.figure()
      copy1 = df1.loc[df1['Function'] == f]
      copy2 = df2.loc[df2['Function'] == f]
      plot(fname1, fname2, copy1, copy2, f, metric, save)

      if save:
        plt.savefig('Benchmark_compare' + '_' + f)
      else:
        plt.show()

      
def plot(fname1, fname2, df1, df2, Function, metric, save):
  plt.semilogx(df1['Size(Bytes)'], df1[metric])
  plt.semilogx(df2['Size(Bytes)'], df2[metric])
  plt.xlabel('Size (Bytes)')
  plt.ylabel('GBytes/s')
  plt.title('Benchmark compare std/kwk' + " : " + Function)
  metric1 = ['std ' + m for m in metric]
  metric2 = ['kwk ' + m for m in metric]
  plt.legend(metric1 + metric2)


def main():
  n_arg = len(sys.argv)

  functions = ['Copy', 'Mul', 'Add','Triad','Dot','Nstream']
  # metric = ['Mean Babel(GBytes/sec)','Mean Nano(GBytes/sec)','Median Nano(GBytes/sec)']
  metric = ['Median Nano(GBytes/sec)']
  plot_type = ['plot'] # 'bar'
  save=True

  if(n_arg < 2):
    print("Please input the file\n")
    return -1
  else:
    fname1 = sys.argv[1]
    fname2 = sys.argv[2]
    df1 = pd.read_csv(fname1, sep=";")
    df2 = pd.read_csv(fname2, sep=";")
    Parse(fname1, fname2, df1, df2, functions, metric, plot_type, save)


   
if __name__ == "__main__":
  sys.exit(main())

