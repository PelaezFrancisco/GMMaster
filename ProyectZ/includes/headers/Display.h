
int vao_vertex(int vaoi);
int vertex_program(const char* vertexShaderSource);
int fragment_program(const char* fragmentShaderSource);
int link_program(int vertexShaderSource, int fragmentShaderSource);
int prep_vertex(int frag_i);