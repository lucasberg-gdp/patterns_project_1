# ConvertOBJtoDAE.py

import bpy
import os

# Path to directory containing .obj files
input_dir = "C:/Users/Lucas/OneDrive/Lucas/Education/GDP/Level2-24Winter/Graphics_2/projects/project_1/OpeningGL/OpenGLCode/OpeningGL/assets/models/PiratePack/"

# Path to output directory for .dae files
output_dir = "C:/Users/Lucas/OneDrive/Lucas/Education/GDP/Level2-24Winter/Graphics_2/projects/project_1/OpeningGL/OpenGLCode/OpeningGL/assets/models/PiratePack/Converted"
# List all files in the input directory
files = os.listdir(input_dir)

batch_size = 500

# Iterate through each .obj file
for i in range(0, len(files), batch_size):

    batch_files = files[i:i+batch_size]
    
    for file in batch_files:
        if file.endswith(".obj"):
        # Load the .obj file

            bpy.ops.import_scene.obj(filepath=os.path.join(input_dir, file))

        # Set output file path
            output_file = os.path.join(output_dir, os.path.splitext(file)[0] + ".dae")

        # Export as .dae
            bpy.ops.wm.collada_export(filepath=output_file)

        # Remove the imported object from the scene
            bpy.ops.object.select_all(action='DESELECT')
            bpy.ops.object.select_by_type(type='MESH')
            bpy.ops.object.delete()

# Exit Blender
bpy.ops.wm.quit_blender()