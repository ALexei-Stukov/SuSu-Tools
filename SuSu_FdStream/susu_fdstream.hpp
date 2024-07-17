#include <streambuf>
#include <iostream>
#include <unistd.h>

//	These useful code comes from here: 
//		https://stackoverflow.com/questions/2746168/how-to-construct-a-c-fstream-from-a-posix-file-descriptor
//	writed by : 
//			Brasme
//	That is a good idea.
//
//
//	I just make a liitle change.
//	I change the code from struct to class.



//We hope to write a fd by [ostream].
//The [ostream] transfer [data] to [streambuf].
//
//But streambuf can not affect fd,so we need to override some function in streambuf.
//We override [overflow] and [xsputn].
//
//
// [on_write] is a virtual function,this function can ONLY impleted in subclass.
// This design can prevent us from using the parent class and calling the parent's function.

namespace susu_tools{

class OStreamBase: public std::ostream, std::streambuf {
public:
        explicit OStreamBase();/* : std::ostream(this) {}*/

        virtual int on_write(const char *s,size_t n) = 0;	//every subclass need to implete(override) this function

private:        
        inline int overflow(int c) override;/* {	// std::streambuf::overflow()
            const char v = static_cast<char>(c);
            return on_write(&v,1);
        }*/

        inline std::streamsize xsputn(const char * s, std::streamsize num) override;/* { // std::streambuf::xsputn()
            return on_write(s,(size_t)num);
        }   */
};

//In this class, we had link ostream to fd.
//But sometime we want to link ostream to FILE.
//So we still need some extra work.
class susu_fdstream:public OStreamBase {
public:
        explicit susu_fdstream(int fd);/*: fd_(fd); {}*/
private:
        int fd_;
        int on_write(const char * s,size_t n) override;/* { return fd_>0?::write(fd_,s,n):0; }    */
};

//Now we finish the work.
class susu_filestream:public susu_fdstream{
public:
	explicit susu_filestream(const char *fileName);/* : susu_filestream(fopen(fileName,"a"),true) {}	// Common practices to simplify construction
*/
	explicit susu_filestream(FILE *file,bool closeAtExit=false);/* : susu_fdstream(fileno(file)), fileToClose_(closeAtExit?file:nullptr) {}
*/
        ~susu_filestream() override;/* { if (fileToClose_) fclose(fileToClose_); }*/	//try to close FILE* when deconstruct,if the FILE* is not nullptr
private:
        FILE *fileToClose_;
};

}//namespace susu_tools


// how to use susu_filestream
/*int main()
{
	FILE *f=fopen("file1.txt","w");
	susu_tools::susu_filestream os(f);
	os << "A test " << 1 << " and " << 2;
	fclose(f);
	return 0;
}*/

//need a ifstream.
