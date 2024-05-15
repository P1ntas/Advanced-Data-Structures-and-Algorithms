import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

directory = 'dataset/'

# Read stops.csv
stops_df = pd.read_csv(directory + 'stops.csv')

# Create a dictionary to store graphs for each line
line_graphs = {}
reversed_line_graphs = {}

# Read lines and line_*.csv files
lines_df = pd.read_csv(directory + 'lines.csv')

# Create a graph to represent the bus network
G = nx.MultiDiGraph()

for index, row in lines_df.iterrows():
    line_code = row['Code']
    line_name = row['Name']

    # Read line file for each line
    line_file_path = directory + f'line_{line_code}_0.csv'
    reverse_line_file_path = directory + f'line_{line_code}_1.csv'

    line_df = pd.read_csv(line_file_path, header=None, names=['Stop'], skiprows=1)  # Skip the first line
    reverse_line_df = pd.read_csv(reverse_line_file_path, header=None, names=['Stop'], skiprows=1)  # Skip the first line

    # Create a directed graph for the line
    line_graph = nx.MultiDiGraph()
    for index, row in line_df.iterrows():
        stop = row['Stop']
        line_graph.add_node(stop)

    reversed_line_graph = nx.MultiDiGraph()
    for index, row in reverse_line_df.iterrows():
        stop = row['Stop']
        reversed_line_graph.add_node(stop)

    # Add edges for the line
    prev_stop = None
    for index, row in line_df.iterrows():
        stop = row['Stop']
        if prev_stop is not None:
            line_graph.add_edge(prev_stop, stop, line=line_code, direction='0')

        prev_stop = stop

    prev_stop = None
    for index, row in reverse_line_df.iterrows():
        stop = row['Stop']
        if prev_stop is not None:
            reversed_line_graph.add_edge(prev_stop, stop, line=line_code, direction='1')
        prev_stop = stop

    line_graphs[line_code] = line_graph
    reversed_line_graphs[line_code] = reversed_line_graph

# Merge graphs to create the multigraph
for graph in line_graphs.values():
    G = nx.compose(G, graph)
for graph in reversed_line_graphs.values():
    G = nx.compose(G, graph)

# Set node positions based on latitude and longitude
node_positions = {row['Code']: (row['Longitude'], row['Latitude']) for idx, row in stops_df.iterrows() if row['Code'] in G}

# Plot the multigraph with adjusted visual attributes
fig, ax = plt.subplots(figsize=(12, 12))
nx.draw(G, pos=node_positions, with_labels=True, node_size=50, ax=ax, arrowsize=10, node_color='lightblue', font_size=8)
plt.title('Bus Network', fontsize=5)
plt.show()
