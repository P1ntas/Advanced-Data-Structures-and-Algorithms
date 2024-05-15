import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

# Define the directory where the dataset is located
directory = 'dataset/'

# Read the stops dataset and preprocess it to group similar stops by name
stops_df = pd.read_csv(directory + 'stops.csv')
grouped_stops = stops_df.groupby('Name').agg({'Code': ','.join, 'Latitude': 'first', 'Longitude': 'first', 'Zone': 'first'}).reset_index()

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
            line_graph.add_edge(prev_stop, group_code, line=line_code, direction='0')

        prev_stop = group_code

    prev_stop = None
    for index, row in reverse_line_df.iterrows():
        stop = row['Stop']
        group_code = grouped_stops[grouped_stops['Code'].str.contains(stop)]['Name'].values[0]
        if prev_stop is not None:
            reversed_line_graph.add_edge(prev_stop, group_code, line=line_code, direction='1')
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

# # Plot the bus network graph with adjusted visual attributes
# fig, ax = plt.subplots(figsize=(30, 30))
# nx.draw(G, pos=node_positions, with_labels=False, node_size=50, arrowsize=5, node_color='lightblue', font_size=8, font_color='black', edge_color='black', width=0.5)
# plt.title('Bus Network', fontsize=15)
# plt.show()

pos = node_positions
fig, ax = plt.subplots(figsize=(30, 30))
nx.draw_networkx_nodes(G, pos, ax=ax, node_color='blue', alpha=0.5, node_size=50)
#nx.draw_networkx_labels(G, pos, ax=ax)
fig.savefig("1.png", bbox_inches='tight',pad_inches=0)

curved_edges = [edge for edge in G.edges() if (edge[1], edge[0]) in G.edges()]
straight_edges = list(set(G.edges()) - set(curved_edges))
nx.draw_networkx_edges(G, pos, ax=ax, edgelist=straight_edges)
arc_rad = 0.1
nx.draw_networkx_edges(G, pos, ax=ax, edgelist=curved_edges, connectionstyle=f'arc3, rad = {arc_rad}')
fig.savefig("2.png", bbox_inches='tight',pad_inches=0)

plt.show()