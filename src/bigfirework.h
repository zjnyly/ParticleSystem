#ifndef BIGFIREWORK_H
#define BIGFIREWORK_H

#include "firework.h"

class bigfirework:public Firework 
{
private:
	void genTrails();
public:
	bigfirework();

	bigfirework(float, glm::fvec3,bool, bool);

	~bigfirework();

	bool isAlive();

	void destroy();

	void init(fireworkParam fwp);

	void light(Shader& shder, float delta_time, int second_trails_num);

	bool isExploded();

	glm::fvec3 get_explode_position();

	glm::fvec4 get_explode_color();

private:
	ptr dummy;
	float exist;
	std::vector<ptr> trails;	// ������β�������ţ���0�����ͷ�ʱ��������
	std::vector<ptr> explosions; // ������ըЧ����������
	float time_cnt, explode_time; // ��ʱ���Լ�Ҫ��ը��ʱ��
	fireworkParam fwp; // �̻��Ĳ���
	bool exploded;
	bool canExplodeTwice;
	glm::fvec3 explode_position;
	glm::fvec4 explode_color;
	bool sound;
	bool randomcolor;
};


#endif