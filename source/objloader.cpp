#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>

#include <glm/glm.hpp>

#include "objloader.hpp"


bool loadOBJ(
    const char * path,
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
)
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	assert(file);

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
            //uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			assert(matches == 9);
			
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}

	return true;
}


bool nearlyEqual(float a, float b, float epsilon)
{
   const float absA = abs(a);
   const float absB = abs(b);
   const float diff = abs(a - b);

   if (a == b) { // shortcut, handles infinities
       return true;
   } else if (a * b == 0) { // a or b or both are zero
       // relative error is not meaningful here
       return diff < (epsilon * epsilon);
   } else { // use relative error
       return diff / (absA + absB) < epsilon;
   }
}

// Returns true if v1 can be considered equal to v2
bool is_near(float v1, float v2)
{
    return nearlyEqual( v1, v2, 0.01f );
}

// Searches through all already-exported vertices
// for a similar one.
// Similar = same position + same UVs + same normal
bool getSimilarVertexIndex(
    glm::vec3 & in_vertex,
    glm::vec2 & in_uv,
    glm::vec3 & in_normal,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals,
    unsigned short & result
)
{
    // Lame linear search
    for ( unsigned int i=0; i<out_vertices.size(); i++ ){
        if (
            is_near( in_vertex.x , out_vertices[i].x ) &&
            is_near( in_vertex.y , out_vertices[i].y ) &&
            is_near( in_vertex.z , out_vertices[i].z ) &&
            is_near( in_uv.x     , out_uvs     [i].x ) &&
            is_near( in_uv.y     , out_uvs     [i].y ) &&
            is_near( in_normal.x , out_normals [i].x ) &&
            is_near( in_normal.y , out_normals [i].y ) &&
            is_near( in_normal.z , out_normals [i].z )
        ){
            result = i;
            return true;
        }
    }
    // No other vertex could be used instead.
    // Looks like we'll have to add it to the VBO.
    return false;
}

void indexVBO_slow(
    std::vector<glm::vec3> & in_vertices,
    std::vector<glm::vec2> & in_uvs,
    std::vector<glm::vec3> & in_normals,

    std::vector<unsigned short> & out_indices,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
)
{
    // For each input vertex
    const size_t verticesSize = in_vertices.size();
    assert(verticesSize < static_cast<unsigned int>(static_cast<unsigned short>(-1)));
    for ( size_t i=0; i<verticesSize; i++ ){

        // Try to find a similar vertex in out_XXXX
        unsigned short index;
        bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

        if ( found ){ // A similar vertex is already in the VBO, use it instead !
            out_indices.push_back( index );
        }else{ // If not, it needs to be added in the output data.
            out_vertices.push_back( in_vertices[i]);
            out_uvs     .push_back( in_uvs[i]);
            out_normals .push_back( in_normals[i]);
            out_indices .push_back( (unsigned short)out_vertices.size() - 1 );
        }
        const float progress = static_cast<float>(index) / static_cast<float>(verticesSize);
        //std::cout << "(" << progress*100.f << "%)" << std::endl;
    }
}

void loadIndexedOBJ(
    const char * path,

    std::vector<unsigned short> & out_indices,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
)
{
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::cout << "loadOBJ..." << std::endl;
    loadOBJ(path, temp_vertices, temp_uvs, temp_normals);
    std::cout << "indexVBO_slow..." << std::endl;
    indexVBO_slow(temp_vertices, temp_uvs, temp_normals, out_indices, out_vertices, out_uvs, out_normals);
}
