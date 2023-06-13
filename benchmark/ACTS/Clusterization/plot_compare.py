import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import sys
      
def plot(df1, df2, metric, save):
  plt.figure()
  plt.plot(df1['Density(1/1000)'], df1[metric])
  plt.plot(df1['Density(1/1000)'], df2[metric])
  size = df1['Size(N*N)'][1]
  plt.xlabel('Percolation (1/10000)')
  plt.ylabel('Cycles/cell')
  plt.title('Benchmark compare CCL std/kwk | N :' + str(size))
  metric1 = ['std']
  metric2 = ['kwk']
  plt.legend(metric1 + metric2)

  if save:
    plt.savefig('compare/Benchmark_compare' + '_' + str(size))
  else:
    plt.show()


def main():
  n_arg = len(sys.argv)
  metric = ['Cycles/cell']
  save=True

  if(n_arg < 2):
    print("Please input the file\n")
    return -1
  else:
    fname1 = sys.argv[1]
    fname2 = sys.argv[2]
    df1 = pd.read_csv(fname1, sep=";")
    df2 = pd.read_csv(fname2, sep=";")
    plot(df1, df2, metric, save)


   
if __name__ == "__main__":
  sys.exit(main())

