#pragma once
#include "main.h"
#include "global.h"

class Texture {
private:
	Vector2 _imgSize;
	GLint _format;
	bool _dataAssigned;
	bool _initialised;
	unsigned int _channels;
	unsigned int _thisID;
	std::vector<unsigned char> data;
	static std::vector<unsigned int> boundTextures;
	static unsigned int textureCount;
public:

	//GL Image format is:
	//Red byte, green byte, blue byte, repeat.
	//defining image size would be std::vector<unsigned int> image (width * height * channels)
	bool DataAssigned() { return _dataAssigned; }
	bool Initialised() { return _initialised; }
	GLint Format() { return _format; }
	unsigned int Channels() { return _channels; }
	unsigned int ID() { return _thisID; }
	Vector2 Dimensions() { return _imgSize; }
	std::vector<unsigned char> ImageData() { return data; }

	void BindTexture(int textureType = GL_TEXTURE_2D) {
		
		/*
		OpenGL can only bind so many textures at once. Each texture COULD be rebound each time it needs to be used, but
		this increases the amount of gl calls. An optimisation is to take advantage of the bind limit opengl has, which 
		in the case of an RTX 3080, would be 32, and only make the gl call if the texture is not currently bound. 
		*/

		for (int i = 0; i < boundTextures.size(); i++) {
			if (boundTextures[i] == _thisID)  // this texture has already been bound, return.
				return;
		}
		if (boundTextures.size() == KTYGlobal::GLMaxTextures())
			boundTextures.erase(boundTextures.begin()); /* At this point the texture is not in the boundTextures list, 
														   and the boundTextures vector is larger than GLMaxTextures. 
														   Remove first element and push new element to the back. */
		boundTextures.push_back(_thisID);
		glBindTexture(textureType, _thisID);
	}
	void CreateTexture(Vector2 size, GLint format = GL_RGBA, GLint internalFormat = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, bool mipmap = true, unsigned int channels = 4, Colour initialiser = Colour(1), std::vector<unsigned char> pixels = std::vector<unsigned char>{0}) {
		initialiser *= 255;
		if (_initialised)
			return;
		_thisID = textureCount;
		_imgSize = size;
		_format = format;
		_channels = channels;
		glGenTextures(1, &_thisID);
		glBindTexture(GL_TEXTURE_2D, _thisID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (!_dataAssigned && pixels == std::vector<unsigned char>{0})
		{
			std::vector<unsigned char> newData((int)size.x * (int)size.y * channels);
			for (int i = 0; i < newData.size(); i+=channels) {
				for (int x = 0; x < channels; x++) {
					newData[i + x] = initialiser[x];
				}
			}
			data = newData;
		}
		if (pixels != std::vector<unsigned char>{0}) {
			data = pixels;
		}
		_dataAssigned = true;
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, (int)size.x, (int)size.y, 0, format, type, &data[0]);
		glGenerateMipmap(GL_TEXTURE_2D);
		textureCount++;
		_initialised = true;
	}
	void RebindData() {
		for (int i = 0; i < boundTextures.size(); i++) {
			if (boundTextures[i] == _thisID)
				boundTextures.erase(boundTextures.begin() + i);
		}
		glBindTexture(GL_TEXTURE_2D, _thisID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, _format, (int)_imgSize.x, (int)_imgSize.y, 0, _format, GL_UNSIGNED_BYTE, &data[0]);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	void SetData(std::vector<unsigned char> newData, Vector2 size, GLint format, unsigned int channels) {
		(!_dataAssigned) ? _dataAssigned = true : NULL;
		data = newData;
		_channels = channels;
		_imgSize = size;
		_format = format;
		RebindData();
	}
	// R G B A  R G B A  R G B A
	// R G B A  R G B A  R G B A
	// R G B A  R G B A  R G B A
	// x0,y0 ^
	void SetPixel(Vector2 pixel, Colour col) {
		col *= 255; 
		pixel = Vector2(Math::clamp(pixel.x, (double)0, _imgSize.x - 1), Math::clamp(pixel.y, (double)0, _imgSize.y - 1));  // pixel cannot be out of bounds of the array
		int stride = _imgSize.x * _channels;
		int index = (data.size() - stride + (pixel.x * _channels)) - (stride * pixel.y);
		data[index + 0] = col.x;
		data[index + 1] = col.y;
		data[index + 2] = col.z;
		data[index + 3] = col.w;
	}
	void SetPixels(std::vector<Vector2> pixels, Colour col) {
		for (int i = 0; i < pixels.size(); i++) {
			SetPixel(pixels[i], col);
		}
	}
	void SetPixels(std::vector<Vector2> pixels, std::vector<Colour> cols) {
		for (int i = 0; i < pixels.size(); i++) {
			SetPixel(pixels[i], cols[i]);
		}
	}
};