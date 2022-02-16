#pragma once
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <stdlib.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265358979323846

/// <summary>
/// ����������delta_time�����ȼ����˶����λ��
/// </summary>
/// <param name="init_pos">����ĳ�ʼλ��</param>
/// <param name="velocity">����ĳ�ʼ�ٶ�</param>
/// <param name="acceleration">�������ܵļ��ٶ�</param>
/// <param name="delta_time">�˶��ĳ���ʱ��</param>
/// <returns>�������յ�λ��</returns>
glm::fvec3 move(glm::fvec3 init_pos, glm::fvec3 velocity, glm::fvec3 acceleration, float delta_time);


/// <summary>
/// ����������delta_time�����ȼ����˶�����ٶ�
/// </summary>
/// <param name="velocity">�����ʼ�ٶ�</param>
/// <param name="acceleration">������ٶ�</param>
/// <param name="delta_time">�˶�����ʱ��</param>
/// <returns>���������ٶ�</returns>
glm::fvec3 velocityUpdate(glm::fvec3 velocity, glm::fvec3 acceleration, float delta_time);


/// <summary>
/// ����һ�������ά����
/// </summary>
/// <param name="min">�½�</param>
/// <param name="max">�Ͻ�</param>
/// <returns>�����ά����</returns>
glm::fvec3 fvec3Random(float min, float max);


/// <summary>
/// ����һ�����������
/// </summary>
/// <param name="min">�½�</param>
/// <param name="max">�Ͻ�</param>
/// <returns>���������</returns>
float floatRandom(float min, float max);


/// <summary>
/// ��ʱ��������������Ⱦ����֮֡���delta_time
/// </summary>
/// <returns>������֮֡��ļ��ʱ��</returns>
float timer();


/// <summary>
/// ����ָ��λԲԲ�������һ��ĵ�λ����
/// </summary>
/// <returns>�������ĵ�λ����</returns>
glm::fvec3 sphereRandom();


glm::fvec3 bundleRandom();


/// <summary>
/// ����origin_posΪ���ģ�radiusΪ�뾶�����������
/// </summary>
/// <param name="origin_pos"></param>
/// <param name="radius"></param>
/// <returns></returns>
glm::fvec3 posRandom(glm::fvec3& origin_pos, float radius);


/// <summary>
/// ���ط��Ӹ�˹�ֲ�������� (0.5,1)
/// </summary>
/// <returns></returns>
float normalRandom(float mu, float sigma);

/// <summary>
/// ���ط��Ӹ�˹�ֲ�������� (0.0, 1.0)
/// </summary>
/// <returns></returns>
glm::fvec3 ColorRandom();

#endif