import runProgram as p
import graphGeneration as gg

graph_sizes = [[500,35],[1000,50],[2500,75],[5000,140]]
threshold_values = [2,3,4,5,6]
seed_set_sizes = [5,6,7,8,9,10]
test_times = 20
graph_similarity = 0.8

for seed_set_size in seed_set_sizes:
    for graph_size in graph_sizes:
        print(graph_size)
        gg.generate_graphs(graph_size[0],graph_size[1],seed_set_size,graph_similarity)
        for threshold in threshold_values:
            for i in range(test_times):
                p.run_and_log_program(threshold,5)
    


