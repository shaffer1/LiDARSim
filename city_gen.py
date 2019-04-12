import bpy
bpy.data.objects['Cube'].select = True
bpy.ops.object.delete()
bpy.ops.node.new_node_tree(type='NodeTree_SceneCity', name="City1")
city = bpy.data.node_groups["City1"]
node1 = city.nodes.new(type="ProceduralCubeMeshNode")
node2 = city.nodes.new(type="SimpleBuildingsLayoutNode")
node3 = city.nodes.new(type="MeshData2ObjectNode")
city.links.new(node2.inputs[0], node1.outputs[0])
city.links.new(node3.inputs[0], node2.outputs[0])
bpy.ops.node.mesh_object_node_create_object(source_node_path="bpy.data.node_groups[\"City1\"].nodes[\"Mesh data -> object\"]")
