#define buffersize 1024*1024

namespace susu_tools{

class susu_tcp_object{
public:
    susu_tcp_object(int port);
    ~susu_tcp_object();

    int set_port(int port);
    int get_port();

    int get_fd();

    int client_conect_to_url(const char* url);
    
    int server_listen_the_port();

private:
    int port = -1;
    int fd = -1;
    char buffer[buffersize];
};

}//namespace susu_tools