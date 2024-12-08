#include <math.h>
#include <time.h>
#include "init.h"

typedef struct Quaternion
{
	float r;
	float i;
	float j;
	float k;
} Quaternion;

/**
 * Computes Hamilton product.
 */
Quaternion q_multiply(Quaternion q1, Quaternion q2)
{
	float R = q1.r * q2.r - q1.i * q2.i - q1.j * q2.j - q1.k * q2.k,
		  I = q1.r * q2.i + q1.i * q2.r + q1.j * q2.k - q1.k * q2.j,
		  J = q1.r * q2.j - q1.i * q2.k + q1.j * q2.r + q1.k * q2.i,
		  K = q1.r * q2.k + q1.i * q2.j - q1.j * q2.i + q1.k * q2.r;

	Quaternion Q = {R, I, J, K};
	return Q;
}

Quaternion q_rotate_about_x(Quaternion p, float angle)
{
	float half = angle * 0.5f,
		  cosa = cosf(half),
		  sina = sinf(half);

	Quaternion q1 = {cosa, sina, 0, 0},
			   q2 = {cosa, -sina, 0, 0};

	return q_multiply(q_multiply(q1, p), q2);
}

Quaternion q_rotate_about_y(Quaternion p, float angle)
{
	float half = angle * 0.5f,
		  cosa = cosf(half),
		  sina = sinf(half);

	Quaternion q1 = {cosa, 0, sina, 0},
			   q2 = {cosa, 0, -sina, 0};

	return q_multiply(q_multiply(q1, p), q2);
}

Quaternion q_rotate_about_z(Quaternion p, float angle)
{
	float half = angle * 0.5f,
		  cosa = cosf(half),
		  sina = sinf(half);

	Quaternion q1 = {cosa, 0, 0, sina},
			   q2 = {cosa, 0, 0, -sina};

	return q_multiply(q_multiply(q1, p), q2);
}

// Screen resolution
#define WIDTH 60
#define HEIGHT 30

const int zDisp = 100;
const int zProj = 50;

// Light vector components
const float xLight = 0.0f;
const float yLight = 1.0f;
const float zLight = -1.0f;

const float deltaTheta = 0.1f;
const float deltaPhi = 0.05f;

// Rotates a point and projects it.
void render_point(Quaternion q, Quaternion qNorm, float A, float B, float C, char **proj, float **zVal, float ooMagLight)
{
	q = q_rotate_about_z(q_rotate_about_y(q_rotate_about_x(q, A), B), C);
	qNorm = q_rotate_about_z(q_rotate_about_y(q_rotate_about_x(qNorm, A), B), C);

	float ooz = 1 / (q.k + zDisp);

	int xProj = (int)(WIDTH * 0.5f + q.i * zProj * ooz),
		yProj = (int)(HEIGHT * 0.5f - q.j * zProj * ooz * 0.5f);

	if (xProj >= 0 && xProj < WIDTH && yProj >= 0 && yProj < HEIGHT)
	{
		if (ooz > zVal[yProj][xProj])
		{
			zVal[yProj][xProj] = ooz;

			float L = ooMagLight * (xLight * qNorm.i + yLight * qNorm.j + zLight * qNorm.k);

			if (L > 0)
			{
				int indexL = (int)(L * 10);
				proj[yProj][xProj] = "`.:-=+*#%@"[indexL];
			}
			else
				proj[yProj][xProj] = ' ';
		}
	}
}

int main(int argc, char *argv[])
{
	initialize();

	// Allocate projection buffers
	char **proj = calloc(HEIGHT, sizeof(char *));
	float **zVal = calloc(HEIGHT, sizeof(float *));
	if (proj == NULL || zVal == NULL)
	{
		fprintf(stderr, "Error: Failed memory allocation\n");
		return 1;
	}
	for (int i = 0; i < HEIGHT; i++)
	{
		proj[i] = calloc(WIDTH, sizeof(char));
		zVal[i] = calloc(WIDTH, sizeof(float));

		if (proj[i] == NULL || zVal[i] == NULL)
		{
			fprintf(stderr, "Error: Failed memory allocation\n");
			return 1;
		}
	}

	srand(time(NULL));

	// Initial rotation angles
	float A = 0.0f, B = 0.0f, C = 0.0f;

	// Rotation angle increments
	float incA = (-1 * rand() % 2) * 0.003f * (rand() % 151 + 50) / 50.0f;
	float incB = (-1 * rand() % 2) * 0.003f * (rand() % 151 + 50) / 50.0f;
	float incC = (-1 * rand() % 2) * 0.003f * (rand() % 151 + 50) / 50.0f;

	float ooMagLight = 1 / sqrtf(xLight * xLight + yLight * yLight + zLight * zLight);

	// Set duration
	double duration = 10.0;
	if (argc == 2)
	{
		duration = strtod(argv[1], NULL);
		if (duration <= 0)
		{
			fprintf(stderr, "Error: Invalid duration value (must be greater than 0)\n");
			printf("Usage: rotating-cube [seconds]\n");
			return 2;
		}
	}
	else if (argc > 2)
	{
		fprintf(stderr, "Error: Too many arguments\n");
		printf("Usage: rotating-cube [seconds]\n");
		return 3;
	}

	// Use alternate buffer
	printf("\x1b[?1049h\x1b[?25l");
	fflush(stdout);

	for (time_t start = time(NULL); difftime(time(NULL), start) < duration;)
	{
		// Clear projection buffers
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				proj[i][j] = ' ';
				zVal[i][j] = 0.0f;
			}
		}

		// Render points along each face of cube
		for (int x = -25; x < 25; x++)
			for (int y = -25; y < 25; y++)
			{
				Quaternion q = {0, x, y, 25},
						   qNorm = {0, 0, 0, 1};

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}
		for (int x = -25; x < 25; x++)
			for (int y = -25; y < 25; y++)
			{
				Quaternion q = {0, x, y, -25},
						   qNorm = {0, 0, 0, -1};

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}
		for (int x = -25; x < 25; x++)
			for (int z = -25; z < 25; z++)
			{
				Quaternion q = {0, x, 25, z},
						   qNorm = {0, 0, 1, 0};

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}
		for (int x = -25; x < 25; x++)
			for (int z = -25; z < 25; z++)
			{
				Quaternion q = {0, x, -25, z},
						   qNorm = {0, 0, -1, 0};

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}
		for (int y = -25; y < 25; y++)
			for (int z = -25; z < 25; z++)
			{
				Quaternion q = {0, -25, y, z},
						   qNorm = {0, -1, 0, 0};

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}
		for (int y = -25; y < 25; y++)
			for (int z = -25; z < 25; z++)
			{
				Quaternion q = {0, 25, y, z},
						   qNorm = {0, 1, 0, 0};

				render_point(q, qNorm, A, B, C, proj, zVal, ooMagLight);
			}

		// Display cube
		char str[HEIGHT * (WIDTH + 1) + 1] = {0};
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				str[(WIDTH + 1) * i + j] = proj[i][j];
			}
			str[(WIDTH + 1) * (i + 1) - 1] = '\n';
		}
		printf("%s\x1b[H", str);
		fflush(stdout);

		// Rotate cube
		A += incA;
		B += incB;
		C += incC;
	}
	printf("Press Enter to Return");
	fflush(stdout);
	getchar();

	printf("\x1b[?25h\x1b[?1049l");
	fflush(stdout);

	return 0;
}