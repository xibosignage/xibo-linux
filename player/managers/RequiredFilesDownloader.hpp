#pragma once

#include "xmds/RequiredFiles.hpp"

#include <sigc++/sigc++.h>

using FileDownloadFinished = std::function<void()>;
class DownloadedFile;

class RequiredFilesDownloader
{
public:
    void download(const RegularFiles& files, FileDownloadFinished callback);

private:
    void downloadAllFiles(const RegularFiles& files);
    void processDownloadedFile(const DownloadedFile& file);

private:
    sigc::signal<void()> m_filesDownloaded;
    size_t m_finishedDownloads = 0;

};
