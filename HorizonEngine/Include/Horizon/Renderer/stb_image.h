#pragma once

// Public wrapper so translation units can include stb_image via the engine include path.
// This includes the top-level copy of stb_image.h (located at project root).

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../../stb_image.h"

#ifdef __cplusplus
}
#endif
