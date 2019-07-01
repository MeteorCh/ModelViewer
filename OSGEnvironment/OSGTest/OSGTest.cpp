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
bool isFirst = true;
//单斜杠转双斜杠
void pathConvert_Single2Double(string& s) {
	string::size_type pos = 0;
	while ((pos = s.find('\\', pos)) != string::npos) {
		s.erase(pos, 1);
		s.insert(pos, "/");
	}
}

//判断文件夹是否存在
bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!  
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!  
	return false;    // this is not a directory!  
}

//判断文件是否存在
bool fileExists(const std::string fileName)
{
	ifstream fin(fileName);
	if (!fin)
		return false;
	return true;
}

/**
 * 从控制台读取模型所在的文件夹
 */
vector<string> readRelativePath()
{
	vector<string> modelPaths;//存储所有要加载的模型的绝对路径
	while (true)
	{
		cout << "请输入模型所在的路径";
		string path;
		std::cin >> path;
		//将双斜杠转换成反斜杠
		pathConvert_Single2Double(path);
		//判断路径是否以/结尾
		if (path[path.size() - 1] != '/')
		{
			path.append("/");
		}
		if (dirExists(path))
		{
			cout << "请输入模型名称（如有多个模型中间用空格隔开）：" << endl;
			std::cin.clear();
			std::cin.ignore();
			string modelNames;
			getline(std::cin, modelNames);
			std::regex re{ split };
			// 调用 std::vector::vector (InputIterator first, InputIterator last,const allocator_type& alloc = allocator_type())
			// 构造函数,完成字符串分割
			vector<string> splitResult = std::vector<std::string>{
				std::sregex_token_iterator(modelNames.begin(), modelNames.end(), re, -1),std::sregex_token_iterator()
			};
			//构造模型的绝对路径
			for (string modelName : splitResult)
			{
				string filePath = path + modelName;
				if (fileExists(filePath))
					modelPaths.push_back(filePath);
				else
					cout << "文件\"" + filePath + "\"不存在" << endl;
			}
		}
		else
		{
			cout << "输入的路径不存在" << endl;
		}
		cout << "请输入选项：1.继续输入路径；2.结束输入 ：";
		short choice;
		std::cin >> choice;
		if (choice == 2)
			break;
		else if (choice != 1)
			cout << "没有该选项，请重新输入" << endl;
	}
	return modelPaths;
}

/**
 * 从控制台读取模型的路径：所有模型的路径全是绝对路径，中间用逗号分隔
 */
vector<string> readAbsolutPath()
{
	vector<string> results;//存储所有要加载的模型的绝对路径
	cout << "请输入模型的绝对路径（如果有多个模型，中间用空格隔开）"<<endl;
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

/**
 * 从控制台读取模型文件的路径，可以选择绝对路径和相对路径
 */
vector<string> readModelPath()
{
	vector<string> results;
	while (true)
	{
		vector<string> absolutePath = readAbsolutPath();
		results.insert(results.end(), absolutePath.begin(), absolutePath.end());
		cout << "请输入选项：1.继续输入；2.结束输入 ：";
		short choice;
		std::cin >> choice;
		if (choice == 2)
			break;
		else if (choice != 1)
			cout << "没有该选项，请重新输入" << endl;
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
				PositionVisitor visitor = PositionVisitor(modelName);
				node->accept(visitor);
				root->addChild(visitor.createRandomColorOsgNode(i));
			}
			viewer.setSceneData(root);
			viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
			viewer.addEventHandler(new osgViewer::StatsHandler);
			viewer.setUpViewOnSingleScreen(1);//这里是单屏幕显示
			viewer.run();
		}
		cout << "请输入选项：1.继续输入显示模型；2.退出程序 ：";
		short choice;
		std::cin >> choice;
		if (choice == 2)
			return 0;
		else if (choice != 1)
			cout << "没有该选项，请重新输入"<<endl;
	}
	return 0;
}
