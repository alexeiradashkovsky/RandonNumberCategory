// RandonNumberCategory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <vector>

#include "RandomNumberClassification.h"
#include "ProcessMessage.h"

int server()
{
    ServerProcessMessage server_p;

    if (!server_p.connect_to_pip())
    {
        std::cerr << "ERROR: Failed to connect to pipe" << std::endl;
        return -3;
    }

    std::cout << "Server is Ready " << std::endl;
    while (true)
    {
        std::cout << "Enter a positive number bigger than 0: ";
        uint32_t n;
        std::cin >> n;

        if (n > 0)
        {
            // Generate and send messages after a valid 'n' is obtained
            std::vector<uint32_t> random_numbers = generate_random_numbers(n);
            std::ostringstream initial_msg;
            initial_msg << "The selected random numbers are:";
            for (auto const& number : random_numbers) {
                initial_msg << " " << number;
            }
            initial_msg << "\n";
            std::string init_str = initial_msg.str();
            server_p.send(init_str.c_str(), init_str.size());

            for (auto const& number : random_numbers) {
                std::string message = print_random_number_classification(number);
                server_p.send(message.c_str(), message.size());
            }
        }
        else if (n == 0)
        {
            std::cout << "Exiting server..." << std::endl;
            break;
        }
        else
            std::cerr << "Invalid input: n must be a positive number." << std::endl;
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
        std::cerr << "ERROR: Failed to connect to pipe" << std::endl;
        return -3;
    }

    while (true)
    {
        std::string x = client_p.wait_to_data();
        size_t end_pos = x.find("end");
        if (end_pos != std::string::npos)
        {
            std::cout << x.substr(0, end_pos);
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
        std::cerr << "ERROR: you did not pass arguments" << std::endl;
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

    std::cerr << "ERROR: you did not pass valid arguments (client/server)" << std::endl;
    return -1;
}
