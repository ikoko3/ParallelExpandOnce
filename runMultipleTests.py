import runProgram as p
import graphGeneration as gg

graph_sizes = [[2500,230],[2500,180],[2500,150],[2500,120],[2500,90]]
threshold_values = [3,4,5,6,7,8]
seed_set_sizes = [5]
test_times = 10
graph_similarity = 80
expanded_seed_set_size = 10

for seed_set_size in seed_set_sizes:
    for graph_size in graph_sizes:
        print(graph_size)
        gg.generate_graphs(graph_size[0],graph_size[1],seed_set_size,graph_similarity)
        for threshold in threshold_values:
            for i in range(test_times):
                try:
                    p.run_and_log_program(threshold,expanded_seed_set_size)
                except:
                    print("An exception occurred")
                
    


