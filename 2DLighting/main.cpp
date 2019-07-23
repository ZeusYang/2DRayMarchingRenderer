#include <iostream>

#include "Renderer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

int main(int argc, char *argv[])
{
	const int width = 800, height = 800;
	Renderer::ptr renderer = std::shared_ptr<Renderer>(new Renderer());
	renderer->initialize(width, height);

	unsigned char *image = renderer->render();
	//stbi_flip_vertically_on_write(1);
	stbi_write_png("./result.png",
		width,
		height,
		4,
		static_cast<void*>(image),
		width * 4);

	return 0;
}