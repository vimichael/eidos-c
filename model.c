#include "model.h"
#include "cglm/cglm.h"
#include "cglm/vec3.h"

model model_default() {
  model m = {0};
  glm_vec3_add(m.scale, (vec3){1, 1, 1}, m.scale);
  return m;
}

void model_matrix(model *self, mat4 dest) {
  glm_mat4_identity(dest);
  glm_translate(dest, self->pos);

  glm_rotate(dest, self->rot[0], (vec3){1.0f, 0.0f, 0.0f});
  glm_rotate(dest, self->rot[1], (vec3){0.0f, 1.0f, 0.0f});
  glm_rotate(dest, self->rot[2], (vec3){0.0f, 0.0f, 1.0f});

  glm_scale(dest, self->scale);
}
