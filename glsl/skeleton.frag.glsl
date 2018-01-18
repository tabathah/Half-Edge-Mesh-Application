#version 150
// ^ Change this to version 130 if you have compatibility issues

uniform vec4 u_Color; // The color with which to render this instance of geometry.

// These are the interpolated values out of the rasterizer, so you can't know
// their specific values without knowing the vertices that contributed to them
in vec4 fs_Pos;
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec4 fs_Col;

out vec4 out_Col; // This is the final output color that you will see on your
                  // screen for the pixel that is currently being processed.

uniform int u_Time;
uniform int u_RenderMode;

void main()
{
    // Material base color (before shading)
    vec4 diffuseColor = fs_Col;

    float diffuseTerm;

    // Calculate the diffuse term for Lambert shading
    diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));

    // Avoid negative lighting values
    diffuseTerm = clamp(diffuseTerm, 0, 1);

    float ambientTerm = 0.5;

    float lightIntensity = max(diffuseTerm + ambientTerm, 1);   //Add a small float value to the color multiplier
                                                        //to simulate ambient lighting. This ensures that faces that are not
                                                        //lit by our point light are not completely black.

    // Compute final shaded color
    if(u_RenderMode == 0)
    {
        out_Col = vec4(diffuseColor.rgb * lightIntensity, diffuseColor.a);
    }
    else
    {
        float factor = (float(u_Time%255))/255.0f + 1.0f;
        out_Col = vec4(abs(factor*(fs_Pos.xyz)), 1);
    }

}
