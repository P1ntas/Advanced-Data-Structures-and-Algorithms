import pandas as pd

input_file_path = './dataset/data.csv'

df = pd.read_csv(input_file_path)

print(df.head())

output_file_path = './output_data.json'

json_str = df.to_json(orient='records', indent=4)

with open(output_file_path, 'w') as json_file:
    json_file.write(json_str)

print(f'Data saved to {output_file_path}')