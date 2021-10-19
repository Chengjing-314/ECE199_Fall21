import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatch
from matplotlib.pyplot import figure
import json


def draw_rect(length, width, x, y,cor = "orange",center = True):
    if center:
        x = x - length / 2
        y = y - width / 2
    print("drawing")
    rec = mpatch.Rectangle((x,y),length,width,color = cor)
    plt.gca().add_patch(rec)

def build_object(file_name):
    with open(file_name) as js_file:
        objects = json.load(js_file)
        for p in objects["col_obj"]:
            length = p["length"]
            width = p["width"]
            x = p["x"]
            y = p['y']
            draw_rect(length,width,x,y)


def build_boundary(file_name):
    with open(file_name) as js_file:
        boundary = json.load(js_file)
        for p in boundary['map_info']:
            length = p['length']
            width = p['width']
            draw_rect(1,width,-1,0,cor = "red",center=False)
            draw_rect(1,width,length,0,cor = "red",center=False)
            draw_rect(length,1,0,-1,cor = "red",center=False)
            draw_rect(length,1,0,width,cor = "red",center=False)
            plt.xlim([-5,length + 10])
            plt.ylim([-5,width + 10])



fig,ax = plt.subplots(figsize = (50,50))

build_object('cl_obj.json')
build_boundary('cl_obj.json')

ax.set_aspect(1)
plt.show()

