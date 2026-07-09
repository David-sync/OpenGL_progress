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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	delete[] data;

	return textureID;

}