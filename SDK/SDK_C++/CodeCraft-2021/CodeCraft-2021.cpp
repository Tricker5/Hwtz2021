#include <iostream>
#include "DataInput.h"
#include "ServerMgr.h"
#include "DataOutput.h"


int main()
{
	// TODO:read standard input
	// TODO:process
	// TODO:write standard output
	// TODO:fflush(stdout);
	DataInput data_input;
	DataOutput data_output;
	ServerMgr server_mgr(data_input, data_output);
	fflush(stdout);

#ifdef LOG
	std::cout << data_input.server_num << std::endl;
	std::cout << data_input.server_map.size() << std::endl;
	std::cout << data_input.vm_map.size() << std::endl;
	std::cout << data_input.server_map["host1CQ99"].toString() << std::endl;
	for(auto day_req : data_input.request_list){
		for(auto req : day_req){
			std::cout << req.toString() << std::endl;
		}
	}
#endif

	return 0;
}
