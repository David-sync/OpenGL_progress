#define _CRT_SECURE_NO_WARNINGS
#include "loadingBMP.h"

GLuint loadBMP_custom(const char* imagepath)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int widght, height;

	unsigned int imageSize;
	unsigned char* data; // chẳng phải con trỏ data này sẽ bằng với biến dataPos trên sao?

	FILE* file = fopen(imagepath, "rb");

	if (!file)
	{
		printf("Couldn't open this image\n");
		return 0;
	}

	if (fread(header, sizeof(char), 54, file) != 54) // chỗ này tác giả ghi tham số thứ 2 là 1, tôi đọc doc thì thấy nó bảo tham số thứ 2 là size nên tôi nghĩ nó là size của từng block định đọc, nên chắc là sizeof(char)
	{
		printf("Not a correct .bmp file!\n");
		return false; // sao lại return false thay vì 0 như trên nhỉ?
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct .bmp file!\n");
		return 0; // sao lại return 0 mà không phải false nhỉ? hay do giá trị trả về của fread là true/false?
	}

	// đọc byte
	dataPos = *(int*)&(header[0x0A]); //đoạn này là đọc từ trong ngoặc ra nhỉ, lấy ra địa chỉ của header[0x0A], thêm * là lấy ra giá trị? ủa sao ban đầu không để yên header[0x0A] nhỉ, nó là lấy giá trị luôn rồi mà. Xong rồi ép kiểu thành con trỏ int
	imageSize = *(int*)&(header[0x22]); // không hiểu
	widght = *(int*)&(header[0x12]); // không hiểu
	height = *(int*)&(header[0x16]); // không hiểu

	if (imageSize == 0)
	{
		imageSize = widght * height * 3;
	}

	if (dataPos == 0)
	{
		dataPos = 54; // là sao ta
	}

	data = new unsigned char[imageSize]; // này là cấp phát vùng nhớ heap với size là imageSize nhỉ

	fread(data, 1, imageSize, file); // ủa sao nó biết mà né 54 byte đầu ra để đọc chính xác data của ảnh nhỉ, đâu thấy nó truyền vào dataPos đâu?

	fclose(file); // dữ liệu chảy vào trong data rồi nên đóng handle file lại

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widght, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] data;

	return textureID;

}


GLuint loadDDS(const char* imagepath)
{
	unsigned char header[124];

	FILE* fp;

	fp = fopen(imagepath, "rb");

	if (fp == NULL)
	{
		return 0;
	}

	char filecode[4];

	fread(&filecode[0], 1, 4, fp);
	if (*(int*)&filecode[0] != *(const int *)"DDS ") // hoặc so sánh với 0x20534444 luôn cho nhanh
	{
		fclose(fp);
		return 0;
	}

	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&header[8];
	unsigned int width= *(unsigned int*)&header[12];
	unsigned int linearSize = *(unsigned int*)&header[16];
	unsigned int mipMapCount = *(unsigned int*)&header[24];
	unsigned int fourCC= *(unsigned int*)&header[80];

	unsigned char* buffer;
	unsigned int bufferSize;

	if (mipMapCount > 1)
	{
		bufferSize = linearSize * 2;
	}
	else
	{
		bufferSize = linearSize;
	}
	
	buffer = (unsigned char*) malloc(bufferSize * sizeof(unsigned char*));

	fread(buffer, 1, bufferSize, fp);

	fclose(fp);

	// DXT compress case
	unsigned int components = (fourCC == FOURCC_DXT1) ? 4 : 3;

	unsigned int blockSize; // để tính size bên dưới
	unsigned int format;


	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		blockSize = 8;
		break;

	case FOURCC_DXT3:

		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		blockSize = 16;
		break;

	case FOURCC_DXT5:

		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		blockSize = 16;
		break;

	default:

		free(buffer);
		return 0;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	unsigned int offset = 0;

	// đại khái tận dụng nguyên lí chia mipMap để chặn ở đoạn width:height 0:0 FALSE, dừng lặp khi ảnh không thể chia nhỏ được nữa
	for (unsigned int level = 0; level < mipMapCount && (width || height); level++)
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}

	free(buffer);

	return textureID;
}