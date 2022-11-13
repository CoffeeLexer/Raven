//
// Created by tartarus on 11/11/22.
//
#include <glad/glad.h>
#include "VAO.h"

namespace Raven {
    VAO::VAO() {
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);


//        GLuint vertexbuffer;
//        glGenBuffers(1, &vertexbuffer);
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    }
} // Raven