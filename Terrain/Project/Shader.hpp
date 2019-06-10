#ifndef SHADERHPP
#define SHADERHPP

class Shader {
    public:
      Shader(const char * vertexShaderFilepath, const char * fragmentShaderFilepath);
			GLuint inline getID(){ return programID; }
			void useShader();
    private:
			GLuint programID;
			GLuint loadShaders(const char * vertexShaderFilepath, const char * fragmentShaderFilepath);
};

#endif