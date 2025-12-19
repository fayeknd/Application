#include "ScreenshotPNG.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

bool Screenshot::take = false;
std::string Screenshot::output;

void Screenshot::TakeScreenshot(std::string _output) {
	output = _output;
	take = true;
	//Marks screenshot to be taken (Screenshot must be done after the final framebuffer is drawn)
}

void Screenshot::PNG_RGBA8() {
	if (!take)
		return;
	take = false;
	if (output == "null") {
		char* buf = nullptr;
		size_t size = 0;
		_dupenv_s(&buf, &size, "USERPROFILE");
		std::string dir(buf, size - 1);
		output = dir + std::string("\\Pictures\\KittyEngine");
		free(buf);
	}
	int w, h;
	w = KTYGlobal::windowSize.x;
	h = KTYGlobal::windowSize.y;
	unsigned int channels = 4;

	unsigned int stride = channels * w;
	stride += (stride % 4) ? (4 - stride % 4) : 0;

	unsigned int bufSize = stride * h;

	std::vector<char> pixelBuffer(bufSize);

	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer.data());

	std::string filename = (
		(std::to_string(Time::dateTime::year()) + std::string("-"))
		+ (std::to_string(Time::dateTime::month()) + std::string("-"))
		+ (std::to_string(Time::dateTime::day()) + std::string("-"))
		+ ((Time::dateTime::hour() < 10) ? std::string("0") + std::to_string(Time::dateTime::hour()) : std::to_string(Time::dateTime::hour()))
		+ ((Time::dateTime::minute() < 10) ? std::string("0") + std::to_string(Time::dateTime::minute()) : std::to_string(Time::dateTime::minute()))
		+ ((Time::dateTime::second() < 10) ? std::string("0") + std::to_string(Time::dateTime::second()) : std::to_string(Time::dateTime::second()))
		);
	std::cout << filename << std::endl;
	std::filesystem::create_directory(output);
	output += std::string("\\") + filename;
	int index = 1;
	while (std::filesystem::exists(output))
		output += "(" + std::to_string(index++) + ")";
	output += ".png";
	stbi_flip_vertically_on_write(true);
	stbi_write_png(output.c_str(), w, h, channels, pixelBuffer.data(), stride);
	Debug::CLog("Taken screenshot '" + output + "'");
}