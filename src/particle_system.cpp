#include "particle_system.h"
#include "utility_tool.h"
#include "stb_image.h"

enum vbo_index { VERTEX, POS, COLOR };

float ParticleSystem::vertex[3] = { 0.0f, 0.0f, 0.0f };
int ParticleSystem::system_cnt = 0;
GLuint ParticleSystem::texture = -1;


ParticleSystem::~ParticleSystem() {
	imDied = true;
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &vao);

	--system_cnt;
	if (system_cnt == 0) {
		deleteTexture();
	}
}

void ParticleSystem::youCanDie() 
{
	imDied = true;
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &vao);
}


bool ParticleSystem::testLife(float estimatedAge)
{
	return particles[0].life <= estimatedAge;
}



/// <summary>
/// ����ϵͳ���캯��
/// </summary>
/// <param name="max_particle"> ���������� </param>
ParticleSystem::ParticleSystem(int max_particle = MAX_PARTICLE_NUM, bool allow = true) : max_particles_num(max_particle), canExplodeTwice(allow) {
	glGenBuffers(3, vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	// ��ʼ���������ӵ�λ��
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX]);
	// ��ʼ��Ϊ������
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(VERTEX);

	// ��ʼ�����ӵ�����λ���Լ���С(����ά)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[POS]);
	// ��ʼ��Ϊ������
	glBufferData(GL_ARRAY_BUFFER, max_particles_num * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(POS, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(POS);

	// ��ʼ�����ӵ���ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
	// ��ʼ��Ϊ������
	glBufferData(GL_ARRAY_BUFFER, max_particles_num * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(COLOR);

	++system_cnt;
	/*particles.resize(MAX_PARTICLE_NUM);*/
}





bool ParticleSystem::haveAnotherChance()
{
	return canExplodeTwice;
}

void ParticleSystem::cancelAnotherChance()
{
	canExplodeTwice = false;
}

/// <summary>
/// �������ӵ���ɫ����
/// </summary>
/// <param name="color_data">��ɫ������ֵ</param>
void ParticleSystem::updateColor(const float* color_data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_num * sizeof(GLfloat) * 4, color_data);
}



/// <summary>
/// �������ӵ�λ���Լ���С��Ϣ
/// </summary>
/// <param name="pos_data">λ���Լ���С������ֵ</param>
void ParticleSystem::updatePos(const float* pos_data) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo[POS]);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_num * sizeof(GLfloat) * 4, pos_data);
}


/// <summary>
/// ÿһ֡��Ⱦ
/// </summary>
/// <param name="shader">��Ⱦ���õ���ɫ������</param>
void ParticleSystem::draw(Shader& shader) {
	if (texture != -1) {
		glEnable(GL_POINT_SPRITE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	glEnable(GL_PROGRAM_POINT_SIZE);
	shader.use();
	if (texture != -1)
		glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribDivisor(VERTEX, 0);
	glVertexAttribDivisor(POS, 1);
	glVertexAttribDivisor(COLOR, 1);
	glDrawArraysInstanced(GL_POINTS, 0, 1, particles_num);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


/// <summary>
/// ��ʼ������ϵͳ�е�����
/// </summary>
/// <param name="base_particle">��ʼ�����õı�׼��������</param>
void ParticleSystem::initTrail(Particle& base_particle) {
	particles.resize(MAX_PARTICLE_NUM);
	particles[0] = base_particle;
	float size_atten = 0.01f;
	for (int i = 1; i < max_particles_num; ++i) {
		particles[i].position = base_particle.position;
		particles[i].velocity = glm::fvec3(0.0f, 0.0f, 0.0f);
		particles[i].life = floatRandom(10.0f, base_particle.life);
		particles[i].color = base_particle.color;
		particles[i].size = (1.0f - size_atten) * base_particle.size;
	}
}

void ParticleSystem::initTrailGen(Particle& base_particle, GenParam& param) {
	particles.push_back(base_particle);
	gen_param = param;
	pos = particles[0].position;
	vel = particles[0].velocity;
	count = 0;
}

/// <summary>
/// �����Ӱ�һ���Ĺ켣�˶�
/// </summary>
/// <param name="delta_time">ʱ����</param>
/// <returns>��ǰ������ӵĸ���</returns>
int ParticleSystem::trail(float delta_time) {
	particles_num = 0;
	glm::fvec3 a(0.0f, -0.02f, 0.0f); // ���ٶ�
	float size_atten;

	glm::fvec3 pre_pos;

	for (int i = 0; i < max_particles_num; ++i) {
		if (particles[i].life > 0.0) {
			if (i == 0) {
				// ������ͷ���ӵ�λ�����ٶ�
				pre_pos = particles[i].position;
				particles[i].position = move(particles[i].position, particles[i].velocity, a, delta_time);
				particles[i].velocity = velocityUpdate(particles[i].velocity, a, delta_time);
				/*particles[i].size *= 0.999f;*/
			}
			else {
				// ����ʣ�����ӵ�λ��
				glm::fvec3 tmp = particles[i].position;
				particles[i].position = pre_pos;
				pre_pos = tmp;
				/*showed[i] = true;
				tmp = true;*/
			}

			particles[i].life -= delta_time;

			// ̫С������ֱ����ʧ
			if (particles[i].size < 0.001) {
				particles[i].life = -1.0f;
				continue;
			}

			pos_size_data[4 * particles_num + 0] = particles[i].position.x;
			pos_size_data[4 * particles_num + 1] = particles[i].position.y;
			pos_size_data[4 * particles_num + 2] = particles[i].position.z;
			pos_size_data[4 * particles_num + 3] = particles[i].size;

			color_data[4 * particles_num + 0] = particles[i].color.r;
			color_data[4 * particles_num + 1] = particles[i].color.g;
			color_data[4 * particles_num + 2] = particles[i].color.b;
			color_data[4 * particles_num + 3] = particles[i].color.a;

			particles_num++;
		}
		else {
			//showed[i] = false;
		}
	}
	updatePos(pos_size_data);
	updateColor(color_data);
	return particles_num;
}


int ParticleSystem::trailGen(float delta_time, float mu, float sigma) {
	glm::fvec3 a(0.0f, -0.02f, 0.0f); // ���ٶ�
	genParticles(delta_time, mu, sigma);
	particles_num = 0;
	for (int i = 0; i < particles.size(); ++i) {
		if (particles[i].life > 0.0f) {

			particles[i].position = move(particles[i].position, particles[i].velocity, a, delta_time);
			particles[i].velocity = velocityUpdate(particles[i].velocity, a, delta_time);

			particles[i].life -= delta_time;

			pos_size_data[4 * particles_num + 0] = particles[i].position.x;
			pos_size_data[4 * particles_num + 1] = particles[i].position.y;
			pos_size_data[4 * particles_num + 2] = particles[i].position.z;
			pos_size_data[4 * particles_num + 3] = particles[i].size;

			color_data[4 * particles_num + 0] = particles[i].color.r;
			color_data[4 * particles_num + 1] = particles[i].color.g;
			color_data[4 * particles_num + 2] = particles[i].color.b;
			color_data[4 * particles_num + 3] = particles[i].color.a;

			particles_num++;
		}
		else {
			particles[i] = particles[particles.size() - 1];
			particles.pop_back();
			--i;
		}
	}
	updatePos(pos_size_data);
	updateColor(color_data);
	return particles_num;
}


/// <summary>
/// ��ʼ��Ҫ��ը��������
/// </summary>
/// <param name="base_particle">ÿ�����ӵĻ������ԣ�ע��������ٶȲ�û��ʹ��</param>
/// <param name="v_abs">Ҫָ�����ٶȴ�С</param>
void ParticleSystem::initExplode(Particle& base_particle, float v_abs) {
	particles.resize(MAX_PARTICLE_NUM);
	for (int i = 0; i < max_particles_num; ++i) {
		particles[i] = base_particle;
		particles[i].velocity = v_abs * sphereRandom();
	}
}


/// <summary>
/// ���ӱ�ը
/// </summary>
/// <param name="delta_time">ʱ����</param>
/// <returns>������ӵĸ���</returns>
int ParticleSystem::explode(float delta_time) {
	particles_num = 0;
	glm::fvec3 a(0.0f, 0.00f, 0.0f); // ���ٶ�
	float size_atten;

	glm::fvec3 pre_pos;

	for (int i = 0; i < max_particles_num; ++i) {
		if (particles[i].life > 0.0) {

			particles[i].position = move(particles[i].position, particles[i].velocity, a, delta_time);
			particles[i].velocity = velocityUpdate(particles[i].velocity, a, delta_time);

			// ̫С������ֱ����ʧ
			if (particles[i].size < 0.01) {
				particles[i].life = -1.0f;
				continue;
			}

			pos_size_data[4 * particles_num + 0] = particles[i].position.x;
			pos_size_data[4 * particles_num + 1] = particles[i].position.y;
			pos_size_data[4 * particles_num + 2] = particles[i].position.z;
			pos_size_data[4 * particles_num + 3] = particles[i].size;

			color_data[4 * particles_num + 0] = particles[i].color.r;
			color_data[4 * particles_num + 1] = particles[i].color.g;
			color_data[4 * particles_num + 2] = particles[i].color.b;
			color_data[4 * particles_num + 3] = particles[i].color.a;

			particles_num++;
		}
	}
	updatePos(pos_size_data);
	updateColor(color_data);
	return particles_num;
}



/// <summary>
/// �������Ƿ��Ѿ�����
/// </summary>
/// <returns>true: ��������</returns>
bool ParticleSystem::isDied() {
	// ����ͷ�������������������ž�����
	return particles[0].life <= 0.0f;
}


glm::fvec3 ParticleSystem::getHeadParticlePos()
{
	return particles[0].position;
}


/// <summary>
/// ������ͷ���ӵ�λ��
/// </summary>
/// <returns>λ��</returns>
glm::fvec3 ParticleSystem::posTrail() {
	return particles[0].position;
}


/// <summary>
/// Ϊ�㾫���������
/// </summary>
/// <param name="file_name">������ļ�·��</param>
void ParticleSystem::setTexture(const char* file_name) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���ز���������
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file_name, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}



/// <summary>
/// ɾ������
/// </summary>
void ParticleSystem::deleteTexture() {
	glDeleteTextures(1, &texture);
}


/// <summary>
/// ��������
/// </summary>
void ParticleSystem::genParticles(float delta_time, float mu, float sigma) {
	++count;
	if (count % gen_param.delay != 0) {
		return;
	}
	int n = delta_time * gen_param.gen_rate; // �����������ӵĸ���
	for (int i = 0; i < n; ++i) {
		if (particles.size() >= max_particles_num)
			break;
		Particle p;
		float r = particles[0].size * gen_param.size_rate;
		p.color = particles[0].color;
		//p.velocity = particles[0].velocity * floatRandom(0.85, 0.90);
		// p.velocity = particles[0].velocity - glm::fvec3(0.0f, r / delta_time, 0.0f) * floatRandom(0.085, 0.090);
		float tmp = normalRandom(mu, sigma);
		p.velocity = vel * gen_param.vel_base_rate * tmp + vel * gen_param.vel_random_rate * sphereRandom();
		p.position = posRandom(pos, r); // (0.01f * delta_time * particles[0].velocity).length()
		p.life = particles[0].life * floatRandom(gen_param.life_rate[1], gen_param.life_rate[1]);
		//p.life = particles[0].life * tmp;
		p.size = gen_param.size;
		particles.push_back(p);
	}
	pos = particles[0].position;
	vel = particles[0].velocity;
}

void ParticleSystem::set_color(glm::fvec4 color) {
	particles[0].color = color;
}

glm::fvec4 ParticleSystem::get_color() {
	return particles[0].color;
}