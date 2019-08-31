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

//�ж��ļ��Ƿ����
bool fileExists(const std::string fileName)
{
	ifstream fin(fileName);
	if (!fin)
		return false;
	return true;
}

/**
 * �ӿ���̨��ȡģ�͵�·��������ģ�͵�·��ȫ�Ǿ���·�����м��ö��ŷָ�
 */
vector<string> readAbsolutPath()
{
	vector<string> results;//�洢����Ҫ���ص�ģ�͵ľ���·��
	cout << "������ģ�͵ľ���·��������ж��ģ�ͣ��м��ÿո������"<<endl;
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

int main()
{
	srand((unsigned)time(NULL));
	int textSize = 0;
	cout << "��������ʾ���ֵ������С��";
	cin >> textSize;
	if (textSize <= 0 || textSize > 50)
		textSize = 5;
	cin.ignore();
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
				PositionVisitor visitor = PositionVisitor(modelName,textSize);
				node->accept(visitor);
				root->addChild(visitor.createRandomColorOsgNode(i));
			}
			viewer.setSceneData(root);
			viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
			viewer.addEventHandler(new osgViewer::StatsHandler);
			viewer.setUpViewOnSingleScreen(1);//�����ǵ���Ļ��ʾ
			viewer.run();
		}
	}
	return 0;
}
