#include "susu_fdstream.hpp"

using namespace susu_tools;

OStreamBase::OStreamBase() : std::ostream(this) {
}

int OStreamBase::overflow(int c) {	// std::streambuf::overflow()
	const char v = static_cast<char>(c);
        return on_write(&v,1);
}

std::streamsize OStreamBase::xsputn(const char * s, std::streamsize num) { // std::streambuf::xsputn()
        return on_write(s,(size_t)num);
}   

susu_fdstream::susu_fdstream(int fd): fd_(fd) {
}

int susu_fdstream::on_write(const char * s,size_t n) {
	return fd_>0?::write(fd_,s,n):0;
}    

susu_filestream::susu_filestream(const char *fileName) : susu_filestream(fopen(fileName,"a"),true)
{}	// Common practices to simplify construction

susu_filestream::susu_filestream(FILE *file,bool closeAtExit) : susu_fdstream(fileno(file)), fileToClose_(closeAtExit?file:nullptr)
{}

susu_filestream::~susu_filestream()
{
       	if (fileToClose_)
	{
		fclose(fileToClose_); 
	}
}	//try to close FILE* when deconstruct,if the FILE* is not nullptr

