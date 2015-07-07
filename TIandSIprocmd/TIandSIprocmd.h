/**
 * TIandSIprocmd
 * (TIandSI Professional CMD version)
 *
 * ������ Lei Xiaohua,���� Zhang Hui
 * leixiaohua1020@126.com
 * zhanghuicuc@gmail.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 *
 * http://blog.csdn.net/leixiaohua1020
 * 
 * �����̿��Լ���һ��ѹ����Ƶ��ʱ����ϢTI��Temporal perceptual Information��
 * Ҳ���Գ�ʱ�临�Ӷȣ��Ϳռ���ϢSI��Spatial perceptual Information��Ҳ����
 * �ƿռ临�Ӷȣ������㷽�����Ա�׼��ITU-R BT.1788
 *
 * Professional: ֧��ѹ��������Ϊ����
 *
 * This software can calculate a video bitstream's TI(Temporal perceptual Information) 
 * and SI(Spatial perceptual Information) according to ITU-R BT.1788.
 *
 * Professional: Support bitstreams (not just raw data such as YUV, RGB, etc.) as Input.
 *
 */
#ifndef TIANDSIPROCMD_H
#define TIANDSIPROCMD_H

#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <xmmintrin.h>
#include <emmintrin.h>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "sdl/SDL.h"
#include "sdl/SDL_main.h"
};
#endif