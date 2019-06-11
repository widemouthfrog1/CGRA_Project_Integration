#ifndef SHADERHPP
#define SHADERHPP

class Shader {
    public:
			Shader();
      Shader(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
			GLuint inline getID(){ return programID; }
			void useShader();
    private:
			GLuint programID;
                        GLuint loadShaders(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
};

#endif
