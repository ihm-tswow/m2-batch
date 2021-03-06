#include "m2.h"
#include "skin.h"
#include "file.h"
#include "config.h"

#include "thread_pool.hpp"
#include <StormLib.h>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <algorithm>

namespace fs = std::filesystem;

uint64_t now()
{
		return std::chrono::duration_cast<std::chrono::milliseconds>
				(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

bool should_extract(std::string name)
{
		std::transform(name.begin(), name.end(), name.begin(),
				[](unsigned char c) { return std::tolower(c); });
		return name.ends_with(".m2") || name.ends_with(".skin");
}

void exit_message(std::string const& message, int code)
{
		std::cerr << message << "\n";
		exit(code);
}

void extract_files()
{
		std::cout << "No existing extracts directory, extracting all models and skins...\n";
		fs::path datapath = fs::path(CLIENT_PATH) / "Data";
		std::vector<fs::path> mpqs = {
				datapath / "common.MPQ",
				datapath / "common-2.MPQ",
				datapath / "expansion.MPQ",
				datapath / "lichking.MPQ",
				datapath / "patch.MPQ",
				datapath / "patch-2.MPQ",
				datapath / "patch-3.MPQ",
		};

		HANDLE mpq = NULL;
		for (size_t i = 0; i < mpqs.size(); ++i)
		{
				std::string path = mpqs[i].string();
				std::cout << "Opening MPQ " << path << "\n";
				bool res = i == 0
						? SFileOpenArchive(path.c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq)
						: SFileOpenPatchArchive(mpq, path.c_str(), NULL, 0)
						;
				if (!res)
				{
						exit_message(
								  "Failed to open MPQ " 
								+ std::string(path)
								+ std::to_string(GetLastError())
								, GetLastError()
						);
				}
		}

		HANDLE find_handle = NULL;
		SFILE_FIND_DATA* find = new SFILE_FIND_DATA;
		std::vector<std::string> files;
		if ((find_handle = SFileFindFirstFile(mpq, "*", find, 0))) {
				if (should_extract(find->cFileName))
				{
						files.push_back(find->cFileName);
				}
		}
		while (SFileFindNextFile(find_handle, find)) {
				if (should_extract(find->cFileName))
				{
						files.push_back(find->cFileName);
				}
		}
		delete find;
		SFileFindClose(find_handle);

		for (size_t i = 0; i < files.size(); ++i)
		{
				std::cout << "\rExtracting " << i << "/" << files.size();
				fs::path out_path = fs::path(EXTRACTS_PATH) / files[i].c_str();
				fs::create_directories(out_path.parent_path());
				if (!SFileExtractFile(mpq, files[i].c_str(), out_path.string().c_str(), 0))
				{
						exit_message("Failed to extract " + files[i], 1);
				}
		}
		SFileCloseArchive(mpq);
}

M2Errors process_skin(std::vector<File<M2SkinHeader>> & skins, fs::path dir, fs::path rel_dir, std::string const& filename, M2Errors error)
{
		fs::path path = dir / filename;
		if (fs::exists(path))
		{
				skins.push_back(File<M2SkinHeader>(path, rel_dir / filename));
				File<M2SkinHeader>& header = (skins[skins.size() - 1]);
				return header.stream.good() ? M2Errors::SUCCESS : error;
		}
		return M2Errors::SUCCESS;
}

void _process_file(std::filesystem::path const& path, bool load_skins, synced_stream& stream)
{
		fs::path dir = path.parent_path();
		std::string filename = path.filename().string();
		std::string plain_filename = filename.substr(0, filename.size() - 3);

		fs::path rel_dir = std::filesystem::relative(dir, EXTRACTS_PATH);

		uint32_t errors = M2Errors::SUCCESS;
		File<M2Header> header = File<M2Header>(path,rel_dir/filename);
		if (!header.stream.good())
		{
				errors |= M2Errors::BAD_M2;
		}

		std::vector<File<M2SkinHeader>> skins;
		if (load_skins)
		{
				errors |= process_skin(skins, dir, rel_dir, plain_filename+"00.skin", M2Errors::BAD_SKIN_00);
				errors |= process_skin(skins, dir, rel_dir, plain_filename+"01.skin", M2Errors::BAD_SKIN_01);
		}

		if (errors != M2Errors::SUCCESS)
		{
				for (M2Script* script : SCRIPTS)
				{
						script->process_bad(header, skins, errors, stream);
				}
		}
		else
		{
				// don't multithread this, ifstreams can be read by scripts
				for (M2Script* script : SCRIPTS)
				{
						script->process(header, skins, stream);
				}
		}

}

void process_files()
{
		synced_stream stream;
		thread_pool pool;
		uint32_t fileCount = 0;

		bool multithread = true;
		bool load_skins = false;

		for (M2Script* script : SCRIPTS)
		{
				if (!(script->flags() & M2ScriptFlags::USE_THREADS))
				{
						multithread = false;
				}

				if (!(script->flags() & M2ScriptFlags::SKIP_SKINS))
				{
						load_skins = true;
				}
		}

		uint64_t start = now();
		for (const fs::directory_entry& dir_entry :
				fs::recursive_directory_iterator(EXTRACTS_PATH))
		{
				std::filesystem::path path = dir_entry.path().string();
				if (!path.string().ends_with(".m2"))
				{
						continue;
				}
				fileCount++;

				if (multithread)
				{
						pool.push_task([path,load_skins,&stream]() { _process_file(path, load_skins, stream); });
				}
				else
				{
						_process_file(path, load_skins, stream);
				}
		}

		pool.wait_for_tasks();

		for (M2Script* script : SCRIPTS)
		{
				script->finish(fileCount, stream);
		}
		std::cout << "Processed " << fileCount << " m2 files in " << now() - start << "ms\n";
}

int main()
{
		if (!fs::exists(EXTRACTS_PATH))
		{
				extract_files();
		}
		process_files();
		for (M2Script* script : SCRIPTS)
		{
				delete script;
		}
}