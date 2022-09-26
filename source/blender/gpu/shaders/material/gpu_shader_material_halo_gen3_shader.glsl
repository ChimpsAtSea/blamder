
void node_halo3_gen3(vec4 color,
                                float scale,
                                vec3 radius,
                                float ior,
                                float anisotropy,
                                vec3 N,
                                float weight,
                                float do_sss,
                                out Closure result)
{
  color = vec4(N, 1.0);
  N = safe_normalize(N);

  ClosureDiffuse diffuse_data;
  diffuse_data.weight = weight;
  diffuse_data.color = color.rgb;
  diffuse_data.N = N;
  diffuse_data.sss_radius = radius * scale;
  diffuse_data.sss_id = uint(do_sss);

  result = closure_eval(diffuse_data);
}
