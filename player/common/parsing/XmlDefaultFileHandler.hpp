#pragma once

#include "Parsing.hpp"
#include "XmlDocVersion.hpp"
#include "XmlFileLoader.hpp"
#include "XmlFileSaver.hpp"
#include "common/PlayerRuntimeError.hpp"

class XmlDefaultFileHandler : public XmlFileLoader, public XmlFileSaver
{
public:
    DECLARE_EXCEPTION(XmlDefaultFileHandler)

protected:
    XmlNode loadXmlFrom(const FilePath& file) override;
    void saveXmlTo(const FilePath& file, const XmlNode& tree) override;

    virtual XmlDocVersion currentVersion() const = 0;
    virtual NodePath versionAttributePath() const = 0;
    virtual std::unique_ptr<XmlFileLoader> backwardCompatibleLoader(const XmlDocVersion& version) const = 0;

private:
    XmlDocVersion documentVersion(const XmlNode& tree) const;
    XmlNode parseXml(const FilePath& file);
};
