
#define SOKOL_IMPL
#define SOKOL_GLCORE
#define SOKOL_NO_ENTRY
#include "vendor/sokol/sokol_app.h"
#include "vendor/sokol/sokol_gfx.h"
#include "vendor/sokol/sokol_glue.h"

#include "cglm/cglm.h"
#include "model.h"
#include "triangle_shader.h"

static struct {
  sg_pass_action pass_action;
  sg_bindings binding;
  sg_pipeline pipeline;
  model triangle;
} state;

void init(void) {
  sg_setup(&(sg_desc){
      .environment = sglue_environment(),
  });

  state.triangle = model_default();

  float vertices[] = {
      // x    y     z     r     g     b
      0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top coord
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right coord
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // left coord
  };

  state.binding.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
      .data = SG_RANGE(vertices),
  });

  state.pipeline = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = sg_make_shader(triangle_shader_desc(sg_query_backend())),
      .layout = {.attrs = {
                     [ATTR_triangle_pos].format = SG_VERTEXFORMAT_FLOAT3,
                     [ATTR_triangle_color].format = SG_VERTEXFORMAT_FLOAT3,
                 }}});

  state.pass_action = (sg_pass_action){
      .colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                    .clear_value = {0.125490196078, 0.125490196078,
                                    0.125490196078, 1.0}}};
}

void frame(void) {

  state.triangle.pos[2] = -1.0f;
  // state.triangle.pos[2] -= sapp_frame_duration();
  state.triangle.rot[1] -= sapp_frame_duration();
  state.triangle.rot[2] -= sapp_frame_duration();
  state.triangle.scale[0] += 0.01f * sapp_frame_duration();

  mat4 model, view, proj;

  model_matrix(&state.triangle, model);

  glm_mat4_identity(view);

  vec3 eye = {0.0f, 0.0f, 0.0f};
  vec3 center = {0.0f, 0.0f, -1.0f};
  vec3 up = {0.0f, 1.0f, 0.0f};
  glm_lookat(eye, center, up, view);

  float fov = 90.0f;
  float width = sapp_widthf();
  float height = sapp_heightf();
  float aspect = width / height;
  float near = 0.1f;
  float far = 100.0f;

  glm_perspective(fov, aspect, near, far, proj);

  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});
  sg_apply_pipeline(state.pipeline);
  sg_apply_bindings(&state.binding);

  triangle_params_t params = {0};
  glm_mat4_mulN((mat4 *[]){&proj, &view, &model}, 3, params.mvp);

  sg_apply_uniforms(UB_triangle_params, &SG_RANGE(params));

  sg_draw(0, 3, 1);
  sg_end_pass();
  sg_commit();
}

void cleanup(void) { sg_shutdown(); }

void event(const sapp_event *ev) {}

int main() {

  sapp_run(&(sapp_desc){
      .width = 720,
      .height = 720,
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
  });

  return 0;
}
