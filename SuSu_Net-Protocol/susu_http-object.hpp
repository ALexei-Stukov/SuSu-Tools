#include "susu_tcp-object.hpp"
#include "susu_http-analyser.hpp"

namespace susu_tools{
class susu_http_object{
public:

private:
    susu_http_analyser http_analyser;
    susu_tcp_object tcp_connection;
};

}//namespace susu_tools