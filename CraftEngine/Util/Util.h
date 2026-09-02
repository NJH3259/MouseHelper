#pragma once

#include <string>
#include <fstream>
#include <random>
#include <cassert>

namespace Util
{
	//랜덤 엔진 반환 함수
	inline std::mt19937& GetRandomEngine()
	{
		//프로그램 실행 중 1번만 실행되는 랜덤 엔진
		//겹치는 수가 적을 수록 랜덤 성능이 좋음 -> 그러나 연산이 더 많아짐
		static std::mt19937 engine;
		return engine;
	}

	//랜덤 시드 설정 함수
	inline void SetRandomSeed() {
		std::random_device randomDevice;

		//랜덤 엔진에 종자값(시드) 설정
		//functor 호출
		GetRandomEngine().seed(randomDevice());
	}

	//정수 난수 함수
	inline int RandomRange(int min, int max) {
		//min에서 max까지 균등하게 부동소수점 난수를 생성해주는 분포 정의
		std::uniform_int_distribution<int> distribution(min, max);

		//난수 반환
		return distribution(GetRandomEngine());
	}

	inline float RandomRange(float min, float max) {
		//min에서 max까지 균등하게 부동소수점 난수를 생성해주는 분포 정의
		std::uniform_real_distribution<float> distribution(min, max);

		//난수 반환
		return distribution(GetRandomEngine());
	}

	inline std::string LoadImageFromFile(const std::string& fileName, const std::string& fileFolder)
	{
		std::string filePath = fileFolder + fileName;

		FILE* file = nullptr;
		fopen_s(&file, filePath.c_str(), "rt");

		if (!file)
		{
			assert(false && "Can't read file");
			return nullptr;
		}

		//파일 내용을 저장할 버퍼 확인 후 파일 길이 확인
		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file);

		//파일 제일 끝위치 구한 후 처음으로 이동
		rewind(file);

		//파일을 저장할 버퍼
		char* buffer = new char[fileSize] {};

		//파일 읽기
		size_t readSize = fread(buffer, sizeof(char), fileSize, file);

		assert(readSize > 0 && "No data in the file");

		std::string image(buffer, readSize);

		delete[] buffer;
		buffer = nullptr;

		fclose(file);

		return image;
	}

	inline std::vector<std::vector<int>> LoadMapAsGrid(const std::string& filepath) {
		std::ifstream file(filepath);
		std::string line;
		std::vector<std::vector<int>> grid;

		while (std::getline(file, line)) { // 한 줄씩 string으로 읽음
			std::vector<int> row;
			for (char ch : line) {
				if (ch == '1') {
					row.push_back(1); // 벽 (이동 불가)
				}
				else if (ch == '0') {
					row.push_back(0); // 길 (이동 가능)
				}
			}
			if (!row.empty()) {
				grid.push_back(row);
			}
		}
		return grid;
	}
}