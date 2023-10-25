#include <fstream>
#include <sstream>
#include <cstring>
#include <glm\glm.hpp>
#include "ImportedModel.h"
using namespace std;

ImportedModel::ImportedModel() {}

ImportedModel::ImportedModel(const char *filePath) {
	ModelImporter modelImporter = ModelImporter();
	modelImporter.parseOBJ(filePath);//调用ModelImporter类的方法实现将点和法向按照顺序再存储
	numVertices = modelImporter.getNumVertices();//获取顶点的个数
	std::vector<float> verts = modelImporter.getVertices();//获取顶点
	std::vector<float> textures = modelImporter.getTexUvs();//获取法向
	std::vector<float> normals = modelImporter.getNormals();//获取法向

	for (int i = 0; i < numVertices; i++) {//将点和法向加入点集和法向集
		vertices.push_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
		normalVecs.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
	}
	for (int i = 0; i < numVertices; i++) {//将点和法向加入点集和法向集
		textureVecs.push_back(glm::vec2(textures[i * 2], textures[i * 2 + 1]));
	}
}
//返回类中参数的各个函数
int ImportedModel::getNumVertices() { return numVertices; }
std::vector<glm::vec3> ImportedModel::getVertices() { return vertices; }
std::vector<glm::vec2> ImportedModel::getTexures() { return textureVecs; }
std::vector<glm::vec3> ImportedModel::getNormals() { return normalVecs; }

void ImportedModel::tansformModel(glm::vec3 trans)
{
	for (auto& item : vertices) {
		item += trans;
	}
}

// ---------------------------------------------------------------

ModelImporter::ModelImporter() {}

//实现将点和法向按照顺序再存储
void ModelImporter::parseOBJ(const char *filePath) {
	float x, y, z;
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		if (line.compare(0, 2, "v ") == 0) {
			stringstream ss(line.erase(0, 1));
			ss >> x; ss >> y; ss >> z;
			vertVals.push_back(x);
			vertVals.push_back(y);
			vertVals.push_back(z);
		}
		if (line.compare(0, 2, "vt") == 0) {
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y;
			texUvVals.push_back(x);
			texUvVals.push_back(y);
		}
		if (line.compare(0, 2, "vn") == 0) {
			stringstream ss(line.erase(0, 2));
			ss >> x; ss >> y; ss >> z;
			normVals.push_back(x);
			normVals.push_back(y);
			normVals.push_back(z);
		}
		if (line.compare(0, 2, "f ") == 0) {
			string oneCorner, v, t, n;
			stringstream ss(line.erase(0, 2));
			for (int i = 0; i < 3; i++) {
				getline(ss, oneCorner, ' ');
				stringstream oneCornerSS(oneCorner);
				getline(oneCornerSS, v, '/');
				getline(oneCornerSS, t, '/');
				getline(oneCornerSS, n, '/');
				
				int vertRef = (stoi(v) - 1) * 3;
				int texRef = (stoi(t) - 1) * 2;
				int normRef = (stoi(n) - 1) * 3;
				
				triangleVerts.push_back(vertVals[vertRef]);
				triangleVerts.push_back(vertVals[vertRef + 1]);
				triangleVerts.push_back(vertVals[vertRef + 2]);

				texUvs.push_back(texUvVals[texRef]);
				texUvs.push_back(texUvVals[texRef + 1]);

				normals.push_back(normVals[normRef]);
				normals.push_back(normVals[normRef + 1]);
				normals.push_back(normVals[normRef + 2]);
			}
		}
	}
}
//获取参数的各个函数
int ModelImporter::getNumVertices() { return (triangleVerts.size()/3); }
std::vector<float> ModelImporter::getVertices() { return triangleVerts; }
std::vector<float> ModelImporter::getTexUvs() { return texUvs; }
std::vector<float> ModelImporter::getNormals() { return normals; }