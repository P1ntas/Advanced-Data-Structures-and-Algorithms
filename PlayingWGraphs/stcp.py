import numpy as np
import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

# Define the directory where the dataset is located
directory = 'dataset/'

# Read the stops dataset and preprocess it to group similar stops by name
stops_df = pd.read_csv(directory + 'stops.csv')
grouped_stops = stops_df.groupby('Name').agg({'Code': ','.join, 'Latitude': 'first', 'Longitude': 'first', 'Zone': 'first'}).reset_index()

# Get unique zones from the grouped_stops dataframe
unique_zones = grouped_stops['Zone'].unique()

# Define the number of unique zones
num_zones = len(unique_zones)

# Create a rainbow color map for the unique zones
colors = plt.cm.rainbow(np.linspace(0, 1, num_zones))

# Create a dictionary to map zones to rainbow colors
zone_color_map = dict(zip(unique_zones, colors))

# Prompt the user to input the number of lines to read
num_lines = int(input("Enter the number of lines to read: "))

# Create a dictionary to store graphs for each line
line_graphs = {}
reversed_line_graphs = {}

# Read the lines dataset
# if num_lines is 0, read all lines
if num_lines == 0:
    lines_df = pd.read_csv(directory + 'lines.csv')
else:
    lines_df = pd.read_csv(directory + 'lines.csv').head(num_lines)  # Read only the specified number of lines

# Create a graph to represent the bus network
G = nx.MultiDiGraph()

# Iterate through each line in the lines dataset
for index, row in lines_df.iterrows():
    line_code = row['Code']
    line_name = row['Name']
    
    line_file_path = directory + f'line_{line_code}_0.csv'
    reverse_line_file_path = directory + f'line_{line_code}_1.csv'

    # Read the line and its reverse line data from the respective CSV files
    line_df = pd.read_csv(line_file_path, header=None, names=['Stop'], skiprows=1)
    reverse_line_df = pd.read_csv(reverse_line_file_path, header=None, names=['Stop'], skiprows=1)

    # Create directed graphs for the line and its reverse
    line_graph = nx.MultiDiGraph()
    reversed_line_graph = nx.MultiDiGraph()

    # Add nodes and edges to the line and reverse line graphs
    # Create a directed graph for the line
    line_graph = nx.MultiDiGraph()
    for index, row in line_df.iterrows():
        stop = row['Stop']
        group_code = grouped_stops[grouped_stops['Code'].str.contains(stop)]['Name'].values[0]
        line_graph.add_node(group_code)

    reversed_line_graph = nx.MultiDiGraph()
    for index, row in reverse_line_df.iterrows():
        stop = row['Stop']
        group_code = grouped_stops[grouped_stops['Code'].str.contains(stop)]['Name'].values[0]
        reversed_line_graph.add_node(group_code)

# Add edges for the line
    prev_stop = None
    for index, row in line_df.iterrows():
        stop = row['Stop']
        group_code = grouped_stops[grouped_stops['Code'].str.contains(stop)]['Name'].values[0]
        if prev_stop is not None:
            line_graph.add_edge(prev_stop, group_code, line=f'{line_code}_0', direction='0')

        prev_stop = group_code

    prev_stop = None
    for index, row in reverse_line_df.iterrows():
        stop = row['Stop']
        group_code = grouped_stops[grouped_stops['Code'].str.contains(stop)]['Name'].values[0]
        if prev_stop is not None:
            reversed_line_graph.add_edge(prev_stop, group_code, line=f'{line_code}_1', direction='1')
        prev_stop = group_code

    # Store the line and reverse line graphs in the respective dictionaries
    line_graphs[line_code] = line_graph
    reversed_line_graphs[line_code] = reversed_line_graph

# Merge the line graphs to create the overall bus network graph
for graph in line_graphs.values():
    G = nx.compose(G, graph)

for graph in reversed_line_graphs.values():
    G = nx.compose(G, graph)
    

# Set node positions based on latitude and longitude
node_positions = {row['Name']: (row['Longitude'], row['Latitude']) for idx, row in grouped_stops.iterrows() if row['Name'] in G}
# Remove self-loop edges from the graph
G.remove_edges_from(nx.selfloop_edges(G))

# Get the zone for each node in the graph
node_zones = {row['Name']: row['Zone'] for idx, row in grouped_stops.iterrows() if row['Name'] in G}

# Assign rainbow colors to nodes based on their zones
node_colors = [zone_color_map[node_zones[node]] for node in G.nodes()]

curved_edges = [edge for edge in G.edges() if (edge[1], edge[0]) in G.edges()]
straight_edges = list(set(G.edges()) - set(curved_edges))
arc_rad = 0.1

# Plot the bus network graph with rainbow node colors
fig, ax = plt.subplots(figsize=(30, 30))
nx.draw_networkx_nodes(G, pos=node_positions, ax=ax, node_color=node_colors, node_size=50, alpha=0.5)
fig.savefig("1.png", bbox_inches='tight',pad_inches=0)

nx.draw_networkx_edges(G, pos=node_positions, ax=ax, edgelist=straight_edges)
nx.draw_networkx_edges(G, pos=node_positions, ax=ax, edgelist=curved_edges, connectionstyle=f'arc3, rad={arc_rad}')
fig.savefig("2.png", bbox_inches='tight',pad_inches=0)

plt.title('Bus Network', fontsize=15)
#plt.show()

# Identify edges with the same origin and destination, even with different lines
same_origin_dest_edges = []
for edge in G.edges(keys=True):
    if G.out_degree(edge[0]) > 1 and G.in_degree(edge[1]) > 1:
        same_origin_dest_edges.append(edge)

# Plot the bus network graph with highlighted red lines for edges with the same origin and destination
fig, ax = plt.subplots(figsize=(30, 30))
nx.draw_networkx_nodes(G, pos=node_positions, ax=ax, node_color=node_colors, node_size=50, alpha=0.5)
nx.draw_networkx_edges(G, pos=node_positions, ax=ax, edgelist=straight_edges, edge_color='black')
nx.draw_networkx_edges(G, pos=node_positions, ax=ax, edgelist=curved_edges, connectionstyle=f'arc3, rad={arc_rad}', edge_color='black')

# Highlight edges with the same origin and destination as red lines
nx.draw_networkx_edges(G, pos=node_positions, ax=ax, edgelist=same_origin_dest_edges, edge_color='red')


fig.savefig("3.png", bbox_inches='tight',pad_inches=0)
plt.title('Bus Network', fontsize=15)
plt.show()
