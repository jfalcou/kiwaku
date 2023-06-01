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
      plot(fname1,copy1, copy2, copy3, f, metric, save)

      if save:
        if "float" in fname1:
          plt.savefig('Benchmark_compare' + '_' + f + "_float")
        else :
          plt.savefig('Benchmark_compare' + '_' + f + "_double")

      else:
        plt.show()

      
def plot(fname1,df1, df2, df3, Function, metric, save):
  plt.loglog(df1['Size(Bytes)'], df1[metric])
  plt.loglog(df2['Size(Bytes)'], df2[metric])
  plt.loglog(df3['Size(Bytes)'], df3[metric])
  plt.xlabel('Size (Bytes)')
  plt.ylabel('Cycles')
  if "float" in fname1:
    plt.title('Benchmark compare std/kwk' + " : " + Function + " float")
  else:
    plt.title('Benchmark compare std/kwk' + " : " + Function + " double")

  metric1 = ['std ' + m for m in metric]
  metric2 = ['kwk table ' + m for m in metric]
  metric3 = ['kwk view ' + m for m in metric]
  plt.legend(metric1 + metric2 + metric3)


def main():
  n_arg = len(sys.argv)

  functions = ['Bsearch Iterative', 'Bsearch Recursive']
  metric = ['Mean Nano(Cycles)']
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

