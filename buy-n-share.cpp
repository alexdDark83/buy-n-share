/**
 * @file buy-n-share.cpp
 */

#include <thread>

#include <argtable2.h>
#include <string>
#include <cstring>
#include <iostream>
#include <signal.h>

#include "buy-n-share.h"

#ifdef _WIN32
void setSignalHandler(int signal)
{
}
#else
void signalHandler(int signal)
{
	switch(signal)
	{
	case SIGINT:
		stopRequest = true;
		if (toxclient)
			toxclient->stop();
		std::cerr << MSG_INTERRUPTED << std::endl;
		break;
	default:
		break;
	}
}

void setSignalHandler(int signal)
{
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = &signalHandler;
	sigaction(signal, &action, NULL);
}
#endif

void read_loop
(
	ToxClient *toxclient,
	ToxReceiverStream *toxreceiverstream,
	bool stopRequest
)
{
	while(!stopRequest)
	{
		toxreceiverstream->readLine(toxclient);
	}
}

int main(int argc, char** argv)
{
    // Signal handler
    setSignalHandler(SIGINT);

	ClitoxConfig config(argc, argv);
	if (config.error())
		exit(config.error());

	std::string r = ToxClient::getIdHex(config.file_name);
	switch (config.cmd) 
	{
		case CMD_PRINT_TOX_ID:
			{
				if (r.empty())
				{
					toxclient = new ToxClient();
					toxclient->newId();
					write_tox(toxclient->getTox(), config.file_name);
					delete toxclient;
					r = ToxClient::getIdHex(config.file_name);
				}
				std::cout << r << std::endl;
			}
			break;
		default:
		{
			std::cerr << "Tox ID: " << r << std::endl;
			
			ToxReceiverStream toxreceiverstream(std::cin, std::cout, std::cerr);
			toxclient = new ToxClient(config.file_name, config.nick_name, config.status_message, &toxreceiverstream);
			
			toxclient->clearFriends();
			for (std::vector<std::string>::const_iterator it(config.ids_to.begin()); it != config.ids_to.end(); ++it)
			{
				toxclient->addFriend(*it);
			}
			std::thread t(read_loop, toxclient,  &toxreceiverstream, stopRequest);
			t.detach();
			toxclient->run();
			
			delete toxclient;
		}
	}

	return 0;
}