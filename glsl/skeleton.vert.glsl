#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

uniform mat4 u_ViewProj;

uniform mat4 u_BindMats[100];
uniform mat4 u_TransMats[100];

in ivec2 vs_Matrix_Ids;
in vec2 vs_Weights;

uniform mat4 u_Model;
uniform mat4 u_ModelInvTr;

in vec4 vs_Pos;
in vec4 vs_Nor;
in vec4 vs_Col;

out vec4 fs_Pos;
out vec4 fs_Nor;
out vec4 fs_LightVec;
out vec4 fs_Col;

const vec4 lightPos = vec4(5, 5, 3, 1);

void main()
{
    fs_Col = vs_Col/5.0;

    mat3 invTranspose = mat3(u_ModelInvTr);
    fs_Nor = vec4(invTranspose * vec3(vs_Nor), 0);

    vec4 modelposition = u_Model * vs_Pos;
    fs_LightVec = lightPos - modelposition;

    fs_Pos = u_ViewProj*
                 (vs_Weights[0] * u_TransMats[vs_Matrix_Ids[0]] * u_BindMats[vs_Matrix_Ids[0]] * vs_Pos
                  +
                  vs_Weights[1] * u_TransMats[vs_Matrix_Ids[1]] * u_BindMats[vs_Matrix_Ids[1]] * vs_Pos);

    gl_Position = fs_Pos;
}
