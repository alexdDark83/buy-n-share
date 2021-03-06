/**
 * @file buy-n-share.cpp
 */

#include <argtable2.h>
#include <string>
#include <cstring>
#include <iostream>
#include "buy-n-share.h"
#include "buy-n-share-config.h"

#include "fbclient.h"

int main(int argc, char** argv)
{

	BuyNShareConfig config(argc, argv);
	if (config.error())
		exit(config.error());

	FBClient cli(config.base_url);

	switch (config.cmd) 
	{
		case CMD_BALANCE:
			break;
		case CMD_LS_USER:
			{
				const Users *u = cli.ls_user(config.locale);
				if (u)
				{
					for (auto it(u->users()->begin()); it != u->users()->end(); ++it)
					{
						std::cout << it->id() << "\t"
							<< it->cn()->str() << "\t"
							<< it->locale()->str() << "\t"
							<< it->key()->str() << "\t"
							<< it->geo()->lat() << "\t"
							<< it->geo()->lon() << "\t"
							<< it->geo()->alt() << "\t"
							<< std::endl;
					}
				}
				else
				{
					std::cerr << cli.url << " HTTP code " << cli.code << ": " << cli.retval << std::endl;
				}
			}
			break;
		case CMD_ADD_USER:
			{
				const User *u = cli.add_user(config.cn, config.key, config.locale, config.lat, config.lon, config.alt);
				if (u)
				{
					config.id = u->id();
					config.key = u->key()->str();
					std::cout << u->id() << "\t" << u->key()->str() << std::endl;
				}
				else
				{
					std::cerr << cli.url << " HTTP code " << cli.code << ": " << cli.retval << std::endl;
				}
			}
			break;
		default:
		// case CMD_MEAL
			break;
	}

	return 0;
}
