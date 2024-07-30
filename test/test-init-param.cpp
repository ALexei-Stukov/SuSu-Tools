#include "susu_init-param.hpp"

using namespace susu_tools;
using namespace std;

int main(int argc,char** argv)
{
	cout<<"---------------------------------------"<<endl;
	cout<<"now begin to test SuSu_InitParam"<<endl;

	if(argc <= 1 )
	{
		printf("too less param\n");
		return 0;
	}
	auto ins = susu_initparam::get_Init_Param_instance();
	
	cout<<"load param file:"<<argv[1]<<endl;
	ins->load_init_param(argv[1]);
	cout<<endl;

	cout<<"print all params"<<endl;
	ins->init_param_print();
	cout<<endl;

	cout<<"get one param"<<endl;
	cout<<"return string is:"<<ins->get_value("name")<<endl;
	cout<<endl;

	cout<<"get one param and change it to int"<<endl;
	cout<<"return is:"<<stoi(ins->get_value("limit"))<<endl;
	cout<<endl;
	
	cout<<"get a wrong param"<<endl;
	cout<<"return string is:"<<ins->get_value("WTF")<<endl;
	cout<<endl;


	return 0;
}
