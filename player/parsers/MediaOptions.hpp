#pragma once

#include "constants.hpp"
#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace ResourcesXlf
{
    class MediaOptions
    {
    public:
        MediaOptions(const xlf_node& node);
        static std::string getType(const xlf_node& node);

        int id() const;
        boost::optional<std::string> uri() const;
        int duration() const;

    protected:
        MediaOptions() = default;

        void setId(int id);
        void setUri(const boost::optional<std::string>& uri);
        void setDuration(int duration);

    private:
        int m_id;
        boost::optional<std::string> m_uri;
        int m_duration;
    };
}
