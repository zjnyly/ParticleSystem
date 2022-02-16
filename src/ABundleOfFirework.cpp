#include "ABundleOfFirework.h"

// sound
extern irrklang::ISoundEngine* SoundEngine;

ABundleOfFirework::ABundleOfFirework() : time_cnt(0.0f), explodedOnce(false), explodedTwice(false) {

}

ABundleOfFirework::~ABundleOfFirework() {

	std::cout << "revoke" << std::endl;

}

ABundleOfFirework::ABundleOfFirework(float explode_time, glm::fvec3 pos, bool option) : time_cnt(0.0f), explode_time(explode_time), explodedOnce(false), explodedTwice(false) {
	//1
	ptr p = std::make_shared<ParticleSystem>(1, true);
	Particle base;

	if (option) {
		base.position = pos;
	}
	else base.position = glm::fvec3(0.0f, -2.0f, 0.5f);

	base.color = glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->explode_color = this->explode_color * (this->exist / (this->exist + 1.0f)) + base.color * (1 / (this->exist + 1.0f));
	this->exist += 1.0f;
	base.velocity = glm::fvec3(0.0f, 0.450f, 0.0f);
	base.size = 10.0f;
	base.life = 0.01f;
	GenParam param;
	param.gen_rate = 300;
	param.size = 1;
	param.size_rate = 0.0015;
	param.vel_base_rate = 1.0;
	param.vel_random_rate = 0.005;
	param.delay = 1;
	param.life_rate[0] = 0.8;
	param.life_rate[1] = 0.9;
	p->initTrailGen(base, param);
	//ParticleSystem::setTexture("texture_img/light_PNG14431.png");
	trails.push_back(p);
	sound = false;
}


/// <summary>
/// ��ʼ���̻�����
/// </summary>
/// <param name="base_fwp">�̻���ʼ����</param>
void ABundleOfFirework::init(fireworkParam base_fwp) {
	fwp = base_fwp;
	// ...����Ҫ�õ��Ĳ���
}


void ABundleOfFirework::destroy()
{
	trails.clear();
}

bool ABundleOfFirework::isAlive()
{
	bool flag = 0;
	for (int i = 0; i < trails.size(); ++i)
	{
		if (trails[i]->isDied() == false)
		{
			flag = 1;
			return true;
		}
	}
	return false;
}

/// <summary>
/// ȼ���̻�ʱ�ĸ��º���
/// </summary>
/// <param name="shader">��Ⱦ���õ���ɫ��</param>
/// <param name="delta_time">ÿ֮֡��ļ��ʱ��</param>
void ABundleOfFirework::light(Shader& shader, float delta_time, int second_trails_num) {

	time_cnt += delta_time;
	if (time_cnt < 0.01) {
		trails[0]->trailGen(delta_time, 0.85, 0.06);
		trails[0]->draw(shader);
		if (!sound) {
			SoundEngine->play2D("../sound/rise.wav", GL_FALSE);
			sound = true;
		}

	}
	else {
		if (!explodedOnce) { // �̻���һ�α�ը

			explodedOnce = true;
			explodePosition = trails[0]->getHeadParticlePos();
			genTrails();
			SoundEngine->play2D("../sound/explosion.wav", GL_FALSE);
		}
		//fwp.trails_num
		for (int i = 0; i < trails.size(); ++i) {

			if (trails[1]->testLife(5.0f) == true && !explodedTwice)
			{
				//std::cout << "gen" << std::endl;
				//explodedTwice = true;
				//for (int j = 0; j < second_trails_num; ++j) {
				//	ptr p = std::make_shared<ParticleSystem>(10000, false);
				//	// ��ʼ�����ɵĽڵ㣬�������Ը�
				//	Particle base;
				//	base.position = explodePosition;
				//	//base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
				//	base.color = glm::fvec4(0.98f, 0.98f, 0.82f, 1.0f);
				//	this->explode_color = this->explode_color * (this->exist / (this->exist + 1.0f)) + base.color * (1 / (this->exist + 1.0f));
				//	this->exist += 1.0f;
				//	base.velocity = 0.15f * sphereRandom();
				//	base.size = 1.0f;
				//	base.life = 2.0f;
				//	GenParam param;
				//	param.gen_rate = 150;
				//	param.size = 1;
				//	param.size_rate = 0.001;
				//	param.vel_base_rate = 1.0;
				//	param.vel_random_rate = 0.0000;
				//	param.delay = 1;
				//	param.life_rate[0] = 0.6;
				//	param.life_rate[1] = 0.7;
				//	p->initTrailGen(base, param);
				//	trails.push_back(p);
				//}
			}
			//if (trails[i]->isDied()) // ������Ⱦ������������
			//{
			//	continue;
			//}
			trails[i]->trailGen(delta_time, 0.7, 0.07);
			trails[i]->draw(shader);
		}
	}
}


/// <summary>
/// �����̻�ը��ʱ����������
/// </summary>
void ABundleOfFirework::genTrails() {
	explode_color = trails[0]->get_color();
	glm::fvec3 explode_pos = trails[0]->posTrail();
	fwp.trails_num /= 5;
	trails.resize(fwp.trails_num + 1);
	//ParticleSystem::setTexture("texture_img/light_PNG14431.png");
	for (int i = 1; i <= fwp.trails_num; ++i) {
		ptr p = std::make_shared<ParticleSystem>(10000, true);
		// ��ʼ�����ɵĽڵ㣬�������Ը�
		Particle base;
		base.position = explode_pos;
		//base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
		base.color = glm::fvec4(ColorRandom(), 1.0f);
		this->explode_color = this->explode_color * (this->exist / (this->exist + 1.0f)) + base.color * (1 / (this->exist + 1.0f));
		this->exist += 1.0f;
		base.velocity = 0.15f * bundleRandom();
		base.size = 3.0f;
		base.life = 6.0;
		GenParam param;
		param.gen_rate = 150;
		param.size = 1;
		param.size_rate = 0.001;
		param.vel_base_rate = 1.0;
		param.vel_random_rate = 0.0000;
		param.delay = 1;
		param.life_rate[0] = 0.6;
		param.life_rate[1] = 0.7;
		p->initTrailGen(base, param);
		trails[i] = p;
	}
}

bool ABundleOfFirework::isExploded() {
	return explodedTwice;
}

glm::fvec3 ABundleOfFirework::get_explode_position() {
	return explodePosition;
}

glm::fvec4 ABundleOfFirework::get_explode_color() {
	explode_color[0] = std::floor(explode_color[0] * 255.f) / 255;
	explode_color[1] = std::floor(explode_color[1] * 255.f) / 255;
	explode_color[2] = std::floor(explode_color[2] * 255.f) / 255;
	explode_color[3] = 3.0f;
	return explode_color;
}