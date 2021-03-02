import matplotlib.pyplot as plt
import pandas as pd
from pandas.plotting import scatter_matrix
import numpy as np

csv_path = "C:\\Users\\john_\\Documents\\tbb_env\\"
json_path = "C:\\Users\\john_\\Documents\\tbb_env\\"

noisy_seeds_results = csv_path + "NoisySeeds.csv"
expand_once_results = csv_path + "expandOnce.csv"

file = noisy_seeds_results
#file = expand_once_results


def normalize_data(df):
    print('started filter')
    for i, row in df.iterrows():
        total = row['Nodes']
        stotal = row['Stotal']
        ptotal = row['Ptotal']

        if stotal < total*0.2 or ptotal < total*0.2:
            print(f'false positive detected removing row {stotal}')
            df.drop(i, inplace=True)


def print_density_time(df):
    plt.scatter(df.loc[:,'G2Edges']/df.loc[:,'Nodes'],df.loc[:,'Stime'],label='Serial',color='r')
    plt.scatter(df.loc[:,'G2Edges']/df.loc[:,'Nodes'],df.loc[:,'Ptime'],label='Parallel',color='b')
    plt.xlabel('Graph Density (Edges/Nodes)')
    plt.ylabel('Time (ms)')
    plt.title('Expand Once Density vs Time')
    plt.legend()
    plt.show()

def print_threshold_accuracy(df):
    plt.scatter(df.loc[:,'threshold'],df.loc[:,'Saccuracy'],label='Serial',color='r')
    plt.scatter(df.loc[:,'threshold'],df.loc[:,'Paccuracy'],label='Parallel',color='b')
    plt.xlabel('Threshold')
    plt.ylabel('Accuracy %')
    plt.title('Noisy Seeds Accuracy')
    plt.legend()
    plt.show()

def print_matrix(df):
    scatter_matrix(df[['G2Edges','Stime','Saccuracy','Ptime','Paccuracy','threshold']])
    plt.show()

def calculate_acceleration(df):
    serial_time_mean = df.mean()['Stime']
    parallel_time_mean = df.mean()['Ptime']
    acceleration = serial_time_mean / parallel_time_mean   
    print(f"Serial time Mean: {serial_time_mean}")
    print(f"Parallel time Mean:  {parallel_time_mean}")
    print(f"Acceleration: {acceleration}")

def compare_avg_accuracy(df, min_treshold):
    df = df.apply(pd.to_numeric)
    filtered_df = df.query(f'threshold >= {min_treshold}')
    serial_acc_mean = filtered_df.mean()['Saccuracy']
    parallel_acc_mean = filtered_df.mean()['Paccuracy']
    improvement = parallel_acc_mean / serial_acc_mean   
    print(f"Serial accuracy Mean: {serial_acc_mean}")
    print(f"Parallel accuracy Mean:  {parallel_acc_mean}")
    print(f"Improvement: {improvement}")

names = ['threshold','SeedSetSize','Nodes','G1Edges','G2Edges','Stime','Saccuracy','Scorrect','Stotal','Ptime','Paccuracy','Pcorrect','Ptotal'] 
df = pd.read_csv(file, names=names,index_col=False)


selection = df.loc[:2,['Nodes','Stime','Ptime']]

normalize_data(df)
#print_density_time(df)
#print_threshold_accuracy(df)
#print_matrix(df)
#calculate_acceleration(df)
compare_avg_accuracy(df,3)
#print(df)

