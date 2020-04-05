#ifndef VOLUMETRICS
void node_halo_gen3_shader(vec4 color,
                           float roughness,
                           float anisotropy,
                           float rotation,
                           vec3 N,
                           vec3 T,
                           out Closure result)
{
  color = vec4(N, 1.0);
  node_bsdf_glossy(color, roughness, N, -1, result);
}
#else
/* Stub Halo Gen3 Shader because it is not compatible with volumetrics. */
#  define node_halo_gen3_shader
#endif
