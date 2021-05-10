import subprocess
import datetime
import json
import csv

exe_path = "C:\\Users\\john_\\AppData\\Local\\Temp\\tbb_examples\\x64\\Release\\"
json_path = "C:\\Users\\john_\\Documents\\tbb_env\\"


def run_program(threshold, seed_size,seed=int(datetime.datetime.utcnow().timestamp())):
    subprocess.run([exe_path + "ExpandOnce.exe", str(threshold),str(seed_size),str(seed)])

def log_program_results():
    f = open(json_path + "res.json")
    res = json.load(f)
    f.close()

    data_file = open(json_path + "tests.csv", 'a',newline='') 
    writer = csv.writer(data_file, delimiter=',')
    writer.writerow(res.values())

    data_file.close()

def run_and_log_program(threshold,seed_size,seed=int(datetime.datetime.utcnow().timestamp())):
    run_program(threshold, seed_size,seed)
    log_program_results()


run_and_log_program(3,5,233)


