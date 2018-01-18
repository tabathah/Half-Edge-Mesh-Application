# Half-Edge-Mesh-Application

Stores and displays meshes in a half-edge mesh data structure. Loads in OBJ files and converts them into half-edge meshes. Mesh can be traversed by selecting vertices, edges, or faces in the menus on the GUI or by using the following keys:

N: selects the next half-edge of the currently selected half-edge
M: selects the symmetric half-edge of the currently selected half-edge
F: selects the face that the currently selected half-edge belongs to
V: selects the vertex that the currently selected half-edge points to
H: selects a half-edge pointing to the currently selected vertex

## Features for Modifying Mesh

- Add a vertex at the midpoint of selected edge

- Triangulate selected face

- Subdivide mesh (using Catmull-Clark subdivision)

- Extrude selected face by one unit in the direction of the face's normal

- Insert edge loop across the selected edge

- Load json file holding skeleton information, bind the mesh to the skeleton using the "Skin" button, and modify the mesh by selecting then translating and rotating the joints 