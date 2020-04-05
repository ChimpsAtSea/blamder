/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 */

#include "../node_shader_util.h"

/* **************** OUTPUT ******************** */

static bNodeSocketTemplate sh_node_halo_gen3_shader_in[] = {
    {SOCK_RGBA, N_("Color"), 0.8f, 0.8f, 0.8f, 1.0f, 0.0f, 1.0f},
    {SOCK_FLOAT, N_("Roughness"), 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_FLOAT, N_("Anisotropy"), 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f},
    {SOCK_FLOAT, N_("Rotation"), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_VECTOR, N_("Normal"), 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, PROP_NONE, SOCK_HIDE_VALUE},
    {SOCK_VECTOR, N_("Tangent"), 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, PROP_NONE, SOCK_HIDE_VALUE},
    {-1, ""},
};

static bNodeSocketTemplate sh_node_halo_gen3_shader_out[] = {
    {SOCK_SHADER, N_("BSDF")},
    {-1, ""},
};

static void node_shader_init_halo_gen3_shader(bNodeTree *UNUSED(ntree), bNode *node)
{
  node->opus_custom_option0 = SHD_HALO_GEN3_ALBEDO_CONSTANT_COLOR;
}

static int node_shader_gpu_halo_gen3_shader(GPUMaterial *mat,
                                            bNode *node,
                                            bNodeExecData *UNUSED(execdata),
                                            GPUNodeStack *in,
                                            GPUNodeStack *out)
{
  if (!in[4].link) {
    GPU_link(mat, "world_normals_get", &in[4].link);
  }

  GPU_material_flag_set(mat, GPU_MATFLAG_GLOSSY);

  return GPU_stack_link(mat, node, "node_halo_gen3_shader", in, out);
}

#define custom_option_albedo opus_custom_option0

static void node_shader_update_halo_gen3_shader(bNodeTree *UNUSED(ntree), bNode *node)
{
  bNodeSocket *sock;
  int albedo_option = node->custom_option_albedo;

  for (sock = node->inputs.first; sock; sock = sock->next) {
    if (STREQ(sock->name, "Roughness")) {
      if (albedo_option == SHD_HALO_GEN3_ALBEDO_CONSTANT_COLOR) {
        sock->flag &= ~SOCK_UNAVAIL;
      }
      else {
        sock->flag |= SOCK_UNAVAIL;
      }
    }
  }
}

/* node type definition */
void register_node_type_sh_halo_gen3_shader(void)
{
  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_HALO_GEN3_SHADER, "Halo Gen3 Shader", NODE_CLASS_SHADER, 0);
  node_type_socket_templates(&ntype, sh_node_halo_gen3_shader_in, sh_node_halo_gen3_shader_out);
  node_type_size_preset(&ntype, NODE_SIZE_MIDDLE);
  node_type_init(&ntype, node_shader_init_halo_gen3_shader);
  node_type_storage(&ntype, "", NULL, NULL);
  node_type_gpu(&ntype, node_shader_gpu_halo_gen3_shader);
  node_type_update(&ntype, node_shader_update_halo_gen3_shader);

  nodeRegisterType(&ntype);
}
