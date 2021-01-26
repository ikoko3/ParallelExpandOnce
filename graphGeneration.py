import networkx as nx
import matplotlib.pyplot as plt
from random import randint


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


graph1 = nx.generators.random_graphs.barabasi_albert_graph(500, 35)
graph2 = create_similar_graph(80, graph1)
noisy_set = create_noisy_seed_set(10, graph1)

#nx.draw(graph1)
#plt.show()

#nx.draw(graph2)
#plt.show()

nx.write_adjlist(graph1, "graph1.adjlist")
nx.write_adjlist(graph2, "graph2.adjlist")

with open('noisy_set.list', 'w') as f:
    for item in noisy_set:
        f.write(f"{item} {item}\n")


