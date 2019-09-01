#include "stdafx.h"
#include "Utility.h"
#include <iostream>
#include <osgText/Font>
Utility::Utility()
{
}


Utility::~Utility()
{
}

/**
 * 比较两个三维向量是否相同
 */
bool Utility::isVec3Same(osg::Vec3 v1, osg::Vec3 v2)
{
	return (v1.x() == v2.x()) && (v1.y() == v2.y()) && (v1.z() == v2.z());
}

/**
 * 用一个字符替换原字符中的另一个字符
 */
void Utility::string_replace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}

/**
 * 从路径中获取文件名（不包括后缀名）
 */
string Utility::getFileNameFromPath(string path)
{
	if(path.empty())
	{
		return "";
	}
	string_replace(path, "/", "\\");
	std::string::size_type iPos = path.find_last_of('\\') + 1;
	std::string::size_type dPos = path.find_last_of('.') + 1;
	if (dPos == 0)
		dPos = path.length();
	return path.substr(iPos, dPos - iPos-1);
}

/**
* 创建text
* parameter1：显示的文字信息
* parameter2：文字的大小
* parameter3：字体
* parameter4：文字位置
* parameter5：direction文字的朝向，0左1右2前4后
* parameter6：文字颜色
*/
osg::ref_ptr<osgText::Text> Utility::createText(string info, int textSize, osg::ref_ptr<osgText::Font> font, osg::Vec3 position, int direction, osg::Vec4 textColor)
{
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setFont(font);//设置字体
	if (textSize <= 0 || textSize>50)
		textSize = 5;
	text->setCharacterSize(textSize);//字体大小
	text->setPosition(position);//字体位置
	text->setColor(textColor);//字体颜色
	//设置文字的朝向
	switch (direction)
	{
	case 0://左
		text->setAxisAlignment(osgText::Text::XZ_PLANE);//文字对称方式
		break;
	case 1://右
		text->setAxisAlignment(osgText::Text::REVERSED_XZ_PLANE);//文字对称方式
		break;
	case 2://前
		text->setAxisAlignment(osgText::Text::YZ_PLANE);//文字对称方式
		break;
	case 3://后
		text->setAxisAlignment(osgText::Text::REVERSED_YZ_PLANE);//文字对称方式
		break;
	}
	text->setAutoRotateToScreen(false);//跟随视角不断变化，距离越远，文字越小
	text->setBackdropType(osgText::Text::OUTLINE);//文件描边
	text->setBackdropColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));//文字描边的颜色
	text->setDrawMode(osgText::Text::TEXT | osgText::Text::BOUNDINGBOX);//添加文字边框
	text->setText(info);
	return text;
}


osg::ref_ptr<osg::Node> Utility::createCoorAxis(int textSize)
{
	//创建保存几何信息的对象
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//创建四个顶点
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(2.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(0.0f, 2.0f, 0.0f));
	v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(0.0f, 0.0f, 2.0f));
	geom->setVertexArray(v.get());

	//为每个顶点指定一种颜色
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();
	c->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //坐标原点为白色
	c->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); //x red
	c->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //坐标原点为白色
	c->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); //y green
	c->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //坐标原点为白色
	c->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); //z blue
													 //如果没指定颜色则会变为黑色
	geom->setColorArray(c.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	//三个轴
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2)); //X
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 2, 2)); //Y
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 4, 2)); //Z
																				//坐标轴的文字
	


	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING,
		osg::StateAttribute::OFF);
	geode->addDrawable(geom.get());

	osg::ref_ptr<osg::Group> newroot = new osg::Group();
	newroot->addChild(geode.get());
	//添加坐标轴文字
	osg::ref_ptr<osgText::Font> font = osgText::readFontFile("fonts/simhei.ttf");
	//x文字
	newroot->addChild(createText("x", textSize, font, osg::Vec3(2.0f, 0.0f, 0.0f), 0, osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	//y文字
	newroot->addChild(createText("y", textSize, font, osg::Vec3(0.0f, 2.0f, 0.0f), 3, osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	//z文字
	newroot->addChild(createText("z", textSize, font, osg::Vec3(0.0f, 0.0f, 2.0f), 0, osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)));
	return newroot;
}



