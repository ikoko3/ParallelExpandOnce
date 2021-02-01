import runProgram as p
import graphGeneration as gg

graph_sizes = [[10000,300],[5000,150]]
threshold_values = [6]
seed_set_sizes = [10]
test_times = 5
graph_similarity = 80

for seed_set_size in seed_set_sizes:
    for graph_size in graph_sizes:
        print(graph_size)
        gg.generate_graphs(graph_size[0],graph_size[1],seed_set_size,graph_similarity)
        for threshold in threshold_values:
            for i in range(test_times):
                try:
                    p.run_and_log_program(threshold,5)
                except:
                    print("An exception occurred")
                
    


