//#include "bigfirework.h"
//
//// sound
//extern irrklang::ISoundEngine* SoundEngine;
//
//bigfirework::bigfirework() : time_cnt(0.0f), exploded(false) {
//
//}
//
//bigfirework::~bigfirework() {
//
//	std::cout << "revoke" << std::endl;
//
//}
//
//bigfirework::bigfirework(float explode_time) : time_cnt(0.0f), explode_time(explode_time), exploded(false) {
//	ptr p = std::make_shared<ParticleSystem>(5000, true);
//	Particle base;
//	base.position = glm::fvec3(0.0f, -1.5f, -1.0f);
//	base.color = glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f);
//	base.velocity = glm::fvec3(0.0f, 0.450f, 0.0f);
//	base.size = 10.0f;
//	base.life = 4.0f;
//	GenParam param;
//	param.gen_rate = 300;
//	param.size = 1;
//	param.size_rate = 0.0015;
//	param.vel_base_rate = 1.0;
//	param.vel_random_rate = 0.005;
//	param.delay = 1;
//	param.life_rate[0] = 0.8;
//	param.life_rate[1] = 0.9;
//	p->initTrailGen(base, param);
//	//ParticleSystem::setTexture("texture_img/light_PNG14431.png");
//	trails.push_back(p);
//}
//
//
///// <summary>
///// ��ʼ���̻�����
///// </summary>
///// <param name="base_fwp">�̻���ʼ����</param>
//void bigfirework::init(fireworkParam base_fwp) {
//	fwp = base_fwp;
//	// ...����Ҫ�õ��Ĳ���
//}
//
//
//void bigfirework::destroy()
//{
//	trails.clear();
//}
//
//bool bigfirework::isAlive()
//{
//	bool flag = 0;
//	for (int i = 0; i < trails.size(); ++i)
//	{
//		if (trails[i]->isDied() == false)
//		{
//			flag = 1;
//			return true;
//		}
//	}
//	return false;
//}
//
///// <summary>
///// ȼ���̻�ʱ�ĸ��º���
///// </summary>
///// <param name="shader">��Ⱦ���õ���ɫ��</param>
///// <param name="delta_time">ÿ֮֡��ļ��ʱ��</param>
//void bigfirework::light(Shader& shader, float delta_time) {
//
//	time_cnt += delta_time;
//	if (time_cnt < explode_time) {
//		static bool sound = false;
//		trails[0]->trailGen(delta_time, 0.85, 0.06);
//		trails[0]->draw(shader);
//		if (!sound) {
//			SoundEngine->play2D("../sound/rise.wav", GL_FALSE);
//			sound = true;
//		}
//
//	}
//	else {
//		if (!exploded) { // �̻���һ�α�ը
//			exploded = true;
//			genTrails();
//			SoundEngine->play2D("../sound/explosion.wav", GL_FALSE);
//		}
//		//fwp.trails_num
//		for (int i = 0; i < trails.size(); ++i) {
//			if (trails[i]->isDied()) // ������Ⱦ������������
//			{
//				if (trails[i]->haveAnotherChance() == true)
//				{
//					trails[i]->cancelAnotherChance();
//
//					auto chance = floatRandom(0.0f, 1.0f);
//
//					if (chance > 0.9)
//					{
//						for (int j = 0; j < 300; ++j) {
//							ptr p = std::make_shared<ParticleSystem>(50, false);
//							// ��ʼ�����ɵĽڵ㣬�������Ը�
//							Particle base;
//							base.position = trails[i]->getHeadParticlePos();
//							//base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
//							base.color = glm::fvec4(ColorRandom(), 1.0f);
//							base.velocity = 0.15f * sphereRandom();
//							base.size = 2.0f;
//							base.life = floatRandom(0.0, 2.0);
//							GenParam param;
//							param.gen_rate = 0;
//							param.size = 1;
//							param.size_rate = 0.001;
//							param.vel_base_rate = 1.0;
//							param.vel_random_rate = 0.0000;
//							param.delay = 1;
//							param.life_rate[0] = 0.6;
//							param.life_rate[1] = 0.7;
//							p->initTrailGen(base, param);
//							trails.push_back(p);
//						}
//					}
//					//trails[i]->youCanDie();
//				}
//				
//			}
//			trails[i]->trailGen(delta_time, 0.7, 0.07);
//			trails[i]->draw(shader);
//		}
//	}
//}
//
//
///// <summary>
///// �����̻�ը��ʱ����������
///// </summary>
//void bigfirework::genTrails() {
//	glm::fvec3 explode_pos = trails[0]->posTrail();
//	trails.resize(fwp.trails_num);
//	//ParticleSystem::setTexture("texture_img/light_PNG14431.png");
//	for (int i = 0; i < fwp.trails_num; ++i) {
//		ptr p = std::make_shared<ParticleSystem>(10000, true);
//		// ��ʼ�����ɵĽڵ㣬�������Ը�
//		Particle base;
//		base.position = explode_pos;
//		//base.color = glm::fvec4(0.1f, 0.4f, 0.3f, 1.0f);
//		base.color = glm::fvec4(ColorRandom(), 1.0f);
//		base.velocity = 0.15f * sphereRandom();
//		base.size = 3.0f;
//		base.life = floatRandom(0.0, 6.0);
//		GenParam param;
//		param.gen_rate = 150;
//		param.size = 1;
//		param.size_rate = 0.001;
//		param.vel_base_rate = 1.0;
//		param.vel_random_rate = 0.0000;
//		param.delay = 1;
//		param.life_rate[0] = 0.6;
//		param.life_rate[1] = 0.7;
//		p->initTrailGen(base, param);
//		trails[i] = p;
//	}
//}