import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import sys

def Parse(fname1, fname2, fname3, df1, df2, df3, Function, metric, plot_type, save):
    for f in Function:
      plt.figure()
      copy1 = df1.loc[df1['Function'] == f]
      copy2 = df2.loc[df2['Function'] == f]
      copy3 = df3.loc[df3['Function'] == f]
      opblas = 'BLAS' + f[0:4]
      blas = df1.loc[df1['Function'] == opblas]

      plot(copy1, copy2, copy3, blas, f, metric, save)

      if save:
        plt.savefig('compare/Benchmark_compare' + '_' + f)
      else:
        plt.show()

      
def plot(df1, df2, df3, blas, Function, metric, save):
  plt.semilogx(df1['Size(Bytes)'], df1[metric])
  plt.semilogx(df2['Size(Bytes)'], df2[metric])
  plt.semilogx(df3['Size(Bytes)'], df3[metric])
  # plt.semilogx(blas['Size(Bytes)'], blas[metric])

  plt.xlabel('FLOP')
  plt.ylabel('GFLOPs')
  plt.title('Benchmark compare std/kwk' + " : " + Function)
  metric1 = ['std']
  metric2 = ['kwk table']
  metric3 = ['kwk view']
  # metric4 = ['BLAS']
  plt.legend(metric1 + metric2 + metric3 )


def main():
  n_arg = len(sys.argv)

  functions = ['GEMV', 'GEMM', 'GEMMSmart']
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
    fname3 = sys.argv[3]
    df1 = pd.read_csv(fname1, sep=";")
    df2 = pd.read_csv(fname2, sep=";")
    df3 = pd.read_csv(fname3, sep=';')
    Parse(fname1, fname2, fname3, df1, df2, df3, functions, metric, plot_type, save)


   
if __name__ == "__main__":
  sys.exit(main())

