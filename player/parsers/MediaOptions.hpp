#pragma once

#include "constants.hpp"

#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace ResourcesXlf
{
    class MediaOptions
    {
    public:
        MediaOptions(int id, boost::optional<std::string> path, int duration);
        MediaOptions(const xml_node& node);

        static std::string getType(const xml_node& node);

        int id() const;
        boost::optional<std::string> path() const;
        int duration() const;

    protected:
        MediaOptions() = default;

        void setId(int id);
        void setUri(const boost::optional<std::string>& path);
        void setDuration(int duration);

    private:
        int m_id;
        boost::optional<std::string> m_path;
        int m_duration;
    };
}
