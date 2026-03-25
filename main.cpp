#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	//해상도 설정
	int length = 720;
	int width = 1280;

	//YUV의 크기 공식 적용
	int Y = length * width;
	int U = (length / 2) * (width / 2);
	int V = (length / 2) * (width / 2);
	int frameSize = Y + U + V;


	FILE* org = NULL;
	FILE* copy = NULL;

	org = fopen("./PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	copy = fopen("./PeopleOnStreet_1280x720_30_ExtractY.yuv", "wb");

	if (org == NULL) {
		printf("해당 파일 없음");
		exit(1);
	}

	unsigned char* buffer = (unsigned char*)malloc(frameSize);

	/*for (int i = 0; i < 30; i++) {
		//버퍼 초기화
		memset(buffer, 0, frameSize);
		//Y크기만큼 버퍼에 저장
		fread(buffer, sizeof(unsigned char), frameSize, org);
		//버퍼를 copy파일에 복사
		fwrite(buffer, sizeof(unsigned char), frameSize, copy);
	}*/

	int roi_x1 = 400, roi_y1 = 200;
	int roi_x2 = 800, roi_y2 = 520;

	for (int i = 0; i < 30; i++) {
		if (fread(buffer, 1, frameSize, org) < frameSize) break; // 버퍼 복사 프레임 무결성 확인

		// 1. [검증용] ROI 내부 데이터 출력 (첫 프레임에서)
		if (i == 0) {
			printf("\n[ROI 내부 - 원본 선명한 구간 (600, 300) 8x8]\n");
			for (int row = 0; row < 8; row++) {
				for (int col = 0; col < 8; col++) {
					printf("%3d ", buffer[(300 + row) * width + (600 + col)]);
				}
				printf("\n");
			}
		}

		for (int y = 0; y < length; y++) {
			for (int x = 0; x < width; x++) {
				if(!(x >= roi_x1 && x <= roi_x2 && y >= roi_y1 && y <= roi_y2)) {
					// 8x8 블록의 시작점 좌표를 찾아서 그 값을 대입 (모자이크 효과)
					int block_y = (y / 8) * 8;
					int block_x = (x / 8) * 8;
					buffer[y * width + x] = buffer[block_y * width + block_x];

				}
			}

		}

		// 2. [검증용] ROI 외부(모자이크) 데이터 딱 한 번만 출력 (첫 프레임에서)
		if (i == 0) {
			printf("\n[ROI 외부 - 모자이크 처리된 구간 (100, 100) 8x8]\n");
			for (int row = 0; row < 8; row++) {
				for (int col = 0; col < 8; col++) {
					printf("%3d ", buffer[(100 + row) * width + (100 + col)]);
				}
				printf("\n");
			}
		}
		fwrite(buffer, 1, frameSize, copy);
	}

	printf("\n추출완료");
	fclose(org);
	fclose(copy);
}