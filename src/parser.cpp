/* ---- Definitions ---- */
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifdef __unix
#define fopen_s(pFile, filename, mode) ((*(pFile)) = fopen((filename), (mode))) == NULL
#endif

/* ---- Header Files ---- */
#include "headers/parser.h"

// temporary vectors to store the vertex contents of the .obj file
std::vector<glm::vec3> temp_vec_v_positions;  // vertex positions (x, y, z)
std::vector<glm::vec2> temp_vec_v_textures;   // vertex textures (u, v)
std::vector<glm::vec3> temp_vec_v_normals;    // vertex normals (x, y, z)

// indices from the face/triangle values
std::vector<unsigned int> v_pos_indices; // indices for vertex positions
std::vector<unsigned int> v_tex_indices; // indices for vertex textures
std::vector<unsigned int> v_nor_indices; // indices for vertex normals

// final vectors with vertex content in the correct order as per the face/triangle indices values
std::vector<glm::vec3> v_positions; // all the positions in the correct order
std::vector<glm::vec2> v_textures; // all the textures in the correct order
std::vector<glm::vec3> v_normals; // all the normals in the correct order

// output tuple
std::pair<float *, unsigned int> parse_OBJ(const char *file_path)
{
    process_file_OBJ(file_path);
    process_data_OBJ();

    unsigned int vertices_triangles = v_positions.size();
    printf("TRIANGLES: %u\n", vertices_triangles);

    return std::make_pair(create_vertices(), vertices_triangles);
}

void process_file_OBJ(const char *file_path)
{
    printf("INFO: Loading OBJ file: %s...\n", file_path);

    // open the .obj file
    FILE *obj_file;
    fopen_s(&obj_file, file_path, "r");

    // error checking the integrity of the file
    if(obj_file == nullptr)
    {
        printf("ERROR: Cannot Open File at: %s.\n", file_path);
        return;
    }

    // PROCESSING THE FILE
    // read the file, line by line, until the end
    while(true)
    {
        // assume that each line won’t exceed 128 chars
        char line_buffer[128];

        // read the first word of the line
        int word_buffer = fscanf(obj_file, "%s", line_buffer);

        // exit loop when End Of File
        if (word_buffer == EOF)
        {
            break;
        }

        // else: parse the line
        // deal with the vertices first (v, vt, vn) values, and add them to respective temp vectors

        // if the first word of the line is “v”, then the rest has to be 3 floats for the vertex position
        // so create a glm::vec3 out of them, and add it to the temp vector.
        if (strcmp(line_buffer, "v") == 0)
        {
            glm::vec3 vertex_pos;
            fscanf(obj_file, "%f %f %f\n", &vertex_pos.x, &vertex_pos.y, &vertex_pos.z);

            temp_vec_v_positions.push_back(vertex_pos);
        }
        // if it’s not a “v” but a “vt”, then the rest has to be 2 floats for the vertex texture
        // so create a glm::vec2 out of them, and add it to the temp vector.
        else if(strcmp(line_buffer, "vt") == 0)
        {
            glm::vec2 vertex_tex;
            fscanf(obj_file, "%f %f\n", &vertex_tex.x, &vertex_tex.y);

            temp_vec_v_textures.push_back(vertex_tex);
        }
        // if it’s not a “v” or a “vt”, it must be a "vn", and the rest has to be 3 floats for the vertex normals
        // so create a glm::vec3 out of them, and add it to the temp vector.
        else if(strcmp(line_buffer, "vn") == 0)
        {
            glm::vec3 vertex_nor;
            fscanf(obj_file, "%f %f %f\n", &vertex_nor.x, &vertex_nor.y, &vertex_nor.z);

            temp_vec_v_normals.push_back(vertex_nor);
        }
        // now the “f”, which is more difficult.
        // code is in fact very similar to the previous one, except there is more data to read.

        // so what we do here is simply change the “shape” of the data.
        // from a string, we construct a set of std::vectors.
        // But it’s not enough, we have to put this into a form that OpenGL understands.
        // By removing the indexes and having plain glm::vec3 instead.
        // This operation is called indexing.
        else if (strcmp(line_buffer, "f") == 0)
        {
            //std::string vertex1, vertex2, vertex3;
            unsigned int v_pos_index[3], v_tex_index[3], v_nor_index[3];

            int matches = fscanf(
                    obj_file,
                    "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                    &v_pos_index[0], &v_tex_index[0], &v_nor_index[0],
                    &v_pos_index[1], &v_tex_index[1], &v_nor_index[1],
                    &v_pos_index[2], &v_tex_index[2], &v_nor_index[2]
            );

            if (matches != 9)
            {
                printf("ERROR: File Cannot be Parsed.\n");
                printf("INFO: Ensure Only Triangles Are Used in OBJ File.\n");
                return;
            }

            // Vertex Positions
            v_pos_indices.push_back(v_pos_index[0]);
            v_pos_indices.push_back(v_pos_index[1]);
            v_pos_indices.push_back(v_pos_index[2]);

            // Vertex Textures
            v_tex_indices.push_back(v_tex_index[0]);
            v_tex_indices.push_back(v_tex_index[1]);
            v_tex_indices.push_back(v_tex_index[2]);

            // Vertex Normals
            v_nor_indices.push_back(v_nor_index[0]);
            v_nor_indices.push_back(v_nor_index[1]);
            v_nor_indices.push_back(v_nor_index[2]);
        }
    }

    printf("INFO: Successfully Loaded File!\n");
}

void process_data_OBJ()
{
    // PROCESSING THE DATA
    // Iterate through each vertex (each v/vt/vn) of each triangle (each face line with a “f”)

    // For each vertex position of each triangle
    for(unsigned int i = 0; i < v_pos_indices.size(); i++)
    {
        // the index to the vertex position is v_pos_indices[i]
        unsigned int v_pos_index = v_pos_indices[i];

        // so the vertex position (v) would be temp_vector[vertex_index - 1]
        // there is a -1 because C indexing starts at 0 and OBJ indexing starts at 1
        glm::vec3 v = temp_vec_v_positions[v_pos_index - 1];

        // this makes the position of our new vertex
        v_positions.push_back(v);
    }

    // For each vertex texture of each triangle
    for(unsigned int i = 0; i < v_tex_indices.size(); i++)
    {
        unsigned int v_tex_index = v_tex_indices[i];
        glm::vec2 vt = temp_vec_v_textures[v_tex_index - 1];

        v_textures.push_back(vt);
    }

    // For each vertex normal of each triangle
    for(unsigned int i = 0; i < v_nor_indices.size(); i++)
    {
        unsigned int v_nor_index = v_nor_indices[i];
        glm::vec3 vn = temp_vec_v_normals[v_nor_index - 1];

        v_normals.push_back(vn);
    }

    printf("INFO: Successfully Parsed Data!\n");
}

float *create_vertices()
{
    // Size of v vectors are number of "f" lines * 3, and contain the 3 vectors of the face

    float *vertices = (float *) malloc((sizeof(float)) * (v_positions.size() * 8));

    int i = 0;
    while (i < v_positions.size() * 8)
    {
        for (int j = 0; j < 3; j++)
        {
            vertices[i] = v_positions[i / 8][j];
            ++i;
        }
        for (int j = 0; j < 2; j++)
        {
            vertices[i] = v_textures[i / 8][j];
            ++i;
        }
        for (int j = 0; j < 3; j++)
        {
            vertices[i] = v_normals[i / 8][j];
            ++i;
        }
    }

    printf("INFO: Successfully Created Vertices Array!\n");
    printf("INFO: ------------------------------------\n");

    // Test Print for Debugging
//    printf("INFO: v: %f, vt: %f, vn: %f\n", v_positions[29][0], v_textures[0][1], v_normals[0][2]);

//    printf("      //pos                                 //tex                    //nor\n");

//    for (int k = 0; k < v_positions.size() * 8; k += 8)
//    {
//        printf("%d:    %f, %f, %f,      %f, %f,       %f, %f, %f,\n",
//               1 + (k / 8),
//               vertices[k], vertices[k + 1], vertices[k + 2],
//               vertices[k + 3], vertices[k + 4],
//               vertices[k + 5], vertices[k + 6], vertices[k + 7]
//        );
//    }

    // clear all the vectors
    temp_vec_v_positions.clear();
    temp_vec_v_textures.clear();
    temp_vec_v_normals.clear();

    v_pos_indices.clear();
    v_tex_indices.clear();
    v_nor_indices.clear();

    v_positions.clear();
    v_textures.clear();
    v_normals.clear();

    return vertices;
}

