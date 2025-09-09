
#define SOKOL_IMPL
#define SOKOL_GLCORE
#define SOKOL_NO_ENTRY
#include "vendor/sokol/sokol_app.h"
#include "vendor/sokol/sokol_gfx.h"
#include "vendor/sokol/sokol_glue.h"

#include "triangle_shader.h"

static struct {
  sg_pass_action pass_action;
  sg_bindings binding;
  sg_pipeline pipeline;
} state;

void init(void) {
  sg_setup(&(sg_desc){
      .environment = sglue_environment(),
  });

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
  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});
  sg_apply_pipeline(state.pipeline);
  sg_apply_bindings(&state.binding);
  sg_draw(0, 3, 1);
  sg_end_pass();
  sg_commit();
}

void cleanup(void) { sg_shutdown(); }

void event(const sapp_event *ev) {}

int main() {

  sapp_run(&(sapp_desc){
      .width = 1280,
      .height = 720,
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
  });

  return 0;
}
