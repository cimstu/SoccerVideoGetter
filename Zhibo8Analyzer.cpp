#include "stdafx.h"
#include "Zhibo8Analyzer.h"
#include <curl/curl.h>

Zhibo8Analyzer::Zhibo8Analyzer()
{
}


Zhibo8Analyzer::~Zhibo8Analyzer()
{
}

bool Zhibo8Analyzer::DownloadVideoPage(const std::string& url, const std::wstring& download_path)
{
	CURL *curl = 0;
	CURLcode res = CURLE_OK;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		/* example.com is redirected, so we tell libcurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		FILE* fp = NULL;
		_wfopen_s(&fp, download_path.data(), L"wb");
		if (!fp)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

			return false;
		}

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

			return false;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);

		fclose(fp);
	}
	return false;
}

bool Zhibo8Analyzer::ExtractUrl(const std::wstring & download_path)
{
	FILE* fp = NULL;
	_wfopen_s(&fp, download_path.data(), L"r, css=UTF-8");

	for(fread)

	return false;
}
