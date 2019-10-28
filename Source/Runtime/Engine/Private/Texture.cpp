
#include "Precompiled.h"
#include "Texture.h"
#include "MathHeaders.h"
#include "lodepng.h"
//#include "BMPReader.h"

#include <vector>

Texture::Texture(char* resourceName)
{
	//Buffer = BMPReader::ReadBMP(resourceName, &Width, &Height);
	//if (Buffer == nullptr)
	//{
	//	Width = 0;
	//	Height = 0;
	//	return;
	//}

	LoadPNGFile(resourceName);
}

void Texture::LoadPNGFile(char * resourceName)
{
	std::vector<unsigned char> image;
	unsigned int error = lodepng::decode(image, Width, Height, resourceName);
	if (error != NULL)
	{
		Width = 0;
		Height = 0;
		bInitialized = false;
	}

	Buffer = new LinearColor[Width * Height];
	for (unsigned int j = 0; j < Height; j++)
	{
		for (unsigned int i = 0; i < Width; i++)
		{
			int pixelIndex = j * Width + i;
			int imageIndex = pixelIndex * 4;

			Buffer[j * Width + i] = LinearColor(
				Color32(
					image[imageIndex],
					image[imageIndex + 1],
					image[imageIndex + 2],
					image[imageIndex + 3]
				)
			);
		}
	}

	bInitialized = true;
}

void Texture::Release()
{
	if (bInitialized)
	{
		free(Buffer);
		Buffer = nullptr;
	}

	bInitialized = false;
}

LinearColor* Texture::GetBuffer() const
{
	return Buffer;
}

LinearColor Texture::GetPixel(Vector2 InUV) const
{
	int pixelX = Math::FloorToInt(InUV.X * Width);
	int pixelY = Math::FloorToInt(InUV.Y * Height);

	pixelX %= Width;
	pixelY %= Height;

	return Buffer[Width * pixelY + pixelX];
}

