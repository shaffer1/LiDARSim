City Generation Script

How to Run:
1. Install Scene City from here: https://drive.google.com/open?id=1CE3PDL98Su3vO91Cd9K0KTIXZ_MlpXD5
2. Instructions on installing: https://www.cgchan.com/static/doc/scenecity/1.2/getting_started.html
3. Open blender. Change one of the panels to Text Editor Mode.
4. Open a new file: city_gen_script.py
5. Run Script
6. Check scenes folder for .obj and .mtl files of each scene generated.

Notes:
1. Blender may throw an error at the file path which is currently set as .//custom.obj and .//scenes//*.obj
You can simply change it to the absolute path of the city_gen folder. 
2. If you run into a bug where all the scenes generated look the same (despite all the numbers variating), reduce the iteration value N to 1,
and execute steps 3-6 for the number of scenes you wish to create.

Some Explanations:
Scene City creates really great cities but unfortunately, you can only use materials and color the buildings using Cycles Render.
This is not compatible with the lidar simulator that uses these scenes. 
The LiDAR simulator requires a Blender Render engine and so I had to create a custom set of buildings to use for the scenes.
As a result, the scenes are pretty ugly. Please feel free to create better custom buildings and use them instead of mine.
Just a note, the building needs to have a combined mesh. And whatever materials you apply to the custom building mesh, will also have to be re-applied in the script.
I left comments mentioning where this can be done.

To-Do:
1. Fix the occassional bug of repeated scenes despite varying values
2. Get better custom buildings.