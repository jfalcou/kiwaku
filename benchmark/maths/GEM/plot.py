import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import sys

def Parse(fname, df, Function, metric, plot_type, save):
    for f in Function:
      copy = df.loc[df['Function'] == f]

      if(fname.__contains__('nano')):
        if("plot" in plot_type):
          plot(fname, copy, f, metric, save)
        if("bar" in plot_type):
          box(fname, copy, f, metric, save)

      if(fname.__contains__('chrono')):
        box_chrono(fname, df, save)
      
def plot(fname, df, Function, metric, save):
  plt.figure()
  plt.semilogx(df['Size(Bytes)'], df[metric])
  plt.xlabel('Size (Bytes)')
  plt.ylabel('GBytes/s')
  plt.title(fname.split(sep='.')[0]+ "_" + Function)
  plt.legend(metric)

  if save:
    plt.savefig(fname.split(sep='.')[0]+ "_" + Function)
  else:
    plt.show()

def box(fname, df, Function, metric, save):
    means = df['Mean Nano(GBytes/sec)']
    mins = df['Min Nano(GBytes/sec)']
    maxs = df['Max Nano(GBytes/sec)']
    errors = df['Err Nano(GBytes/sec)']
    x = df['Size(Bytes)']

    plt.figure()
    plt.xscale('log')
    plt.errorbar(x, means, errors, fmt='.k', lw=2)
    plt.errorbar(x, means, [abs(means-mins), abs(maxs-means)], fmt='.k', lw=1)
    plt.xlabel('Size (Bytes)')
    plt.ylabel('GBytes/s')
    plt.title(fname.split(sep='.')[0]+ "_" + Function)

    if save:
      plt.savefig(fname.split(sep='.')[0]+ "_" + Function + "_bars")
    else:
      plt.show()

def box_chrono(fname, df, save):
  x = df['Size(Bytes)'].to_numpy()
  values = df.iloc[:,3:].to_numpy()

  toto = str(x[0])
  print(toto)

  print(values[0])
  plt.figure()
  plt.boxplot(values)
  # plt.xlabel(toto)
  plt.xscale('log')

  
  plt.show()
  
def main():
  n_arg = len(sys.argv)

  functions = ['GEMV', 'GEMM']
  metric = ['Mean Babel(GBytes/sec)','Mean Nano(GBytes/sec)','Median Nano(GBytes/sec)']
  plot_type = ['plot'] # 'bar'

  if(n_arg < 2):
    print("Please input the file\n")
    return -1
  else:
    for Arg in sys.argv[1:]:
      fname = Arg
      df = pd.read_csv(fname, sep=";")
      Parse(fname, df, functions, metric, plot_type, save=True)
   
if __name__ == "__main__":
  sys.exit(main())

