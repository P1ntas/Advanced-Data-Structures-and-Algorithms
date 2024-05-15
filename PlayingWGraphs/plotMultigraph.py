import matplotlib.pyplot as plt
import networkx as nx

G = nx.MultiDiGraph()  # Change graph type to MultiGraph
edge_list = [
    ("A", "B", {"label": "Road", "weight": 10}),
    ("A", "B", {"label": "Railway", "weight": 7}),
    ("A", "C", {"label": "Road", "weight": 15}),
    ("B", "C", {"label": "Road", "weight": 5}),
    ("B", "D", {"label": "Railway", "weight": 12}),
    ("C", "E", {"label": "Road", "weight": 8}),
    ("D", "E", {"label": "Road", "weight": 9}),
    ("D", "E", {"label": "Air", "weight": 4}),
    ("E", "F", {"label": "Road", "weight": 6}),
    ("E", "G", {"label": "Railway", "weight": 11}),
    ("F", "G", {"label": "Road", "weight": 7}),
    ("A", "D", {"label": "Air", "weight": 14}),
    ("B", "E", {"label": "Road", "weight": 9}),
    ("C", "D", {"label": "Railway", "weight": 10}),
    ("D", "F", {"label": "Road", "weight": 8}),
    ("E", "D", {"label": "Railway", "weight": 13}),
    ("F", "A", {"label": "Road", "weight": 12}),
    ("A", "E", {"label": "Air", "weight": 11}),
    ("B", "F", {"label": "Road", "weight": 7}),
    ("C", "G", {"label": "Railway", "weight": 9}),
    ("D", "G", {"label": "Road", "weight": 10}),
    ("E", "C", {"label": "Air", "weight": 15}),
    ("F", "D", {"label": "Road", "weight": 8}),
    ("G", "A", {"label": "Road", "weight": 13}),
    ("A", "F", {"label": "Railway", "weight": 9}),
    ("B", "G", {"label": "Air", "weight": 12}),
    ("C", "A", {"label": "Road", "weight": 7}),
    ("D", "B", {"label": "Railway", "weight": 11}),
    ("E", "A", {"label": "Road", "weight": 8}),
    ("F", "B", {"label": "Road", "weight": 10}),
    ("G", "C", {"label": "Air", "weight": 14}),
    ("A", "H", {"label": "Road", "weight": 9}),
    ("B", "I", {"label": "Railway", "weight": 10}),
    ("C", "J", {"label": "Road", "weight": 11}),
    ("D", "K", {"label": "Road", "weight": 12}),
    ("E", "L", {"label": "Railway", "weight": 13}),
    ("F", "M", {"label": "Road", "weight": 14}),
    ("G", "N", {"label": "Road", "weight": 15}),
    ("H", "I", {"label": "Air", "weight": 16}),
    ("I", "J", {"label": "Road", "weight": 17}),
    ("J", "K", {"label": "Railway", "weight": 18}),
    ("K", "L", {"label": "Road", "weight": 19}),
    ("L", "M", {"label": "Air", "weight": 20}),
    ("M", "N", {"label": "Road", "weight": 21}),
    ("N", "A", {"label": "Road", "weight": 22}),
    ("A", "O", {"label": "Road", "weight": 23}),
    ("B", "P", {"label": "Railway", "weight": 24}),
    ("C", "Q", {"label": "Road", "weight": 25}),
    ("D", "R", {"label": "Road", "weight": 26}),
    ("E", "S", {"label": "Railway", "weight": 27}),
    ("F", "T", {"label": "Road", "weight": 28}),
    ("G", "U", {"label": "Road", "weight": 29}),
    ("H", "V", {"label": "Air", "weight": 30}),
    ("I", "W", {"label": "Road", "weight": 31}),
    ("J", "X", {"label": "Railway", "weight": 32}),
    ("K", "Y", {"label": "Road", "weight": 33}),
    ("L", "Z", {"label": "Air", "weight": 34}),
    ("M", "O", {"label": "Road", "weight": 17}),
    ("N", "P", {"label": "Railway", "weight": 19}),
    ("O", "Q", {"label": "Road", "weight": 21}),
    ("P", "R", {"label": "Road", "weight": 23}),
    ("Q", "S", {"label": "Railway", "weight": 25}),
    ("R", "T", {"label": "Road", "weight": 27}),
    ("S", "U", {"label": "Road", "weight": 29}),
    ("T", "V", {"label": "Air", "weight": 31}),
    ("U", "W", {"label": "Road", "weight": 33}),
    ("V", "X", {"label": "Railway", "weight": 35}),
    ("W", "Y", {"label": "Road", "weight": 37}),
    ("X", "Z", {"label": "Air", "weight": 39}),
    ("Z", "A", {"label": "Road", "weight": 40}),
]
G.add_edges_from(edge_list)


pos=nx.spring_layout(G,seed=5)
fig, ax = plt.subplots()
nx.draw_networkx_nodes(G, pos, ax=ax)
nx.draw_networkx_labels(G, pos, ax=ax)

# Separate curved and straight edges
curved_edges = [edge for edge in G.edges(keys=True) if (edge[1], edge[0], edge[2]) in G.edges(keys=True)]
straight_edges = list(set(G.edges(keys=True)) - set(curved_edges))
print(len(curved_edges))
print(len(straight_edges))

# Draw straight edges
nx.draw_networkx_edges(G, pos, ax=ax, edgelist=straight_edges)

# Draw curved edges
arc_rad = 0.25
nx.draw_networkx_edges(G, pos, ax=ax, edgelist=curved_edges, connectionstyle=f'arc3, rad = {arc_rad}')
fig.savefig("graph.png", bbox_inches='tight',pad_inches=0)
