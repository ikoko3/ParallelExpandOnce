import networkx as nx
import matplotlib.pyplot as plt
from random import randint

export_path = "C:\\Users\\john_\\Documents\\tbb_env\\tbb\\"

def create_similar_graph(percentage, graph):
    n_graph = graph.copy()

    for edge in n_graph.edges:
        rand = randint(0, 99)
        if rand > percentage+1:
            graph.remove_edge(*edge)

    return n_graph


def create_noisy_seed_set(max_count, graph):
    list = []
    size = graph.number_of_nodes()
    if size <= max_count:
        raise Exception('not enough nodes')
    while len(list) < max_count:
        r = randint(0, graph.number_of_nodes()-1)
        if r not in list:
            list.append(r)

    return list

def generate_graphs(nodes,neighbouring_nodes,seed_set_size,graph_similatity):
    graph1 = nx.generators.random_graphs.barabasi_albert_graph(nodes, neighbouring_nodes)
    graph2 = create_similar_graph(graph_similatity, graph1)
    noisy_set = create_noisy_seed_set(seed_set_size, graph1)

    #nx.draw(graph1)
    #plt.show()

    #nx.draw(graph2)
    #plt.show()

    nx.write_adjlist(graph1,export_path + "graph1.adjlist")
    nx.write_adjlist(graph2,export_path + "graph2.adjlist")

    with open(export_path + 'noisy_set.list', 'w') as f:
        for item in noisy_set:
            f.write(f"{item} {item}\n")


generate_graphs(500,35,10,80)