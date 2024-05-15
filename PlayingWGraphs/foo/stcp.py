import pandas as pd
import networkx as nx

directory = 'dataset/'

# Read stops.csv
stops_df = pd.read_csv(directory + 'stops.csv')

# Read lines.csv
lines_df = pd.read_csv(directory + 'lines.csv')

# Read line_200_0.csv as an example
line_200_0_df = pd.read_csv(directory + 'line_200_0.csv', header=None, names=['Stop'])

# Create a directed multigraph
G = nx.MultiDiGraph()

# Add nodes from stops.csv
for index, row in stops_df.iterrows():
    G.add_node(row['Code'], name=row['Name'], zone=row['Zone'], latitude=row['Latitude'], longitude=row['Longitude'])

# Add edges from line_200_0.csv
prev_stop = None
for index, row in line_200_0_df.iterrows():
    stop = row['Stop']
    if prev_stop is not None:
        G.add_edge(prev_stop, stop, line='200', direction='0')
    prev_stop = stop

# Print the nodes and edges of the multigraph
print("Nodes:", G.nodes(data=True))
print("Edges:", G.edges(data=True))


