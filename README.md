# LiDARSim

Generates two images given a 3D scene, one labeled and one unlabeled.
The unlabeled image is the result of a simulation of a spherical LiDAR camera. The color of the pixels in the image represents the distance of the intersection point of the light ray on the scene to the camera origin.

The color of the pixels in the labeled image is the classification of that intersection point. Each color should represent a different type of "object" being hit (green means tree leaves, gray is cement, etc.). The color comes from the .mtl file that is associated with the Wavefront object that is input.

**usage:** LiDARSim.exe path_to_scene_obj output_file_name

**How to build:** The only dependencies to this project is the WaveFront object parsing library, tiny_obj_loader.h, and a bitmap image writing library, bitmap_image.hpp. The source for these libraries is included in the _src_ directory and they do not require and dlls. Because of this, the project should build without need for any other downloads except for the source code.

**Other note:** the .mtl file associated with the input WaveFront object must be in the same directory as the executable.
