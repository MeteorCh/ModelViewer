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
 * �Ƚ�������ά�����Ƿ���ͬ
 */
bool Utility::isVec3Same(osg::Vec3 v1, osg::Vec3 v2)
{
	return (v1.x() == v2.x()) && (v1.y() == v2.y()) && (v1.z() == v2.z());
}

/**
 * ��һ���ַ��滻ԭ�ַ��е���һ���ַ�
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
 * ��·���л�ȡ�ļ�������������׺����
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
* ����text
* parameter1����ʾ��������Ϣ
* parameter2�����ֵĴ�С
* parameter3������
* parameter4������λ��
* parameter5��direction���ֵĳ���0��1��2ǰ4��
* parameter6��������ɫ
*/
osg::ref_ptr<osgText::Text> Utility::createText(string info, int textSize, osg::ref_ptr<osgText::Font> font, osg::Vec3 position, int direction, osg::Vec4 textColor)
{
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setFont(font);//��������
	if (textSize <= 0 || textSize>50)
		textSize = 5;
	text->setCharacterSize(textSize);//�����С
	text->setPosition(position);//����λ��
	text->setColor(textColor);//������ɫ
	//�������ֵĳ���
	switch (direction)
	{
	case 0://��
		text->setAxisAlignment(osgText::Text::XZ_PLANE);//���ֶԳƷ�ʽ
		break;
	case 1://��
		text->setAxisAlignment(osgText::Text::REVERSED_XZ_PLANE);//���ֶԳƷ�ʽ
		break;
	case 2://ǰ
		text->setAxisAlignment(osgText::Text::YZ_PLANE);//���ֶԳƷ�ʽ
		break;
	case 3://��
		text->setAxisAlignment(osgText::Text::REVERSED_YZ_PLANE);//���ֶԳƷ�ʽ
		break;
	}
	text->setAutoRotateToScreen(false);//�����ӽǲ��ϱ仯������ԽԶ������ԽС
	text->setBackdropType(osgText::Text::OUTLINE);//�ļ����
	text->setBackdropColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));//������ߵ���ɫ
	text->setDrawMode(osgText::Text::TEXT | osgText::Text::BOUNDINGBOX);//������ֱ߿�
	text->setText(info);
	return text;
}


osg::ref_ptr<osg::Node> Utility::createCoorAxis(int textSize)
{
	//�������漸����Ϣ�Ķ���
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//�����ĸ�����
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(2.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(0.0f, 2.0f, 0.0f));
	v->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	v->push_back(osg::Vec3(0.0f, 0.0f, 2.0f));
	geom->setVertexArray(v.get());

	//Ϊÿ������ָ��һ����ɫ
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();
	c->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //����ԭ��Ϊ��ɫ
	c->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); //x red
	c->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //����ԭ��Ϊ��ɫ
	c->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); //y green
	c->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //����ԭ��Ϊ��ɫ
	c->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); //z blue
													 //���ûָ����ɫ����Ϊ��ɫ
	geom->setColorArray(c.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	//������
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2)); //X
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 2, 2)); //Y
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 4, 2)); //Z
																				//�����������
	


	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING,
		osg::StateAttribute::OFF);
	geode->addDrawable(geom.get());

	osg::ref_ptr<osg::Group> newroot = new osg::Group();
	newroot->addChild(geode.get());
	//�������������
	osg::ref_ptr<osgText::Font> font = osgText::readFontFile("fonts/simhei.ttf");
	//x����
	newroot->addChild(createText("x", textSize, font, osg::Vec3(2.0f, 0.0f, 0.0f), 0, osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	//y����
	newroot->addChild(createText("y", textSize, font, osg::Vec3(0.0f, 2.0f, 0.0f), 3, osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	//z����
	newroot->addChild(createText("z", textSize, font, osg::Vec3(0.0f, 0.0f, 2.0f), 0, osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)));
	return newroot;
}



