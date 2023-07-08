#include <vector>

class ImportedModel
{
private:
	int numVertices;//��ŵ�ĸ���
	std::vector<glm::vec3> vertices;//��Ŷ��㼯
	std::vector<glm::vec2> textureVecs;
	std::vector<glm::vec3> normalVecs;//��ŷ���
public:
	ImportedModel();//�޲ι��죬��ûʲô��
	ImportedModel(const char *filePath);//�ļ������죬���䱾��ģ��
	int getNumVertices();//���ض�������ĺ���
	std::vector<glm::vec3> getVertices();//���ض��㼯vertices�ĺ���
	std::vector<glm::vec2> getTexures();
	std::vector<glm::vec3> getNormals();//���ط���normalVecs�ĺ���
};

class ModelImporter
{
private:
	std::vector<float> vertVals;//��Ŷ���ĵ��ֵ
	std::vector<float> triangleVerts;//��Ű�������������Ķ��㼯
	std::vector<float> normals;//��Ű�������������ķ���
	std::vector<float> normVals;//��ŷ����ֵ
	std::vector<float> texUvs;//��Ű�������������ķ���
	std::vector<float> texUvVals;//��ŷ����ֵ
public:
	ModelImporter();//�޲ι��죬��ûʲô��
	void parseOBJ(const char *filePath);//���䱾��ģ�ͺ���
	int getNumVertices();//���ض�������ĺ���
	std::vector<float> getVertices();//���ض��㼯vertices�ĺ���
	std::vector<float> getTexUvs();
	std::vector<float> getNormals();//���ط���normalVecs�ĺ���
};