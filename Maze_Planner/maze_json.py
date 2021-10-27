import json

obj = {}

obj['col_obj'] = []
obj['map_info'] = []

obj['map_info'].append({
        'length' : 100,
        'width' : 100
})

obj['col_obj'].append(
    {
        'length': 2,
        'width': 30,
        'x': 10,
        'y': 15
    }
)

obj['col_obj'].append(
    {
        'length': 20,
        'width': 2,
        'x': 26,
        'y': 29
    }
)


obj['col_obj'].append(
    {
        'length': 2,
        'width': 20,
        'x': 35,
        'y': 50
    }
)


obj['col_obj'].append(
    {
        'length': 28,
        'width': 2,
        'x': 14,
        'y': 50
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 14,
        'x': 14,
        'y': 58
    }
)


obj['col_obj'].append(
    {
        'length': 2,
        'width': 10,
        'x': 18,
        'y': 40
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 40,
        'x': 25,
        'y': 80
    }
)

obj['col_obj'].append(
    {
        'length': 15,
        'width': 2,
        'x': 7.5,
        'y': 78
    }
)


obj['col_obj'].append(
    {
        'length': 30,
        'width': 2,
        'x': 50,
        'y': 75
    }
)

obj['col_obj'].append(
    {
        'length': 20,
        'width': 2,
        'x': 36,
        'y': 86
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 18,
        'x': 56,
        'y': 91
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 14,
        'x': 12,
        'y': 93
    }
)


obj['col_obj'].append(
    {
        'length': 2,
        'width': 50,
        'x': 50,
        'y': 49
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 20,
        'x': 26,
        'y': 10
    }
)

obj['col_obj'].append(
    {
        'length': 12,
        'width': 2,
        'x': 33,
        'y': 12
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 28,
        'x': 63,
        'y': 20
    }
)


obj['col_obj'].append(
    {
        'length': 2,
        'width': 12,
        'x': 50,
        'y': 6
    }
)


obj['col_obj'].append(
    {
        'length': 24,
        'width': 2,
        'x': 88,
        'y': 42
    }
)

obj['col_obj'].append(
    {
        'length': 24,
        'width': 2,
        'x': 63,
        'y': 58
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 24,
        'x': 79,
        'y': 80
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 8,
        'x': 63,
        'y': 50
    }
)


obj['col_obj'].append(
    {
        'length': 20,
        'width': 2,
        'x': 90,
        'y': 82
    }
)

obj['col_obj'].append(
    {
        'length': 12,
        'width': 2,
        'x': 63,
        'y': 92
    }
)


obj['col_obj'].append(
    {
        'length': 18,
        'width': 2,
        'x': 72,
        'y': 20
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 18,
        'x': 81,
        'y': 15
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 16,
        'x': 92,
        'y': 33
    }
)

obj['col_obj'].append(
    {
        'length': 2,
        'width': 24,
        'x': 90,
        'y': 69
    }
)


try:
    with open('cl_obj.json','w') as outputfile:
        json.dump(obj,outputfile,indent = 4)
    print("Writing Success!")
except Exception as e:
    print("Writing Faild")
finally:
    exit()
    
