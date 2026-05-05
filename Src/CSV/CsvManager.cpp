#include "CsvManager.h"
#include <DxLib.h>
#include <fstream>
#include <sstream>

#include <array>
#include <vector>
#include <string>
#include <memory>
#include <cassert>

#include "../Manager/ResourceManager.h"
#include "../Utility/UtilityCommon.h"

CsvManager* CsvManager::instance_ = nullptr;


void CsvManager::CreateInstance(void)
{
	/* インスタンス生成処理 */
	if (instance_ == nullptr)
	{
		instance_ = new CsvManager();
	}
	// マネージャ読み込み処理
	instance_->Load();
}
void CsvManager::DestroyInstance(void)
{
	/* インスタンス削除処理 */
	if (instance_ != nullptr)
	{
		delete instance_;
	}
}

CsvManager::CsvManager(void)
	:stage_(StageMap::StageMap())
{
#ifndef _DEBUG
	// リリースビルド時のみDXアーカイブを使用
	SetUseDXArchiveFlag(TRUE);
#endif
}


void CsvManager::Load(void)
{
	// CSV読み込み
	LoadStageMoveCsv(ResourceManager::PATH_CSV + PATH_STAGE_MOVE, true);

	LoadStageGravityCsv(ResourceManager::PATH_CSV + PATH_STAGE_GRAVITY, true);
}

std::string CsvManager::ReadCsvFile(const std::string& path)
{
	/* ビルド別ファイル読み込み処理 */

	std::string content;

#ifdef _DEBUG

	/* Debugビルド時、通常のファイルシステムから読み込み */

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

	/* Releaseビルド時、DxArchiveから読み込む */

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
	int fileSize = static_cast<int>(FileRead_size(path.c_str()));
	std::vector<char> buffer((fileSize + 1), '\0'); // +1 for null terminator
	FileRead_read(buffer.data(), fileSize, fileHandle);
	FileRead_close(fileHandle);
	content = std::string(buffer.data());
#endif

	return content;
}


void CsvManager::LoadStageMoveCsv(const std::string& _path, bool _isLabelSkip)
{
	/*　csvファイル読み込み処理　*/

	// 空白時の値
	const int BLANK_NUM = -1;

	// ラベルの行をスキップするか否か
	bool isSkip = _isLabelSkip;

	// 縦横のマップ値[列数]
	StageMap::MoveStagePlace dataFrontNums;
	StageMap::MoveStagePlace dataBackNums;

	// 行
	std::string line;

	// セーブファイルパス

	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(_path);
	std::istringstream fileStream(fileContent);

	int linePos = (STAGE_Y - 1); // 行数
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
			if (cellPos > (STAGE_X * 2)) { break; }

			// 前ステージと後ろステージの間はスキップ
			if (cellPos == STAGE_X)
			{
				cellPos++;
				continue;
			}

			// string→int変換
			int num = 0;
			UtilityCommon::ChangeString(text, num, BLANK_NUM);

			// セルの数値を格納
			if (cellPos < STAGE_X)
			{
				dataFrontNums[linePos][cellPos] = num;
			}
			else
			{
				int cell = (cellPos - STAGE_X) - 1;
				dataBackNums[linePos][cell] = num;
			}

			cellPos++;
		}

		// 列数がステージ幅未満の時、空白の値にする
		while (cellPos <= (STAGE_X * 2))
		{
			if (cellPos == STAGE_X)
			{
				cellPos++;
				continue;
			}

			// セルの数値を格納
			if (cellPos < STAGE_X)
			{
				dataFrontNums[linePos][cellPos] = BLANK_NUM;
			}
			else
			{
				int cell = (cellPos - STAGE_X) - 1;
				dataBackNums[linePos][cell] = BLANK_NUM;
			}

			cellPos++;
		}

		// １マップ登録時、初期化
		if (linePos-- <= 0)
		{
			// ステージリストに格納
			stage_.move.emplace_back(dataFrontNums);
			stage_.moveBack.emplace_back(dataBackNums);

			isSkip = _isLabelSkip;
			linePos = (STAGE_Y - 1);
		}

		cellPos = 0;
	}

	// 読み込まれていない領域を全て-1にする
	if (linePos >= (STAGE_Y - 1)) {
		return;
	}

	while (linePos >= 0)
	{
		while (cellPos <= (STAGE_X * 2))
		{
			if (cellPos == STAGE_X - 1) { continue; }

			// セルの数値を格納
			if (cellPos < STAGE_X)
			{
				dataFrontNums[linePos][cellPos] = BLANK_NUM;
			}
			else
			{
				dataBackNums[linePos][(cellPos / 2) + (cellPos % 2)] = BLANK_NUM;
			}

			cellPos++;
		}
		linePos--;
		cellPos = 0;
	}

	// ステージリストに格納
	stage_.move.emplace_back(dataFrontNums);
	stage_.moveBack.emplace_back(dataBackNums);
}
void CsvManager::LoadStageGravityCsv(const std::string& _path, bool _isLabelSkip)
{
	/*　csvファイル読み込み処理　*/

	// 空白時の値
	const int BLANK_NUM = -1;

	// ラベルの行をスキップするか否か
	bool isSkip = _isLabelSkip;

	// 縦横のマップ値[列数]
	StageMap::GravityStagePlace dataNums;
	StageMap::GravityStagePlace dataNumBacks;

	// 行
	std::string line;

	// セーブファイルパス

	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(_path);
	std::istringstream fileStream(fileContent);

	int linePos = (STAGE_Y - 1); // 行数
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
			if (cellPos > (STAGE_X * 2)) { break; }

			// 前ステージと後ろステージの間はスキップ
			if (cellPos == STAGE_X)
			{
				cellPos++;
				continue;
			}

			// string→int変換
			int num = 0;
			UtilityCommon::ChangeString(text, num, BLANK_NUM);

			// セルの数値を格納
			if (cellPos < STAGE_X)
			{
				// 前のステージ格納
				dataNums[linePos][cellPos] = num;
			}
			else
			{
				// 後ろのステージ格納
				int cell = (cellPos - STAGE_X) - 1;
				dataNumBacks[linePos][cell] = num;
			}

			// セル位置移動
			cellPos++;
		}

		// 列数がステージ幅未満の時、空白の値にする
		while (cellPos <= (STAGE_X * 2))
		{
			if (cellPos == STAGE_X)
			{
				cellPos++;
				continue;
			}

			// セルの数値を格納
			if (cellPos < STAGE_X)
			{
				// 前のステージ格納
				dataNums[linePos][cellPos] = BLANK_NUM;
			}
			else
			{
				// 後ろのステージ格納
				int cell = (cellPos - STAGE_X) - 1;
				dataNumBacks[linePos][cell] = BLANK_NUM;
			}

			// セル位置移動
			cellPos++;
		}

		// １マップ登録時、初期化
		if (linePos-- <= 0)
		{
			// ステージリストに格納
			stage_.gravity.emplace_back(dataNums);
			stage_.gravityBack.emplace_back(dataNumBacks);

			isSkip = _isLabelSkip;
			linePos = (STAGE_Y - 1);
		}

		cellPos = 0;
	}
	
	// 読み込まれていない領域を全て-1にする
	if (linePos >= (STAGE_Y - 1)) {
		return;
	}
	

	while (linePos >= 0)
	{
		while (cellPos <= (STAGE_X * 2))
		{
			if (cellPos == STAGE_X - 1) { continue; }

			// セルの数値を格納
			if (cellPos < STAGE_X)
			{
				// 前のステージ格納
				dataNums[linePos][cellPos] = BLANK_NUM;
			}
			else
			{
				// 後ろのステージ格納
				dataNumBacks[linePos][(cellPos / 2) + (cellPos % 2)] = BLANK_NUM;
			}

			cellPos++;
		}
		linePos--;
		cellPos = 0;
	}

	// ステージリストに格納
	stage_.gravity.emplace_back(dataNums);
	stage_.gravityBack.emplace_back(dataNumBacks);
}

/* ステージのマップ情報取得 */
int CsvManager::GetStageMoveNum(int _type, int x, int y)
{
	return stage_.move[_type].at(y).at(x);
}
int CsvManager::GetStageMoveBackNum(int _type, int x, int y)
{
	return stage_.moveBack[_type].at(y).at(x);
}
int CsvManager::GetStageGravityNum(int _type, int x, int y)
{
	return stage_.gravity[_type].at(y).at(x);
}
int CsvManager::GetStageGravityBackNum(int _type, int x, int y)
{
	return stage_.gravityBack[_type].at(y).at(x);
}