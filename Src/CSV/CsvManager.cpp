#include "CsvManager.h"
#include <DxLib.h>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include <memory>
#include <cassert>
#include <map>

#include "./StatusPlayer.h"
#include "./StatusEnemy.h"
#include "../Manager/ResourceManager.h"

CsvManager* CsvManager::instance_ = nullptr;

void CsvManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new CsvManager();
	}
	// マネージャ読み込み処理
	instance_->Load();
}

CsvManager& CsvManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		CreateInstance();
	}
	return *instance_;
}

void CsvManager::Destroy(void)
{
	if (instance_ != nullptr)
	{
		delete instance_;
	}
}

CsvManager::CsvManager(void)
{
#ifndef _DEBUG
	// リリースビルド時のみDXアーカイブを使用
	SetUseDXArchiveFlag(TRUE);
#endif
}



void CsvManager::Load(void)
{
	LoadPlayerStatus();

	LoadEnemy();
}

std::string CsvManager::ReadCsvFile(const std::string& path)
{
	std::string content;

#ifdef _DEBUG
	// デバッグビルド：通常のファイルシステムから読み込み
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::string error = "\ncsvファイルが読み込まれませんでした。\nファイルパス:";
		error += path;
		OutputDebugString(error.c_str());
		assert(false);
		return "";
	}

	// バッファ確保
	std::stringstream buffer;
	buffer << file.rdbuf();
	content = buffer.str();
	file.close();

#else
	// リリースビルド：DXアーカイブから読み込み
	int fileHandle = FileRead_open(path.c_str());

	if (fileHandle == 0)
	{
		// 読み込み失敗
		std::string error = "\ncsvファイルが読み込まれませんでした。\nファイルパス:";
		error += path;
		error += " ";
		OutputDebugString(error.c_str());
		assert(false);
		return "";
	}

	// バッファ確保
	int fileSize = FileRead_size(path.c_str());
	char* buffer = new char[fileSize + 1];

	FileRead_read(buffer, fileSize, fileHandle);
	buffer[fileSize] = '\0';
	FileRead_close(fileHandle);

	content = std::string(buffer);
	delete[] buffer;
#endif

	return content;
}


void CsvManager::LoadPlayerStatus(void)
{
	/*　csvファイル読み込み処理　*/

	// 文字列の一時格納配列
	int max = static_cast<int>(StatusPlayer::PARAM::MAX);
	std::string dataText[static_cast<int>(StatusPlayer::PARAM::MAX)];

	// 行
	std::string line;
	std::string path = (ResourceManager::PATH_CSV + PATH_PLAYER);

	// セーブファイルパス

	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(path);
	std::istringstream fileStream(fileContent);

	int length = 0;
	int param = 0; // 種類


	// 行読み込み
	while (getline(fileStream, line))
	{
		std::stringstream ss(line);
		std::string text;

		if (param == 0)
		{
			// ラベルはスキップ
			param++;
			length = 0;
			continue;
		}

		// 列読み込み(コンマごと)
		while (getline(ss, text, ','))
		{
			// カンマごとに区別されていない
			if (length < max)
			{
				dataText[length] = text;
				length++;
			}
		}

		length = 0;
		param++;
	}


	// 数値読込
	player_ = std::make_unique<StatusPlayer>();
	player_->LoadStatusParam(std::to_array(dataText));
}

void CsvManager::LoadEnemy(void)
{
	/*　csvファイル読み込み処理　*/

	// 文字列の一時格納配列
	int max = static_cast<int>(StatusEnemy::PARAM::MAX);
	std::string dataText[static_cast<int>(StatusEnemy::TYPE::MAX)][static_cast<int>(StatusEnemy::PARAM::MAX)];

	// 行
	std::string line;
	std::string path = (ResourceManager::PATH_CSV + PATH_ENEMY);

	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(path);
	std::istringstream fileStream(fileContent);

	int length = 0;
	int param = 0; // 種類

	// 行読み込み
	while (getline(fileStream, line))
	{
		std::stringstream ss(line);
		std::string text;

		if (param == 0)
		{
			// ラベルはスキップ
			param++;
			length = 0;
			continue;
		}

		// 列読み込み(コンマごと)
		while (getline(ss, text, ','))
		{
			// カンマごとに区別されていない
			if (length < max)
			{
				dataText[(param - 1)][length] = text;
				length++;
			}
		}

		length = 0;
		param++;
	}

	for (int i = 0; i < (param - 1); i++)
	{
		// 敵パラメータ登録
		enemy_[i] = std::make_unique<StatusEnemy>();
		enemy_[i]->LoadParam(std::to_array(dataText[i]));
	}
}


void CsvManager::SaveCSV(void)
{
	/* CSV書き込み処理 */
	/*

	int max = static_cast<int>(DATA_PLAYER::MAX);

	//　データ保存処理
	for (int y = 0; y < (SAVE_LENGTH - 1); y++)
	{
		// ラベル以外を書き出す
		for (int x = 0; x < (max - 1); x++)
		{
			// 数値 → 文字列 変換読み込み
			//loadString_[y + 1][x + 1] = std::to_string(loadValue_[y][x]);
		}
	}

	// 保存ファイル読み込み
	std::ofstream outputFile(DATA_HANDLE);
	if (!outputFile.is_open())
	{
		OutputDebugString(w"\ncsvファイルが開かれませんでした。\n");
		assert(false);
	}

	for (int y = 0; y < SAVE_LENGTH; ++y)
	{
		for (int x = 0; x < max; ++x)
		{
			// データ入力
			outputFile << loadString_[y][x];

			// カンマ入力
			if (x < (max - 1)) outputFile << ",";
		}
		outputFile << "\n"; // 改行
	}
	outputFile.close(); //保存終了

#ifdef _DEBUG
	OutputDebugString("\n正常に保存完了しました\n");
#endif
	*/
}


std::string& CsvManager::GetHandlePathPlayer(void)
{
	return player_->GetHandlePath();
}

std::string& CsvManager::GetHandlePathEnemy(int type)
{
	return enemy_[type]->GetHandlePath();
}