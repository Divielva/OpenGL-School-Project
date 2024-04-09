#pragma once
#include <glad/glad.h>

#include <cstdarg>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Mesh.h"
#include "HSL.h"


//INSERT INTO MAIN.CPP TO RUN
//--------------------------------
//Printing using DrawFromFile
//Just uncomment any of the functions below to run them
//auto f = drawFromFile("function_output.txt", GL_LINE_STRIP, false);
//auto f = drawFromFile("VertexFunc_outp.txt", GL_LINE_STRIP, false);
//auto f = drawFromFile("TwoVariable_outp.txt", GL_TRIANGLES, true);
//--------------------------------


//calling the functions to draw
//Uncomment to run the code, which has to be done at least once
//to create the txt file
//--------------------------------
//auto f = func();
//auto f2 = vertexFunc();
//auto f3 = twoVarFunc(100, .1f);
//
//auto f4 = func2();
//auto f5 = func3();
//
//f.initDraw();
//--------------------------------

//f.draw();

class file_handle
{
	FILE* file;
public:
	file_handle(const char* path, const char* mode)
	{
		fopen_s(&file, path, mode);
	}
	void printf(_Printf_format_string_ char const* const format, ...) const
	{
		va_list args;
		va_start(args, format);
		vfprintf(file, format, args);
		va_end(args);
	}
	~file_handle()
	{
		fclose(file);
	}
};

//=========================================================//
//                                                         //
//                     _----------_,                       //
//                  ,"__          _-:,                     //
//                 /     ""--_--""...:\                    //
//                /          |.........\                   //
//               /           |..........\                  //
//              /,          _'_........./:                 //
//              ! -,     _-"   "- _...,;;:                 //
//              \   -_- "         "-_/;;;;                 //
//               \   \              /;;;;'                 //
//                \   \            /;;;;                   //
//                 '.  \          /;;;'                    //
//                   "-_\________/;;'                      //
//                                                         //
//=========================================================//

//=========================================================//
//            Oblig 1 beginning (Math and Prog)            //
//=========================================================//

//A simple function Class
class func
{
	std::vector<Vertex> vertices;
	std::vector<Vertex> finalVertecies;
	std::vector<unsigned int> indices;

	float fX(float x)
	{
		//A very simple normal function x^2 - 2
		//will be a upside down arch
		return pow(x, 2) - 2;
	}
	float dfx(float x)
	{
		//the derived function, 6x^2
		return 2 * x;
	}
	std::vector<Vertex> getVertices(float min, float max, float step, bool useZ)
	{
		std::vector<Vertex> vertices;
		auto diff = (max + min) / 2;
		while (min < max)
		{
			auto v = Vertex();
			v.position.x = min - diff;
			v.position.y = fX(min);
			if (useZ)
				v.position.z = dfx(min);
			min += step;
			vertices.push_back(v);
		}
		auto v = Vertex();
		v.position.x = min - diff;
		v.position.y = fX(min);
		if (useZ)
			v.position.z = dfx(min);
		vertices.push_back(v);
		return vertices;
	}
	unsigned int vao;
	unsigned int ebo;
	unsigned int vbo;
public:
	func(bool useZ = false)
	{
		vertices = getVertices(-10.f, 10.f, 0.1f, useZ);
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			auto vertex = vertices[i];
			auto nextVertex = vertices[i + 1];
			if (nextVertex.position.y < vertex.position.y)
				vertex.color.r = 1;
			else
				vertex.color.g = 1;
			vertices[i] = vertex;
		}
		FILE* outpFile;
		fopen_s(&outpFile, "function_output.txt", "w");
		fprintf(outpFile, "%d points\n", vertices.size());
		for (auto vertex : vertices)
		{
			fprintf(outpFile, "%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n",
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.color.r, vertex.color.g, vertex.color.b,
				vertex.texture_coord.x, vertex.texture_coord.y);
		}
		fclose(outpFile);
	}

	void initDraw()
	{
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			auto vertex = vertices[i];
			auto nextVertex = vertices[i + 1];
			auto betweenVertex = Vertex();
			finalVertecies.push_back(vertex);
			indices.push_back(finalVertecies.size() - 1);
			betweenVertex.position.x = nextVertex.position.x;
			betweenVertex.position.y = nextVertex.position.y;
			betweenVertex.position.z = vertex.position.z;
			betweenVertex.color.r = vertex.color.r;
			betweenVertex.color.g = vertex.color.g;
			betweenVertex.color.b = vertex.color.b;
			finalVertecies.push_back(betweenVertex);
			indices.push_back(finalVertecies.size() - 1);
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &ebo);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void draw()
	{
		glBufferData(GL_ARRAY_BUFFER, finalVertecies.size() * sizeof(Vertex), finalVertecies.data(), GL_STATIC_DRAW);
		glBindVertexArray(vao);
		glDrawElements(GL_LINES, finalVertecies.size(), GL_UNSIGNED_INT, 0);
	}
};

//A vertex function class
class vertexFunc
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<Vertex> getVertices(float min, float max, float step)
	{
		std::vector<Vertex> vertices;
		hsl hsl{ 0,1,0.5 };
		auto diff = (max + min) / 2;
		while (min < max)
		{
			auto v = Vertex();
			v.position.x = cos(min);
			v.position.y = sin(min);
			v.position.z = min / 10;
			hsl.shift(min);
			auto rgb = hsl.get_rgb();
			v.color.r = rgb[0];
			v.color.g = rgb[1];
			v.color.b = rgb[2];
			min += step;
			vertices.push_back(v);
		}
		auto v = Vertex();
		v.position.x = cos(min);
		v.position.y = sin(min);
		v.position.z = min / 10;
		hsl.shift(min);
		auto rgb = hsl.get_rgb();
		v.color.r = rgb[0];
		v.color.g = rgb[1];
		v.color.b = rgb[2];
		vertices.push_back(v);
		return vertices;
	}
	unsigned int vao;
	unsigned int ebo;
	unsigned int vbo;
public:
	vertexFunc()
	{
		vertices = getVertices(0, 6 * 3.14, 0.1f);
		const file_handle file("VertexFunc_outp.txt", "w");
		file.printf("%d points\n", vertices.size());
		for (auto vertex : vertices)
		{
			file.printf("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n",
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.color.r, vertex.color.g, vertex.color.b,
				vertex.texture_coord.x, vertex.texture_coord.y);
		}
	}

	void initDraw()
	{
		for (int i = 0; i < vertices.size(); i++)
		{
			indices.push_back(i);
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &ebo);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void draw()
	{
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glBindVertexArray(vao);
		glDrawElements(GL_LINE_STRIP, vertices.size(), GL_UNSIGNED_INT, 0);
	}
};

//a two variable function class
class twoVarFunc
{
	int size;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Vertex> getVertices(int size, float step, float offset_x, float offset_y)
	{
		std::vector<Vertex> vertices;
		hsl hsl{ 0,1,0.5 };
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				auto v = Vertex();
				v.position.x = -offset_x + i * step;
				v.position.y = -offset_y + j * step;
				v.position.z = cos(v.position.x + v.position.y);
				hsl.shift(v.position.z);
				auto rgb = hsl.get_rgb();
				v.color.r = rgb[0];
				v.color.g = rgb[1];
				v.color.b = rgb[2];
				vertices.push_back(v);
			}
		}
		return vertices;
	}
	unsigned int vao;
	unsigned int ebo;
	unsigned int vbo;
public:
	twoVarFunc(int size, float step)
	{
		this->size = size;
		const float offset = step * size / 2.f;
		vertices = getVertices(size, step, offset, offset);
		const file_handle file("TwoVariable_outp.txt", "w");
		file.printf("%d points\n", vertices.size());
		for (auto vertex : vertices)
		{
			file.printf("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n",
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.color.r, vertex.color.g, vertex.color.b,
				vertex.texture_coord.x, vertex.texture_coord.y);
		}
	}
	void initDraw()
	{
		for (int i = 0; i < size - 1; i++)
		{
			for (int j = 0; j < size - 1; j++)
			{
				indices.push_back(i * size + j);
				indices.push_back(i * size + j + 1);
				indices.push_back((i + 1) * size + j);
				indices.push_back((i + 1) * size + j);
				indices.push_back((i + 1) * size + j + 1);
				indices.push_back(i * size + j + 1);
			}
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &ebo);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void draw()
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
};

//a class to draw from file, can be called separately from the 
//other functions
class drawFromFile
{
	int size;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int vao;
	unsigned int ebo;
	unsigned int vbo;
	GLenum mode;
	bool optimizedTriangles;
public:
	drawFromFile(const char* filename, GLenum mode = GL_LINES, bool optimizedTriangles = false)
	{
		std::ifstream file(filename);
		std::string line;
		int size = 0;
		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			if (size == 0)
			{
				ss >> size;
				continue;
			}
			auto v = Vertex();
			ss >> v.position.x >> v.position.y >> v.position.z
				>> v.color.r >> v.color.g >> v.color.b
				>> v.texture_coord.x >> v.texture_coord.y;
			vertices.push_back(v);
			if (!optimizedTriangles)
				indices.push_back(indices.size());
		}
		this->mode = mode;
		this->optimizedTriangles = optimizedTriangles;
	}
	void initDraw()
	{
		if (optimizedTriangles)
		{
			int size = sqrt(vertices.size());

			for (int i = 0; i < size - 1; i++)
			{
				for (int j = 0; j < size - 1; j++)
				{
					indices.push_back(i * size + j);
					indices.push_back(i * size + j + 1);
					indices.push_back((i + 1) * size + j);
					indices.push_back((i + 1) * size + j);
					indices.push_back((i + 1) * size + j + 1);
					indices.push_back(i * size + j + 1);
				}
			}
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &ebo);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	void draw()
	{
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glBindVertexArray(vao);
		glDrawElements(this->mode, indices.size(), GL_UNSIGNED_INT, 0);
	}
};

//=========================================================//
//               Oblig 1 end (Math and Prog)               //
//=========================================================//


//                       _-_.
//                   _-' ,^. `-_.
//               ._-'  ,'   `.  `-_
//              !`-__._________`-':::
//              !    /\        /\::::
//              ;   /  \      /..\:::
//              !  /    \    /....\::
//              ! /      \  /......\:
//              ;--.___.  \/_.__.--;;
//               '-_    `:!;;;;;;;'
//                  `--_,:!;;;''
//                      `-!'        


//=========================================================//
//                 Math Oblig 2 beginning                  //
//=========================================================//

class func2
{
	std::vector<Vertex> vertices;
	std::vector<Vertex> finalVertecies;
	std::vector<unsigned int> indices;

	float fX(float x)
	{
		//Just inserting the data
		return 0.03f * pow(x, 2) - 0.2f * x + 0.36f;
	}
	std::vector<Vertex> getVertices(float min, float max, float step)
	{
		std::vector<Vertex> vertices;
		auto diff = (max + min) / 2;
		while (min < max)
		{
			auto v = Vertex();
			v.position.x = min - diff;
			v.position.y = fX(min);
			min += step;
			vertices.push_back(v);
		}
		auto v = Vertex();
		v.position.x = min - diff;
		v.position.y = fX(min);
		vertices.push_back(v);
		return vertices;
	}
	unsigned int vao;
	unsigned int ebo;
	unsigned int vbo;
public:
	func2()
	{
		vertices = getVertices(-10.f, 20.f, 0.1f);
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			auto vertex = vertices[i];
			auto nextVertex = vertices[i + 1];
			if (nextVertex.position.y < vertex.position.y)
				vertex.color.r = 1;
			else
				vertex.color.g = 1;
			vertices[i] = vertex;
		}
		FILE* outpFile;
		fopen_s(&outpFile, "Oppgave_4_6_11.txt", "w");
		fprintf(outpFile, "%d points\n", vertices.size());
		for (auto vertex : vertices)
		{
			fprintf(outpFile, "%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n",
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.color.r, vertex.color.g, vertex.color.b,
				vertex.texture_coord.x, vertex.texture_coord.y);
		}
		fclose(outpFile);
	}

	void initDraw()
	{
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			auto vertex = vertices[i];
			auto nextVertex = vertices[i + 1];
			auto betweenVertex = Vertex();
			finalVertecies.push_back(vertex);
			indices.push_back(finalVertecies.size() - 1);
			betweenVertex.position.x = nextVertex.position.x;
			betweenVertex.position.y = nextVertex.position.y;
			betweenVertex.position.z = vertex.position.z;
			betweenVertex.color.r = vertex.color.r;
			betweenVertex.color.g = vertex.color.g;
			betweenVertex.color.b = vertex.color.b;
			finalVertecies.push_back(betweenVertex);
			indices.push_back(finalVertecies.size() - 1);
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &ebo);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void draw()
	{
		glBufferData(GL_ARRAY_BUFFER, finalVertecies.size() * sizeof(Vertex), finalVertecies.data(), GL_STATIC_DRAW);
		glBindVertexArray(vao);
		glDrawElements(GL_LINES, finalVertecies.size(), GL_UNSIGNED_INT, 0);
	}
};

class func3
{
	std::vector<Vertex> vertices;
	std::vector<Vertex> finalVertecies;
	std::vector<unsigned int> indices;

	float fX(float x)
	{
		//Just inserting the data
		return 11.f / 30 * pow(x, 3) - 29.f / 10 * pow(x, 2) + 86.f / 15 * x + 1;
	}
	std::vector<Vertex> getVertices(float min, float max, float step)
	{
		std::vector<Vertex> vertices;
		auto diff = (max + min) / 2;
		while (min < max)
		{
			auto v = Vertex();
			v.position.x = min - diff;
			v.position.y = fX(min);
			min += step;
			vertices.push_back(v);
		}
		auto v = Vertex();
		v.position.x = min - diff;
		v.position.y = fX(min);
		vertices.push_back(v);
		return vertices;
	}
	unsigned int vao;
	unsigned int ebo;
	unsigned int vbo;
public:
	func3()
	{
		vertices = getVertices(-10.f, 20.f, 0.1f);
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			auto vertex = vertices[i];
			auto nextVertex = vertices[i + 1];
			if (nextVertex.position.y < vertex.position.y)
				vertex.color.r = 1;
			else
				vertex.color.g = 1;
			vertices[i] = vertex;
		}
		FILE* outpFile;
		fopen_s(&outpFile, "Oppgave_4_6_16.txt", "w");
		fprintf(outpFile, "%d points\n", vertices.size());
		for (auto vertex : vertices)
		{
			fprintf(outpFile, "%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n",
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.color.r, vertex.color.g, vertex.color.b,
				vertex.texture_coord.x, vertex.texture_coord.y);
		}
		fclose(outpFile);
	}

	void initDraw()
	{
		for (int i = 0; i < vertices.size() - 1; i++)
		{
			auto vertex = vertices[i];
			auto nextVertex = vertices[i + 1];
			auto betweenVertex = Vertex();
			finalVertecies.push_back(vertex);
			indices.push_back(finalVertecies.size() - 1);
			betweenVertex.position.x = nextVertex.position.x;
			betweenVertex.position.y = nextVertex.position.y;
			betweenVertex.position.z = vertex.position.z;
			betweenVertex.color.r = vertex.color.r;
			betweenVertex.color.g = vertex.color.g;
			betweenVertex.color.b = vertex.color.b;
			finalVertecies.push_back(betweenVertex);
			indices.push_back(finalVertecies.size() - 1);
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &ebo);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void draw()
	{
		glBufferData(GL_ARRAY_BUFFER, finalVertecies.size() * sizeof(Vertex), finalVertecies.data(), GL_STATIC_DRAW);
		glBindVertexArray(vao);
		glDrawElements(GL_LINES, finalVertecies.size(), GL_UNSIGNED_INT, 0);
	}
};

//=========================================================//
//                 Math Oblig 2 End                        //
//=========================================================//
