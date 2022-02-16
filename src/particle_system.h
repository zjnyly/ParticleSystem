#pragma once
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#define MAX_PARTICLE_NUM 10000
#define DEFAULT_GEN_RATE 1000

#include <stdlib.h>
#include <iostream>
#include <filesystem>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

struct Particle
{
    glm::fvec3 position; // �����ӵ�offset����ʾ���ӵ�λ��
    glm::fvec3 velocity;
    glm::fvec4 color;
    float size;
    float life;

    Particle()
        : position(glm::fvec3(0.0f)), velocity(glm::fvec3(0.0f)), color(glm::fvec4(0.0f)), life(-1.0f), size(0.0f) { }
};


struct GenParam {
    int gen_rate;
    float size_rate;
    float vel_base_rate;
    float vel_random_rate;
    int size;
    int delay = 1;
    float life_rate[2] = { 1.0, 1.0 };
};

class ParticleSystem
{
public:
    ParticleSystem() {};

    ParticleSystem(int max_particle, bool allow);

    ~ParticleSystem();

    void youCanDie();

    bool testLife(float estimatedAge);

    bool haveAnotherChance();

    void cancelAnotherChance();

    void initTrail(Particle& base_particle);

    void initExplode(Particle& base_particle, float v_abs);

    void updatePos(const float*);

    void updateColor(const float*);

    void draw(Shader&);

    int trail(float delta_time);

    int explode(float delta_time);

    bool isDied();

    glm::fvec3 getHeadParticlePos();

    glm::fvec3 posTrail();

    static void setTexture(const char* file_name);
    static void deleteTexture();

    void genParticles(float dt, float mu, float sigma);

    int trailGen(float delta_time, float mu, float sigma);

    void initTrailGen(Particle& base_particle, GenParam& param);

    void ParticleSystem::set_color(glm::fvec4 color);

    glm::fvec4 get_color();

private:
    // ʹ�����ĸ�VBO��
    // ���е�0�������������ӵĶ������꣬��1�������������ӵ�����λ��offset����2�������������ӵ���ɫ��Ϣ����3���������ӵĴ�С
    GLuint vbo[3], vao;
    //Particle particles[MAX_PARTICLE_NUM];
    std::vector<Particle> particles;
    int particles_num, max_particles_num;
    int layer; // ��β�ĳ��ȣ������Ӳ�������ʾ

    float pos_size_data[MAX_PARTICLE_NUM * 4];
    float color_data[MAX_PARTICLE_NUM * 4];
    static float vertex[3];
    // �㾫��
    static GLuint texture;
    static int system_cnt;

    // �������ӵĲ���
    GenParam gen_param;
    int count;
    glm::fvec3 pos;
    glm::fvec3 vel;
    bool canExplodeTwice;
    bool imDied;
};

#endif