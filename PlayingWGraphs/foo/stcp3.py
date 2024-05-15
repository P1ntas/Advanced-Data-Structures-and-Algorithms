import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

directory = 'dataset/'

# Read stops.csv
stops_df = pd.read_csv(directory + 'stops.csv')

# Read lines and line_*.csv files
lines_df = pd.read_csv(directory + 'lines.csv')

# Create an empty MultiDiGraph to represent the bus network
G = nx.MultiDiGraph()

# Add nodes for each stop with their coordinates
for index, row in stops_df.iterrows():
    G.add_node(row['Code'], name=row['Name'], zone=row['Zone'], pos=(row['Longitude'], row['Latitude']))

# Iterate through the lines to add edges to the multigraph
for index, row in lines_df.iterrows():
    line_code = row['Code']
    line_name = row['Name']

    # Read line file for each line
    line_file_path = directory + f'line_{line_code}_0.csv'
    line_df = pd.read_csv(line_file_path, header=None, names=['Stop'])

    # Add edges for the line
    prev_stop = None
    for index, row in line_df.iterrows():
        stop = row['Stop']
        if prev_stop is not None:
            G.add_edge(prev_stop, stop, line=line_code, direction='0')

        prev_stop = stop

# Print the nodes and edges of the multigraph
print("Nodes:", G.nodes(data=True))
#print("Edges:", G.edges(data=True))

# Generate colors for the edges based on the lines
edge_colors = [f'C{i}' for i in range(len(G.edges()))]

# Extract node positions from the graph
node_positions = {node: data['pos'] for node, data in G.nodes(data=True)}

# Draw the multigraph with node positions and edge colors
fig, ax = plt.subplots()
nx.draw(G, pos=node_positions, with_labels=True, node_size=300, ax=ax, edge_color=edge_colors)
plt.show()
