import bpy
import mathutils
import random
import time
import numpy as np
N = 5
CITY_NAME = "City"
MESH_NAME = "Custom"
WALL = "Material.009"
DOOR = "Material.011"
WINDOW = "Material.010"
AREA_X = 10000
AREA_Y = 10000
MIN_XY = 10
MAX_XY = 50
MIN_Z = 25
MAX_Z = 50
Z_SIZE_RATIO_MIN = 0.75
bpy.data.objects['Cube'].select = True
bpy.ops.object.delete()
bpy.ops.import_scene.obj(filepath=".\\custom.obj")
bpy.data.objects[MESH_NAME].hide = True
np.random.seed(seed=int(time.time()))
for i in range(N):
	TOTAL_BUILDINGS = np.random.randint(60, 120)
	AVG_VAR_XY_1 = np.random.randint(1, 10)
	AVG_VAR_XY_2 = np.random.randint(20, 30)
	AVG_VAR_Z_1 = np.random.randint(8, 16)
	AVG_VAR_Z_2 = np.random.randint(20, 28)
	bpy.ops.node.new_node_tree(type='NodeTree_SceneCity', name=CITY_NAME)
	city = bpy.data.node_groups[CITY_NAME]
	node1 = city.nodes.new(type="BlenderMesh2MeshDataNode")
	node2 = city.nodes.new(type="SimpleBuildingsLayoutNode")
	node3 = city.nodes.new(type="MeshData2ObjectNode")
	node1.mesh_name = MESH_NAME
	node2.total_buildings = TOTAL_BUILDINGS
	node2.area_size_xy[0] = AREA_X
	node2.area_size_xy[1] = AREA_Y
	node2.buildings_size_xy_values[0] = AVG_VAR_XY_1
	node2.buildings_size_xy_values[1] = AVG_VAR_XY_2
	node2.buildings_size_z_values[0] = AVG_VAR_Z_1
	node2.buildings_size_z_values[1] = AVG_VAR_Z_2
	node2.buildings_xy_size_min_max[0] = MIN_XY
	node2.buildings_xy_size_min_max[1] = MAX_XY
	node2.buildings_z_size_min_max[0] = MIN_Z
	node2.buildings_z_size_min_max[1] = MAX_Z
	node2.buildings_ratio_z[0] = Z_SIZE_RATIO_MIN
	city.links.new(node2.inputs[0], node1.outputs[0])
	city.links.new(node3.inputs[0], node2.outputs[0])
	bpy.ops.node.mesh_object_node_create_object(source_node_path="bpy.data.node_groups[\"" + CITY_NAME + "\"].nodes[\"Mesh data -> object\"]")
	city_materials = bpy.data.objects[CITY_NAME].data.materials
	if len(city_materials) == 3:
		city_materials[0] = bpy.data.materials[WALL]
		city_materials[1] = bpy.data.materials[WINDOW]
		city_materials[2] = bpy.data.materials[DOOR]
	bpy.ops.export_scene.obj(filepath=".\\scenes\\" + str(i) + ".obj")
	bpy.data.objects['City'].select = True
	bpy.ops.object.delete()
