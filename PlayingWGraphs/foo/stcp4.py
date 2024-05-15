import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

directory = 'dataset/'

# Read stops.csv
stops_df = pd.read_csv(directory + 'stops.csv')

# Create a dictionary to store graphs for each line
line_graphs = {}

# Read lines and line_*.csv files
lines_df = pd.read_csv(directory + 'lines.csv')

for index, row in lines_df.iterrows():
    line_code = row['Code']
    line_name = row['Name']

    # Read line file for each line
    line_file_path = directory + f'line_{line_code}_0.csv'
    line_df = pd.read_csv(line_file_path, header=None, names=['Stop'], skiprows=1)  # Skip the first line

    # Create a directed graph for the line
    line_graph = nx.MultiDiGraph()
    for index, row in line_df.iterrows():
        stop = row['Stop']
        line_graph.add_node(stop)

    # Add edges for the line
    prev_stop = None
    for index, row in line_df.iterrows():
        stop = row['Stop']
        if prev_stop is not None:
            line_graph.add_edge(prev_stop, stop, line=line_code, direction='0')

        prev_stop = stop

    line_graphs[line_code] = line_graph

# Merge graphs to create the multigraph
G = nx.compose_all(list(line_graphs.values()))

# Set node positions based on latitude and longitude
node_positions = {row['Code']: (row['Longitude'], row['Latitude']) for idx, row in stops_df.iterrows() if row['Code'] in G}

# Print the nodes and edges of the multigraph
print("Nodes:", G.nodes(data=True))
print("Edges:", G.edges(data=True))

# Plot the multigraph with node positions
fig, ax = plt.subplots()
nx.draw(G, pos=node_positions, with_labels=True, node_size=300, ax=ax)
plt.show()
