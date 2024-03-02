# Ray-Tracer-Application

![Example](/resources/example.png)

## About
This project aims to present a Ray Tracing library (and a practical Application using it) developed with
Software Engineering guidelines, as a solution for attending to the following client specifications:
1. Allow the description of a realistic 3D scene composition, such as lights and simple shapes (spheres, cubes, etc.);
2. Allow the rendering of a 3D image by using different and extensible motors for casting rays in Ray Tracing methods;
3. Integrate the functionalities of the library within an Application that allows the specification of the scenes, execution
of the Ray Tracing motors and the visualizing the result;
4. Support textures, colors, translucency and reflections in the scene;
5. Define a file language that allows the positioning of objects and composition of objects within the scene;
6. Save the rendered image.

## Built With
This project was fully built in C++ using the following external libraries:
1. [SDL2](https://www.libsdl.org/);
2. [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page);
3. [rapidjason](https://rapidjson.org/).

Special thanks to [QuantitativeBytes](https://github.com/QuantitativeBytes) for showing the implementation 
of different core Ray Tracing functionalities on [YouTube](https://www.youtube.com/c/QuantitativeBytes)!

## Getting Started
Please be sure to include the SDL2.dll and SDL2_image.dll libraries files in the repository,
as the compiler needs these files for properly building the executable. Then, execute the following command:

```console
g++ -Iinclude -Llib -o RayTracerApp main.cpp Application.cpp dependencies/scenes/SceneGeneric.cpp dependencies/Camera.cpp dependencies/Ray.cpp dependencies/objects/ObjectGeneric.cpp dependencies/objects/ObjectSphere.cpp dependencies/light/LightPoint.cpp dependencies/light/LightGeneric.cpp dependencies/objects/GeometricalTransformation.cpp dependencies/objects/ObjectPlane.cpp dependencies/materials/MaterialGeneric.cpp dependencies/materials/MaterialCompleteSimple.cpp dependencies/objects/ObjectCylinder.cpp dependencies/objects/objectCone.cpp dependencies/textures/TextureGeneric.cpp dependencies/textures/TextureChecker.cpp dependencies/textures/TextureFlat.cpp dependencies/textures/TextureGradient.cpp dependencies/textures/ColorMap.cpp dependencies/objects/ObjectCuboid.cpp dependencies/objects/ObjectComposition.cpp dependencies/scenes/SceneBasic.cpp dependencies/scenes/SceneFromJson.cpp dependencies/RTMotors/RTMotorGeneric.cpp dependencies/RTMotors/RTMotorThreaded.cpp dependencies/RTMotors/RTMotorSimple.cpp -lSDL2 -lSDL2_image -pthread

```

## Ray Tracing

### Introduction
Ray Tracing is a rendering technique used in computer graphics to simulate the way light interacts with objects, 
producing realistic images by tracing the path of rays as they intersect with objects and surfaces. Instead of casting 
rays from the light sources, this method casts rays directly from the viewer perspective (camera) into the objects, and
considers the influence of light only if it finds a valid intersection, thus reducing unused computational calculations.

![Basic Ray Tracing](/resources/rt.png)

### Fundamentals
The mathematical base for interactions in the 3D scene between rays, objects and lights are the matrix multiplication 
for matrices and the dot product and cross product for vectors. Every object can be modeled with spatial equations
for their (x,y,z) positions, thus, it is possible to determine points of intersection, normals and parallels to rays
that intersect an object. Objects may have special properties when intersecting with a ray, allowing different types of
color calculation such as:
1. Shadows: When a ray intersects with an object, it prevents the intersection with other objects behind it, thus projecting
a shadow (that is, a region without light) in the object(s) behind it.
2. Diffuse color: this is the base color of the object itself scattered uniformly, disregarding any special interactions 
with the light.
3. Specular color: the reflected color of the object's highlights, that is, the sources of light being reflected to the camera.
4. Reflective color: the ability of the object to recast the ray intersected with it in the appropriate direction into
other objects, simulating a mirror-like interaction.
5. Translucent color: Associated with an index of reflection, it inhibits the effects of shadows by letting rays pass 
through it with a reduced influence in the final color of the intersection, simulating a glass-like interaction.
6. Textures: Pattern of colors in the surface of the object.

### Transformations
The library allows the free placement of objects in a 3D scene, without needing individual definitions for each placement. 
To achieve this, transformations between 2D (u,v) and 3D (x,y,z) representations must be applied to properly represent 
the intended properties of the objects:
1. Image rendering (3D -> 2D): the objects are defined in a 3D space, but we visualize them in 2D. Therefore, they are
projected into the 2D screen.
2. Translation, rotation and scale of objects (3D -> 3D): All objects descriptions are unitary defined in the origin
(that is, with base dimension of 1) and can be translated, rotated and scaled in the scene with a transformation associated
with the object's instance.
3. Translation, rotation and scale of texture (2D->2D): all textures are also modifiable with translation, rotation and scale.
4. Placement of texture in an object (2D -> 3D): For the texture to be rendered in the object, it needs to firstly go through
a transformation to properly calculate its new color values for that position. 

### Object composition and bounding boxes
Object compositions are a special type of object that is composed of a list of objects (that can also be object compositions).
They allow the definitions of internal transformations for the objects within it and also a transformation for itself, 
making it easier to edit the placement of different objects in the scene. This composition is associated with a bounding box
defined with the "Cuboid" class, and the intersection tests for internal objects are only made if the ray intersects with 
this bounding box. This behavior reduces the computational processing in general for compositions made of close objects, 
since the number of intersection tests is reduced.

![Bounding Box of a sphere](/resources/boundingbox.png)

## Using .json files for describing the scene
The folder "userFiles" is used by the application to manage the input .json files given by the user and output the image
file rendered. This section aims to explain how to properly set up the user .json files describing the scene in order to
configure the rendering of any desired scene easily. Some examples are already available in the folder for reference. 

### Generalities
Files in json are very simple and can represent object data without lots of unused information. For our representation,
the name of the file itself does not matter, since a "type" field is dedicated to discerning the different types of 
scene components represented in the file. All files need to specify only one type of component for the scene, and this 
section aims to specify how each field in the file should be fulfilled and what it means.

Above all, it is important to pay attention to the axis coordinates and transformation definitions to avoid misplacing 
the components.

### Camera
- **"type":** "camera".
- **"position":** {"x": x, "y": y,"z": z}, where (x,y,z) are the 3D coordinates in the scene represented in floating point notation. 
This is where the camera will be placed.
- **"lookAt":** {"x": x, "y": y,"z": z}, where (x,y,z) are the 3D coordinates in the scene represented in floating point notation.
This is where the camera will be looking at.
- **"up":** {"x": x, "y": y,"z": z}, where (x,y,z) are the 3D coordinates in the scene represented in floating point notation.
This is what is perceived as "up".
- **"zoomOut":** positive float value, where for values > 1 the camera will be zooming out and for values < 1 it will be zooming in.
- **"ASPECT_RATIO":** positive float value representing the ratio between the width and height of the rendered image.

### Lights
- **"type":** "light".
- **"position":** {"x": x, "y": y,"z": z}, where (x,y,z) are the 3D coordinates in the scene represented in floating point notation.
This is where the light will be placed.
- **"color":** {"red": r, "green": g,"blue": b}, where (r,g,b) are the rgb colors in positive floating points from 0.0 to 1.0.
The ranged values will be converted to the rgb representation, that is, 0.0 -> 0 and 1.0 -> 255. This is the light color.
All light colors are standardized in this same way.

### Objects
- **"type":** "sphere", "plane", "cylinder", "cone" or "cuboid". This is the type of the object.
- **"transformation":** Representation of the 3x3 matrix that specifies the geometrical transformation of the object.
  All fields are represented in floating point values.
  - **"translation":** {"x": x, "y": y,"z": z}, (x,y,z) is the position of the center of the object in the scene.
  - **"rotation":** {"x": x, "y": y,"z": z}: given in a percentage of the rotation, that is, 0.0 = no rotation and 1.0 = 2*pi.
  - **"scale":** {"x": x, "y": y,"z": z}: positive values for the scale of the object. The value 1.0 is the regular size,
  values < 1 reduce the object size and values > 1 increase the object size.
- **"material":** Contains definitions for everything related to the material class of the object.
  - **"reflectivity":** float value ranged from 0.0 to 1.0 for the calculation of the reflection color. A reflectivity of
    0.0 means a non-reflective object and a reflectivity of 1.0 means a completely reflective object.
  - **"shininess":** Not ranged positive float value for the calculation of the specular color. The higher, the most the
  lights have a strong reflection.
  - **"translucency":** float value ranged from 0.0 to 1.0 for the calculation of the translucent color. A translucency of
  0.0 means an opaque object and a translucency of 1.0 means a completely transparent object.
  - **"indexOfRefraction":** Not ranged positive float value for the calculation of the translucent color.
  Represents the index of refraction of the object relative to the scene ambient (if = 1.0, same index).
  - **"texture":** defined in the following subsection.

#### Textures
Textures have different fields for each type. Their colors will be used for the calculation of the diffuse color.

##### Flat
A simple texture of one homogenous color without any special behavior.
- **"type":** "flat".
- **"color":** {"red": r, "green": g,"blue": b}, in the standard representation.

##### Checker
A texture of two homogenous colors without any special behavior, displayed in a checker pattern.
- **"type":** "checker".
- **"transformationUV":** Representation of the 2x2 matrix that specifies the geometrical transformation of the texture.
  All fields are represented in floating point values.
  - **"translation":** {"x": x, "y": y}, the position of the center of the texture.
  - **"rotation":** Given in a percentage of the rotation, that is, 0.0 = no rotation and 1.0 = 2*pi.
  - **"scale":** {"x": x, "y": y}, the scale of the texture.
- **"color1":** {"red": r, "green": g,"blue": b}, in the standard representation.
- **"color2":** {"red": r, "green": g,"blue": b}, in the standard representation.

##### Gradient
Various colors are displayed in a gradient pattern.
- **"type":** "gradient".
- **"stops":** List of stops for the gradient (at least two). Each stop is represented by two values:
  - **"position":** Floating point ranged from 0.0 to 1.0 with the position of the color in the gradient.
  - **"color":** {"red": r, "green": g,"blue": b}, in the standard representation.

### Object Composition
- "type": "composition".
- "transformation": Same definition as in the regular objects.
- "objectList": List with all objects contained in the composition. They are listed in the same way as if they were
specified alone in a file.


## Use case analysis
This section aims to analyze how a user would make use of the library. Two important classes defined in this project
that exemplify this scenario are the "SceneBasic", that makes use of all the library's capabilities to create a scene; 
the "SceneFromJSON", which aims to translate the implementation of all basic functionalities to .json files;
and the "Application" class, showing how we can associate a scene with RTMotors and SDL2 and produce a useful result.

### Context of use
The user wants to use a Ray Tracing application and library with the specifications described in the [About section](#about).

### Demanded functionalities
For the application, the user would expect the following functionalities:
1. Be informed about the status and documentation of the application;
2. Select the motor that will cast the rays into the scene;
3. Manage the files that will be used for composing the scene;
4. Save the rendered image;
5. Get error messages for when something went wrong with the execution of the application;
6. See the image being generated.

For the library, the user would expect the following functionalities:
1. Create a scene object responsible for managing the components rendered in the image;
2. Define the camera specifications;
3. Define lights specifications;
4. Define objects specifications;
5. Define materials for the objects;
6. Define textures for the materials.

## Functional analysis
The following image presents how the functionalities are implemented through functional blocks.
![Functional diagram](/resources/functionalblocks.png)
 
## Architecture description
This section will elaborate on the choice of classes and the general structure of the architecture for the project.

### Root directory
1. **Application class:** Presents a practical use of the library. Makes use of all components from the library in order to
present a coherent use of all it's capabilities.
2. **main.cpp:** Responsible for creating the application class and the adequate methods for generating the executable.

### /userFiles
Here is where the user should place the .json files that will be generated in the scene and retrieve the output image.

### /dependencies
Library folder with all its related dependencies.
1. **Ray class:** Object with vector structure for creating all rays casted into the scene.
2. **HitInformation data structure:** Contains data for intersections between rays and objects.
3. **TileInformation data structure:** Contains data for rendering scene tiles into a SDL surface.
4. **Camera class:** Contains the methods for casting the rays into the scene and adapting the retrieved HitInformation to
   the desired screen specifications.

### /dependencies/RTMotors
1. **RTMotorGeneric class:** Parent class with the methods to be inherited for casting rays into the selected scene and adapt 
the information for SDL tile rendering, using the TileInformation data structure. This class has a pointer to the scene 
class, so that it can access all the information necessary for rendering without depending on the Application class.
Further RTMotors can be developed inheriting from this class.
2. **RTMotorSimple class:** Implementation of a motor that renders one tile at a time.
3. **RTMotorThreaded class:** Implementation of a motor that renders multiple tiles at a time using threads.

### /dependencies/scenes
1. **SceneGeneric class:** Parent class with the methods to be inherited for generating scenes, and a method for verifying 
if a scene has all the necessary parts necessary for rendering. This class has a pointer to all objects contained in it.
Further scene types can be developed inheriting from this class.
2. **SceneBasic class:** An example of a hard-coded scene.
3. **SceneFormJSON class:** An example of a scene generated from data of .json files.

### /dependencies/light
1. **LightGeneric class:** Parent class that inherits the method for calculating the light color in an object. Further light
types can be developed inheriting from this class.
2. **LightPoint class:** Implementation of a light whose origin is punctual.

### /dependencies/objects
1. **GeometricalTransformation class:** Specifies the 3D behaviour of objects transformations in the scene.
2. **ObjectGeneric class:** Parent class that inherits the methods for defining an object, handling 3D and 2D transformations, 
retrieving colors from the material, handling intersections with itself and the bounding box (using the HitInformation data 
structure). Further object types can be developed inheriting from this class.
3. **ObjectSphere class:** 3D sphere implementation.
4. **ObjectPlane class:** 3D plane implementation.
5. **ObjectCylinder class:** 3D cylinder implementation.
6. **ObjectCone class:** 3D cone implementation.
7. **ObjectCuboid class:** 3D cuboid implementation. This class also defines the bounding box geometry and intersection calculation.
8. **ObjectComposition class:** Inherited from the generic class, the composition overrides the way that intersections are
calculated, since the intersection does not happen to its own dimensions, but the dimensions of the objects contained in
their list of objects. Once their list is of generic objects, a composition can have another composition within itself.

### /dependencies/materials
1. **MaterialGeneric class:** Parent class that inherits the methods for defining a material, and provides all color 
calculation methods for the objects. Has a pointer to the used texture to get its diffuse color. Further material types 
can be developed inheriting from this class.
2. **MaterialCompleteSimple class:** Material with all the supported color calculations.

### /dependencies/textures
1. **ColorMap class:** Contains the mathematical base for mapping a linear variable to a color.
2. **TextureGeneric class:** Parent class that inherits the methods for defining a texture, retrieve its color and handle 2D
transformations. Further texture types can be developed inheriting from this class.
3. **TextureFlat class:** Implementation of a texture with one homogenous color.
4. **TextureChecker class:** Implementation of a texture with two homogenous colors in a checker pattern.
5. **TextureGradient class:** Implementation of a texture with colors displayed in a gradient pattern defined by a ColorMap.