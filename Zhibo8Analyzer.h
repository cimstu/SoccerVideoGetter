#pragma once

#include "string"

class Zhibo8Analyzer
{
public:
	Zhibo8Analyzer();
	~Zhibo8Analyzer();

	bool DownloadVideoPage(const std::string& url, const std::wstring& download_path);
	bool ExtractUrl(const std::wstring& download_path);
};

