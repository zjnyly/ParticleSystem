#ifndef BlinBlinFirework_H
#define BlinBlinFirework_H

#include "firework.h"

class BlinBlinFirework :public Firework
{
private:
	void genTrails();
public:
	BlinBlinFirework();

	BlinBlinFirework(float, glm::fvec3, bool);

	~BlinBlinFirework();

	bool isAlive();

	void destroy();

	void init(fireworkParam fwp);

	void light(Shader& shder, float delta_time, int second_trails_num);

	bool isExploded();

	glm::fvec3 get_explode_position();

	glm::fvec4 get_explode_color();

private:
	float exist;
	std::vector<glm::fvec4> store;
	std::vector<ptr> trails;	// ������β�������ţ���0�����ͷ�ʱ��������
	std::vector<ptr> explosions; // ������ըЧ����������
	float time_cnt, explode_time; // ��ʱ���Լ�Ҫ��ը��ʱ��
	fireworkParam fwp; // �̻��Ĳ���
	bool explodedOnce, explodedTwice;
	glm::fvec3 explodePosition;
	bool canExplodeTwice;
	glm::fvec4 explode_color;
	bool sound;
};


#endif