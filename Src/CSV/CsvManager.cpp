#include "CsvManager.h"
#include <DxLib.h>
#include <fstream>
#include <sstream>

#include <array>
#include <vector>
#include <string>
#include <memory>
#include <cassert>

#include "./StatusPlayer.h"
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
	//LoadPlayerStatus();

	LoadStages();
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

void CsvManager::LoadStages(void)
{
	// CSV読み込み
	LoadStageMoveCsv(ResourceManager::PATH_CSV + PATH_STAGE_MOVE, STAGE_MOVE_X, STAGE_MOVE_Y, true);

	//LoadStageGravityCsv(ResourceManager::PATH_CSV + PATH_STAGE_GRAVITY, STAGE_GRAVITY_X, STAGE_GRAVITY_Y, true);
}

void CsvManager::LoadStageMoveCsv(const std::string& _path, int _xSize, int _ySize, bool _isLabelSkip)
{
	/*　csvファイル読み込み処理　*/

	// 空白時の値
	const int BLANK_NUM = -1;

	// ラベルの行をスキップするか否か
	bool isSkip = _isLabelSkip;

	// 縦横のマップ値[列数]
	StageMap::MoveStagePlace dataNums;

	// 行
	std::string line;

	// セーブファイルパス

	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(_path);
	std::istringstream fileStream(fileContent);

	int linePos = 0; // 行数
	int cellPos = 0; // セル数

	// 行ごとに読み込み
	while (getline(fileStream, line))
	{
		std::stringstream ss(line);
		std::string text;

		if (isSkip)
		{
			// ラベルの行をスキップ
			isSkip = false;
			continue;
		}
		
		// コンマごとにセルを読み込み
		while (getline(ss, text, ','))
		{
			// セル読み込み範囲を超えた場合、読み込み終了
			if (cellPos >= _xSize) { break; }

			// string→int変換
			int num = 0;
			UtilityCommon::ChangeString(text, num, BLANK_NUM);

			// セルの数値を格納
			dataNums[linePos][cellPos] = num;

			// セル位置移動
			cellPos++;
		}

		// 列数がステージ幅未満の時、空白の値にする
		while (cellPos < _xSize)
		{
			dataNums[linePos][cellPos++] = BLANK_NUM;
		}

		if (linePos++ >= (_ySize - 1))
		{
			// ステージリストに格納
			stage_.move.emplace_back(dataNums);

			isSkip = _isLabelSkip;
			linePos = 0;
		}

		cellPos = 0;
	}

	// 読み込まれていない領域を全て-1にする
	while (linePos < _ySize)
	{
		while (cellPos < _xSize)
		{
			// セルの数値を格納
			dataNums[linePos][cellPos++] = -1;
		}
		linePos++;
		cellPos = 0;
	}

	// ステージリストに格納
	stage_.move.emplace_back(dataNums);
}
void CsvManager::LoadStageGravityCsv(const std::string& _path, int _xSize, int _ySize, bool _isLabelSkip)
{
	/*　csvファイル読み込み処理　*/

	// 空白時の値
	const int BLANK_NUM = -1;

	// ラベルの行をスキップするか否か
	bool isSkip = _isLabelSkip;

	// 縦横のマップ値[列数]
	StageMap::GravityStagePlace dataNums;

	// 行
	std::string line;

	// セーブファイルパス

	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(_path);
	std::istringstream fileStream(fileContent);

	int linePos = 0; // 行数
	int cellPos = 0; // セル数

	// 行ごとに読み込み
	while (getline(fileStream, line))
	{
		std::stringstream ss(line);
		std::string text;

		if (isSkip)
		{
			// ラベルの行をスキップ
			isSkip = false;
			continue;
		}
		
		// コンマごとにセルを読み込み
		while (getline(ss, text, ','))
		{
			// セル読み込み範囲を超えた場合、読み込み終了
			if (cellPos >= _xSize) { break; }

			// string→int変換
			int num = 0;
			UtilityCommon::ChangeString(text, num, BLANK_NUM);

			// セルの数値を格納
			dataNums[linePos][cellPos] = num;

			// セル位置移動
			cellPos++;
		}

		// 列数がステージ幅未満の時、空白の値にする
		while (cellPos < _xSize)
		{
			dataNums[linePos][cellPos++] = BLANK_NUM;
		}

		if (linePos++ >= (_ySize - 1))
		{
			// ステージリストに格納
			stage_.gravity.emplace_back(dataNums);

			isSkip = _isLabelSkip;
			linePos = 0;
		}

		cellPos = 0;
	}

	// 読み込まれていない領域を全て-1にする
	while (linePos < _ySize)
	{
		while (cellPos < _xSize)
		{
			// セルの数値を格納
			dataNums[linePos][cellPos++] = -1;
		}
		linePos++;
		cellPos = 0;
	}

	// ステージリストに格納
	stage_.gravity.emplace_back(dataNums);
}


std::string& CsvManager::GetHandlePathPlayer(void)
{
	return player_->GetHandlePath();
}

int CsvManager::GetStageMoveNum(int _type, int x, int y)
{
	return stage_.move[_type].at(y).at(x);
}
int CsvManager::GetStageGravityNum(int _type, int x, int y)
{
	return stage_.gravity[_type].at(y).at(x);
}


/*
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
}*/