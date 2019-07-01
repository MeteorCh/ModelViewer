#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <vector>
#include <regex>
#include <windows.h>
#include "PositionVisitor.h"
#include "Utility.h"
using namespace std;

const char* split = " ";//�ָ��ַ�
bool isFirst = true;
//��б��ת˫б��
void pathConvert_Single2Double(string& s) {
	string::size_type pos = 0;
	while ((pos = s.find('\\', pos)) != string::npos) {
		s.erase(pos, 1);
		s.insert(pos, "/");
	}
}

//�ж��ļ����Ƿ����
bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!  
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!  
	return false;    // this is not a directory!  
}

//�ж��ļ��Ƿ����
bool fileExists(const std::string fileName)
{
	ifstream fin(fileName);
	if (!fin)
		return false;
	return true;
}

/**
 * �ӿ���̨��ȡģ�����ڵ��ļ���
 */
vector<string> readRelativePath()
{
	vector<string> modelPaths;//�洢����Ҫ���ص�ģ�͵ľ���·��
	while (true)
	{
		cout << "������ģ�����ڵ�·��";
		string path;
		std::cin >> path;
		//��˫б��ת���ɷ�б��
		pathConvert_Single2Double(path);
		//�ж�·���Ƿ���/��β
		if (path[path.size() - 1] != '/')
		{
			path.append("/");
		}
		if (dirExists(path))
		{
			cout << "������ģ�����ƣ����ж��ģ���м��ÿո��������" << endl;
			std::cin.clear();
			std::cin.ignore();
			string modelNames;
			getline(std::cin, modelNames);
			std::regex re{ split };
			// ���� std::vector::vector (InputIterator first, InputIterator last,const allocator_type& alloc = allocator_type())
			// ���캯��,����ַ����ָ�
			vector<string> splitResult = std::vector<std::string>{
				std::sregex_token_iterator(modelNames.begin(), modelNames.end(), re, -1),std::sregex_token_iterator()
			};
			//����ģ�͵ľ���·��
			for (string modelName : splitResult)
			{
				string filePath = path + modelName;
				if (fileExists(filePath))
					modelPaths.push_back(filePath);
				else
					cout << "�ļ�\"" + filePath + "\"������" << endl;
			}
		}
		else
		{
			cout << "�����·��������" << endl;
		}
		cout << "������ѡ�1.��������·����2.�������� ��";
		short choice;
		std::cin >> choice;
		if (choice == 2)
			break;
		else if (choice != 1)
			cout << "û�и�ѡ�����������" << endl;
	}
	return modelPaths;
}

/**
 * �ӿ���̨��ȡģ�͵�·��������ģ�͵�·��ȫ�Ǿ���·�����м��ö��ŷָ�
 */
vector<string> readAbsolutPath()
{
	vector<string> results;//�洢����Ҫ���ص�ģ�͵ľ���·��
	cout << "������ģ�͵ľ���·��������ж��ģ�ͣ��м��ÿո������"<<endl;
	if (!isFirst)
	{
		std::cin.clear();
		std::cin.ignore();
	}
	else
		isFirst = false;
	
	string modelPaths;
	getline(std::cin,modelPaths);
	std::regex re{ split };
	// ���� std::vector::vector (InputIterator first, InputIterator last,const allocator_type& alloc = allocator_type())
	// ���캯��,����ַ����ָ�
	vector<string> splitResult = std::vector<std::string>{
		std::sregex_token_iterator(modelPaths.begin(), modelPaths.end(), re, -1),std::sregex_token_iterator()
	};
	for (string modelName : splitResult)
	{
		if (fileExists(modelName))
			results.push_back(modelName);
	}
	return results;
}

/**
 * �ӿ���̨��ȡģ���ļ���·��������ѡ�����·�������·��
 */
vector<string> readModelPath()
{
	vector<string> results;
	while (true)
	{
		vector<string> absolutePath = readAbsolutPath();
		results.insert(results.end(), absolutePath.begin(), absolutePath.end());
		cout << "������ѡ�1.�������룻2.�������� ��";
		short choice;
		std::cin >> choice;
		if (choice == 2)
			break;
		else if (choice != 1)
			cout << "û�и�ѡ�����������" << endl;
	}
	return results;
}

int main()
{
	srand((unsigned)time(NULL));
	while (true)
	{
		vector<string> fileNames = readAbsolutPath();
		if (fileNames.size() == 0)
			cout << "�������Ч���ļ�·��Ϊ0" << endl;
		else
		{
			osgViewer::Viewer viewer;
			osg::ref_ptr<osg::Group> root = new osg::Group;
			osg::ref_ptr<osgDB::Options> options = new osgDB::Options("noRotation");
			for (size_t i = 0; i<fileNames.size(); i++)
			{
				string name = fileNames[i];
				osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(name, options);
				string modelName = Utility::getFileNameFromPath(name);
				PositionVisitor visitor = PositionVisitor(modelName);
				node->accept(visitor);
				root->addChild(visitor.createRandomColorOsgNode(i));
			}
			viewer.setSceneData(root);
			viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
			viewer.addEventHandler(new osgViewer::StatsHandler);
			viewer.setUpViewOnSingleScreen(1);//�����ǵ���Ļ��ʾ
			viewer.run();
		}
		cout << "������ѡ�1.����������ʾģ�ͣ�2.�˳����� ��";
		short choice;
		std::cin >> choice;
		if (choice == 2)
			return 0;
		else if (choice != 1)
			cout << "û�и�ѡ�����������"<<endl;
	}
	return 0;
}
