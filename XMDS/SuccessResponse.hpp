#ifndef SUCCESSRESPONSE_HPP
#define SUCCESSRESPONSE_HPP

#include "BaseResponse.hpp"

template<const char* ResponseName>
class SuccessResponse : public BaseResponse<ResponseName>
{
public:
    SuccessResponse(const std::string& xml_response) :
        BaseResponse<ResponseName>(xml_response)
    {
        if(this->response_tree())
        {
            m_success = this->response_tree()->get_child("success").template get_value<bool>();
        }
    }
    bool success() const
    {
        return m_success;
    }

private:
    bool m_success = false;

};


#endif // SUCCESSRESPONSE_HPP
