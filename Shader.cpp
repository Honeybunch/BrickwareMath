#define _CRT_SECURE_NO_WARNINGS

#include "Shader.h"
#include "Utils.h"
#include "GameObject.h"

#include "Game.h"

Shader::Shader(char* vertexShaderFileName, char* pixelShaderFileName)
{
	//TODO: Strip existing file extension if any
	char* trimmedVertexFileName = Utils::trimToLastChar(vertexShaderFileName, '/');
	char* trimmedPixelFileName = Utils::trimToLastChar(pixelShaderFileName, '/');

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		loadHLSL(trimmedVertexFileName, trimmedPixelFileName);
	else
		loadGLSL(vertexShaderFileName, pixelShaderFileName);
#elif defined(USE_D3D_ONLY)
	loadHLSL(trimmedVertexFileName, trimmedPixelFileName);
#else
	loadGLSL(vertexShaderFileName, pixelShaderFileName);
#endif
}

void Shader::bindShader()
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		bindHLSL();
	else
		bindGLSL();
#elif defined(USE_D3D_ONLY)
	bindHLSL();
#else
	bindGLSL();
#endif
}

void Shader::freeShader()
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		freeHLSL();
	else
		freeGLSL();
#elif defined(USE_D3D_ONLY)
	freeHLSL();
#else
	freeGLSL();
#endif
}

#ifndef USE_D3D_ONLY

void Shader::bindGLSL(){ glUseProgram(shaderProgram); }
void Shader::freeGLSL(){ glUseProgram(shaderProgram); }

bool Shader::loadGLSL(char* vertexShaderFileName, char* pixelShaderFileName)
{
	//Assume strings have no file extension

	//+5 for .glsl +1 for null terminator
	char* glslVertexFileName = new char[strlen(vertexShaderFileName) + 6];
	strcpy(glslVertexFileName, vertexShaderFileName);
	strcat(glslVertexFileName, ".glsl");

	char* glslPixelFileName = new char[strlen(pixelShaderFileName) + 6];
	strcpy(glslPixelFileName, pixelShaderFileName);
	strcat(glslPixelFileName, ".glsl");

	// Read in shader source
	char* vertexShaderSource;
	char* pixelShaderSource;

	// Create the shader 
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLuint program;

	// Read in shader source
	vertexShaderSource = Utils::textFileRead(glslVertexFileName);
	if (!vertexShaderSource) {
		cerr << "Error reading vertex shader " << glslVertexFileName << endl;
		shaderProgram = 0;
		return false;
	}
	pixelShaderSource = Utils::textFileRead(glslPixelFileName);
	if (!pixelShaderSource) {
		cerr << "Error reading fragment shader " << glslPixelFileName << endl;
		shaderProgram = 0;
		return false;
	}

	//We need the source in a GLchar
	const GLchar* vertexShaderSourceGL = vertexShaderSource;
	const GLchar* fragmentShaderSourceGL = pixelShaderSource;

	glShaderSource(vertexShader, 1, &vertexShaderSourceGL, NULL);
	glShaderSource(pixelShader, 1, &fragmentShaderSourceGL, NULL);

	// Compile the shader
	glCompileShader(vertexShader);
	glCompileShader(pixelShader);
	Utils::printShaderInfoLog(vertexShader);
	Utils::printShaderInfoLog(pixelShader);

	// Create the program -- attaching your shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, pixelShader);
	Utils::printProgramInfoLog(program);

	// Link the program
	glLinkProgram(program);
	Utils::printProgramInfoLog(program);

	//Delete unneeded strings
	delete vertexShaderSource;
	delete pixelShaderSource;
	delete glslVertexFileName;
	delete glslPixelFileName;

	//Save program
	shaderProgram = program;

	return true;
}

#endif
#ifdef D3D_SUPPORT

void Shader::bindHLSL()
{
	
}
void Shader::freeHLSL(){}

bool Shader::loadHLSL(char* vertexShaderFileName, char* pixelShaderFileName)
{
	//Assume strings have no file extension
	char* hlslVertexFileName = new char[strlen(vertexShaderFileName) + 5];
	strcpy(hlslVertexFileName, vertexShaderFileName);
	strcat(hlslVertexFileName, ".cso");

	char* hlslPixelFileName = new char[strlen(pixelShaderFileName) + 5];
	strcpy(hlslPixelFileName, pixelShaderFileName);
	strcat(hlslPixelFileName, ".cso");

	//Convert to wide strings because DirectX asked nicely
	WCHAR* hlslVertexWideString = Utils::stringToWideString(hlslVertexFileName);
	WCHAR* hlslPixelWideString = Utils::stringToWideString(hlslPixelFileName);

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	if (D3DReadFileToBlob(hlslVertexWideString, &vsBlob) != S_OK)
	{
		cerr << "Error reading vertex shader " << hlslVertexFileName << endl;
		return false;
	}

	// Create the shader on the device
	HR(Game::device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader));

	// Before cleaning up the data, create the input layout
	HR(Game::device->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout));

	// Clean up
	ReleaseMacro(vsBlob);

	// Load Pixel Shader ---------------------------------------
	ID3DBlob* psBlob;
	D3DReadFileToBlob(hlslPixelWideString, &psBlob);
	if (false)
	{
		cerr << "Error reading pixel shader " << hlslPixelFileName << endl;
		return false;
	}

	// Create the shader on the device
	HR(Game::device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&pixelShader));

	// Clean up
	ReleaseMacro(psBlob);

	//Free unneeded data
	delete hlslVertexFileName;
	delete hlslPixelFileName;
	delete hlslVertexWideString;
	delete hlslPixelWideString;

	return true;
}

#endif

Shader::~Shader()
{
#ifdef D3D_SUPPORT
	ReleaseMacro(vertexShader);
	ReleaseMacro(pixelShader);
	ReleaseMacro(inputLayout);
#endif
}