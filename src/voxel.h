#ifndef VOXEL_H
#define VOXEL_H

#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif

#include "types.h"

struct VoxelData
{
    f32 r, g, b, a;
};

static const VoxelData testModel[4][4][4] = {
    {
        {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 0.0f}
        },
        {
            {1.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 0.0f}
        },
        {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 0.0f}
        },
        {
            {1.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 0.0f}
        }
    },
    {
        {
            {1.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f}
        },
        {
            {1.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f}
        },
        {
            {1.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f}
        },
        {
            {1.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f}
        }
    },
    {
        {
            {1.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f}
        },
        {
            {1.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f}
        },
        {
            {1.0f, 0.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f}
        },
        {
            {1.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f}
        }
    },
    {
        {
            {1.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 0.0f}
        },
        {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 0.0f}
        },
        {
            {1.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 0.0f}
        },
        {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 0.0f}
        }
    }
};

static const f32 vertices[24] = {
    1.0f, 1.0f, 1.0f,   // Right   Top     Front
    0.0f, 1.0f, 1.0f,   // Left    Top     Front
    1.0f, 0.0f, 1.0f,   // Right   Bottom  Front
    0.0f, 0.0f, 1.0f,   // Left    Bottom  Front
    1.0f, 1.0f, 0.0f,   // Right   Top     Back
    0.0f, 1.0f, 0.0f,   // Left    Top     Back
    1.0f, 0.0f, 0.0f,   // Right   Bottom  Back
    0.0f, 0.0f, 0.0f    // Left    Bottom  Back
};

static const u32 indices[36] = {
    0, 1, 2, 3, 1, 2,   // Front
    4, 5, 6, 7, 5, 6,   // Back
    0, 1, 4, 5, 1, 4,   // Top
    2, 3, 6, 7, 3, 6,   // Bottom
    0, 2, 4, 6, 2, 4,   // Right
    1, 3, 5, 7, 3, 5    // Left
};

class VoxelMesh
{
public:
    VoxelMesh();
    ~VoxelMesh();

    void bind();

private:
    u32 VAO;
    u32 EBO;
    u32 VBO;
};

#endif
