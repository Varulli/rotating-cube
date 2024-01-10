#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <math.h>

typedef struct Quaternion
{
	float r;
	float i;
	float j;
	float k;
} Quaternion;

// Hamilton product
Quaternion q_multiply(Quaternion q1, Quaternion q2)
{
	float R = q1.r * q2.r - q1.i * q2.i - q1.j * q2.j - q1.k * q2.k,
		I = q1.r * q2.i + q1.i * q2.r + q1.j * q2.k - q1.k * q2.j,
		J = q1.r * q2.j - q1.i * q2.k + q1.j * q2.r + q1.k * q2.i,
		K = q1.r * q2.k + q1.i * q2.j - q1.j * q2.i + q1.k * q2.r;

	Quaternion Q = { R, I, J, K };
	return Q;
}

Quaternion q_rotate_about_x(Quaternion p, float angle)
{
	float half = angle * 0.5F,
		cosa = cosf(half),
		sina = sinf(half);

	Quaternion q1 = { cosa, sina, 0, 0 },
		q2 = { cosa, -sina, 0, 0 };

	return q_multiply(q_multiply(q1, p), q2);
}

Quaternion q_rotate_about_y(Quaternion p, float angle)
{
	float half = angle * 0.5F,
		cosa = cosf(half),
		sina = sinf(half);

	Quaternion q1 = { cosa, 0, sina, 0 },
		q2 = { cosa, 0, -sina, 0 };

	return q_multiply(q_multiply(q1, p), q2);
}

Quaternion q_rotate_about_z(Quaternion p, float angle)
{
	float half = angle * 0.5F,
		cosa = cosf(half),
		sina = sinf(half);

	Quaternion q1 = { cosa, 0, 0, sina },
		q2 = { cosa, 0, 0, -sina };

	return q_multiply(q_multiply(q1, p), q2);
}

// Screen resolution
#define WIDTH 100
#define HEIGHT 50

#define NUM_FRAMES 1000

const int zDisp = 100;
const int zProj = 50;

const float xLight = 0.0F;
const float yLight = 1.0F;
const float zLight = -1.0F;

const float deltaTheta = 0.1F;
const float deltaPhi = 0.05F;


void render_point(Quaternion q, Quaternion qNorm, float A, float B, float C, char** proj, float** zVal, float ooMagLight)
{
	q = q_rotate_about_z(q_rotate_about_y(q_rotate_about_x(q, A), B), C);
	qNorm = q_rotate_about_z(q_rotate_about_y(q_rotate_about_x(qNorm, A), B), C);

	float ooz = 1 / (q.k + zDisp);

	int xProj = (int)(WIDTH * 0.5F + q.i * zProj * ooz),
		yProj = (int)(HEIGHT * 0.5F - q.j * zProj * ooz * 0.5F);

	if (xProj >= 0 && xProj < WIDTH && yProj >= 0 && yProj < HEIGHT)
	{
		if (ooz > zVal[yProj][xProj])
		{
			zVal[yProj][xProj] = ooz;

			float L = ooMagLight * (xLight * qNorm.i + yLight * qNorm.j + zLight * qNorm.k);

			if (L > 0)
			{
				int indexL = (int)(L * 10);
				proj[yProj][xProj] = ".,-~:;+*#@"[indexL];
			}
			else
				proj[yProj][xProj] = ' ';
		}
	}
}

void render_point2(Quaternion q, Quaternion qNorm, float A, float B, float C, char** proj, float** zVal, char fill)
{
	q = q_rotate_about_z(q_rotate_about_y(q_rotate_about_x(q, A), B), C);
	qNorm = q_rotate_about_z(q_rotate_about_y(q_rotate_about_x(qNorm, A), B), C);

	float ooz = 1 / (q.k + zDisp);

	int xProj = (int)(WIDTH * 0.5F + q.i * zProj * ooz),
		yProj = (int)(HEIGHT * 0.5F - q.j * zProj * ooz * 0.5F);

	if (xProj >= 0 && xProj < WIDTH && yProj >= 0 && yProj < HEIGHT)
	{
		if (ooz > zVal[yProj][xProj])
		{
			zVal[yProj][xProj] = ooz;
			proj[yProj][xProj] = fill;
		}
	}
}

int main()
{
	char** proj = calloc(HEIGHT, sizeof(char*));
	float** zVal = calloc(HEIGHT, sizeof(float*));

	if (proj == NULL || zVal == NULL)
	{
		printf("Failed memory allocation");
		return -1;
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		proj[i] = calloc(WIDTH, sizeof(char));
		zVal[i] = calloc(WIDTH, sizeof(float));

		if (proj[i] == NULL || zVal[i] == NULL)
		{
			printf("Failed memory allocation");
			return -1;
		}
	}

	float A = 0.0F, B = 0.0F, C = 0.0F;

	float ooMagLight = 1 / sqrtf(xLight * xLight + yLight * yLight + zLight * zLight);

	getchar();

	for (int f = 0; f < NUM_FRAMES; f++)
	{
		printf("\x1b[H");

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				proj[i][j] = ' ';
				zVal[i][j] = 0.0F;
			}
		}

		for (int x = -25; x < 25; x++)
			for (int y = -25; y < 25; y++)
			{
				Quaternion q = { 0, x, y, 25 },
							qNorm = { 0, 0, 0, 1 };

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}

		for (int x = -25; x < 25; x++)
			for (int y = -25; y < 25; y++)
			{
				Quaternion q = { 0, x, y, -25 },
					qNorm = { 0, 0, 0, -1 };

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}

		for (int x = -25; x < 25; x++)
			for (int z = -25; z < 25; z++)
			{
				Quaternion q = { 0, x, 25, z },
					qNorm = { 0, 0, 1, 0 };

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}

		for (int x = -25; x < 25; x++)
			for (int z = -25; z < 25; z++)
			{
				Quaternion q = { 0, x, -25, z },
					qNorm = { 0, 0, -1, 0 };

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}

		for (int y = -25; y < 25; y++)
			for (int z = -25; z < 25; z++)
			{
				Quaternion q = { 0, -25, y, z },
					qNorm = { 0, -1, 0, 0 };

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}

		for (int y = -25; y < 25; y++)
			for (int z = -25; z < 25; z++)
			{
				Quaternion q = { 0, 25, y, z },
					qNorm = { 0, 1, 0, 0 };

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}

		char str[HEIGHT * (WIDTH + 1)] = { 0 };
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				strncat(str, &proj[i][j], 1);
			}
			if (i - (HEIGHT - 1))
				strcat(str, "\n");
		}
		printf("%s", str);

		A += 0.01F, B += 0.015F, C += 0.025F;
	}

	return 0;
}