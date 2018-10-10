#pragma once

#include <memory>
#include <filesystem>
#include <boost/optional/optional.hpp>

#include "control/IBackground.hpp"
#include "adaptors/IImageAdaptor.hpp"

using FilePath = std::filesystem::path;

class IFileSystemAdaptor
{
public:
    virtual ~IFileSystemAdaptor() = default;
    virtual void setResourcesDirectory(FilePath path) = 0;
    virtual FilePath resourcesDirectory() const = 0;
    virtual bool isRegularFile(FilePath path) const = 0;
};

class FileSystemAdaptor : public IFileSystemAdaptor
{
public:
    void setResourcesDirectory(FilePath path) override
    {
        m_resourcesDirectory = path;
    }

    FilePath resourcesDirectory() const override
    {
        return m_resourcesDirectory;
    }

    bool isRegularFile(FilePath path) const override
    {
        return std::filesystem::is_regular_file(path);
    }

private:
    FilePath m_resourcesDirectory;

};

#include <gmock/gmock.h>
class MockFileSystemAdaptor : public IFileSystemAdaptor
{
public:
    MOCK_METHOD1(setResourcesDirectory, void(FilePath path));
    MOCK_CONST_METHOD0(resourcesDirectory, FilePath());
    MOCK_CONST_METHOD1(isRegularFile, bool(FilePath path));
};

class BackgroundBuilder
{
public:
    BackgroundBuilder(std::unique_ptr<IFileSystemAdaptor>&& filesystem);

    std::unique_ptr<IBackground> build();

    BackgroundBuilder& adaptor(std::unique_ptr<IImageAdaptor>&& adaptor);
    BackgroundBuilder& width(int width);
    BackgroundBuilder& height(int height);
    BackgroundBuilder& color(const boost::optional<std::string>& color);
    BackgroundBuilder& path(const boost::optional<std::string>& path);

protected:
    virtual std::unique_ptr<IBackground> createBackground();

private:
    void checkWidth(int width);
    void checkHeight(int height);
    void checkPath(FilePath path);

private:
    std::unique_ptr<IFileSystemAdaptor> m_filesystem;
    std::unique_ptr<IImageAdaptor> m_adaptor;
    int m_width;
    int m_height;
    boost::optional<uint32_t> m_hexColor;
    FilePath m_path;

};
