import json

obj = {}

obj['col_obj'] = []

obj['col_obj'].append(
    {
        'length': 10,
        'width': 10,
        'x': 10,
        'y': 20,
    }
)

obj['col_obj'].append(
    {
        'length': 5,
        'width': 5,
        'x': 15,
        'y': 16,
    }
)

obj['col_obj'].append(
    {
        'length': 5,
        'width': 6.5,
        'x': 12,
        'y': 16,
    }
)

with open('cl_obj.json','w') as outputfile:
    json.dump(obj,outputfile,indent = 4)\

print("Writing Success!")
