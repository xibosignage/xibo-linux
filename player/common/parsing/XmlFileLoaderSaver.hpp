#pragma once

#include "Parsing.hpp"
#include "XmlFileLoader.hpp"
#include "XmlFileSaver.hpp"
#include "common/PlayerRuntimeError.hpp"

class XmlDefaultFileLoader : public XmlFileLoader, public XmlFileSaver
{
public:
    DECLARE_EXCEPTION(XmlDefaultFileLoader)

protected:
    using DocVersionType = std::string;

    XmlNode loadXmlFrom(const FilePath& file) override;
    void saveXmlTo(const FilePath& file, const XmlNode& tree) override;

    virtual std::string currentVersion() const = 0;
    virtual boost::optional<DocVersionType> documentVersion(const XmlNode& tree) const = 0;
    virtual std::unique_ptr<XmlFileLoader> backwardCompatibleLoader(const DocVersionType& version) const = 0;

private:
    XmlNode parseXml(const FilePath& file);
};
