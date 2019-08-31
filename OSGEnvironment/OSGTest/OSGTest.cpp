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

const char* split = " ";//分隔字符

//判断文件是否存在
bool fileExists(const std::string fileName)
{
	ifstream fin(fileName);
	if (!fin)
		return false;
	return true;
}

/**
 * 从控制台读取模型的路径：所有模型的路径全是绝对路径，中间用逗号分隔
 */
vector<string> readAbsolutPath()
{
	vector<string> results;//存储所有要加载的模型的绝对路径
	cout << "请输入模型的绝对路径（如果有多个模型，中间用空格隔开）"<<endl;
	string modelPaths;
	getline(std::cin,modelPaths);
	std::regex re{ split };
	// 调用 std::vector::vector (InputIterator first, InputIterator last,const allocator_type& alloc = allocator_type())
	// 构造函数,完成字符串分割
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
	cout << "请输入提示文字的字体大小：";
	cin >> textSize;
	if (textSize <= 0 || textSize > 50)
		textSize = 5;
	cin.ignore();
	while (true)
	{
		vector<string> fileNames = readAbsolutPath();
		if (fileNames.size() == 0)
			cout << "输入的有效的文件路径为0" << endl;
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
			viewer.setUpViewOnSingleScreen(1);//这里是单屏幕显示
			viewer.run();
		}
	}
	return 0;
}
