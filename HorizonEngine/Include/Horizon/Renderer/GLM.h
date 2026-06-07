// Small wrapper to include GLM while undefining a few one-letter macros that
// can break GLM's template parsing on some projects.
#pragma once

#ifdef Q
#undef Q
#endif
#ifdef vec
#undef vec
#endif
#ifdef mat
#undef mat
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
