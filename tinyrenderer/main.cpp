#include "tgaimage.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int width = 800;
int height = 800;

Model* model = NULL;

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

int main(int argc, char** argv) {
	
	//line(13, 20, 80, 40, image, white);
	//line(20, 13, 40, 80, image, red);
	//line(80, 40, 13, 20, image, red);

	// Wire Renderer

	if (2 == argc) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("african_head.obj");
	}

	TGAImage image(width, height, TGAImage::RGB);

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);

		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.0f) * width / 2.0f;
			int y0 = (v0.y + 1.0f) * height / 2.0f;
			int x1 = (v1.x + 1.0f) * width / 2.0f;
			int y1 = (v1.y + 1.0f) * height / 2.0f;

			line(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically();
	image.write_tga_file("output.tga");

	delete model;

	return 0;
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	/*
	int dx = x1 - x0;
	int dy = y1 - y0;

	int derror2 = std::abs(dy) * 2;
	int error2 = 0;

	int y = y0;
	const int y_incr = (y1 > y0 ? 1 : -1);
	
	if (steep) {
		for (int x = x0; x <= x1; x++) {
			image.set(x, y, color);
			error2 += derror2;
			if (error2 > dx) {
				y += y_incr;
				error2 -= dx * 2;
			}
		}
	}
	else {
		for (int x = x0; x <= x1; x++) {
			image.set(y, x, color);
			error2 += derror2;
			if (error2 > dx) {
				y += y_incr;
				error2 -= dx * 2;
			}
		}
	}
	*/

	for (int x = x0; x <= x1; x++) {
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 * (1. - t) + y1 * t;
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
	}
}