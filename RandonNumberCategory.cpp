// RandonNumberCategory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "RandomNumberClassification.h"
#include "ProcessMessage.h"

int server()
{
    ServerProcessMessage server_p;

    if (!server_p.connect_to_pip())
    {
        std::cerr << "ERROR: Failto connect to pip" << std::endl;
        return -3;
    }

    std::cout << "Servet is Ready " << std::endl;
    std::cout << "Enter positive number bigget the 0: ";
    uint32_t n;
    std::cin >> n;
    for (auto const& number : generate_random_numbers(n)) {
        std::string message = print_random_number_classificatrion(number);
        server_p.send(message.c_str(), message.size());
    }

    server_p.send("end", 3);
    server_p.disconnect();
    return 0;
}

int client()
{
    ClientProcessMessage client_p;
    if (!client_p.connect_to_pip())
    {
        std::cerr << "ERROR: Failto connect to pip" << std::endl;
        return -3;
    }

    while (true)
    {
        std::string x = client_p.wait_to_data();
        if (x.find("end") != std::string::npos)
        {
            std::cout << x << std::endl;
            client_p.disconnect();
            return 0;
        }

        std::cout << x;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "ERROR: you do not passed args" << std::endl;
        return -1;
    }

    std::string s{ argv[1] };
    if (s == "server")
    {
        return server();
    }

    if (s == "client")
    {
        return client();
    }

    std::cerr << "ERROR: you do not passed valid args (client/server)" << std::endl;
    return -1;
}
