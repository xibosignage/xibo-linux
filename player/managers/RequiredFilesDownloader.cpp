#include "RequiredFilesDownloader.hpp"

#include "HTTPDownloader.hpp"

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "utils/FilePath.hpp"

void RequiredFilesDownloader::download(const RegularFiles& files, FileDownloadFinished callback)
{
    m_filesDownloaded.connect(callback);

    downloadAllFiles(files);
}

void RequiredFilesDownloader::downloadAllFiles(const RegularFiles& files)
{
    for(auto&& file : files)
    {
        Log::trace("File type: {} ID: {} Size: {}", static_cast<int>(file.fileType), file.id, file.size);
        Log::trace("MD5: {} File name: {} Download type: {}", file.md5, file.filename, static_cast<int>(file.downloadType));

        Utils::httpDownloader().download(file.filename, file.path, [=](const DownloadedFile& file)
        {
            processDownloadedFile(file);
            if(m_finishedDownloads == files.size())
            {
                m_filesDownloaded.emit();
            }
        });
    }
}

void RequiredFilesDownloader::processDownloadedFile(const DownloadedFile& file)
{
    ++m_finishedDownloads;
    if(!file.downloadError)
    {
        Log::debug("[{}] Downloaded", file.name);
    }
    else
    {
        Log::error("[{}] Download error: {}", file.name, file.downloadError.message());
    }
}
