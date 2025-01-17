// CPPBot.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include <chrono>
#include "corefunc.h"
#include "userfunc.h"
#include <string>
#include <unistd.h>
#include <stdint.h>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;
vector<GrowtopiaBot> bots;

int main() {
	ifstream i("config.json");
	json j;
	i >> j;
	init();
	system("clear");
	cout << "GrowtopiaBot v1.0 [Not included QT]! by GrowtopiaNoobs and DrOreo002" << endl;
	
	if (j["ip"] == "Ip Target")
	{
		cout << "Please change the \"Ip Target\" to ip gtps on config.json" << endl;
		return 0;
	}
	
	GrowtopiaBot bot = {
		j["username"], j["password"], j["ip"], j["port"]
	};
	cout << "------------------------" << endl;
	cout << "Bot loaded!. Using username " << j["username"].get<string>() << ", With owner " + j["ownerUsername"].get<string>() << endl;

	bot.gameVersion = j["gameVersion"].get<string>();
	bot.worldName = j["targetWorld"].get<string>();
	bot.ownerUsername = j["ownerUsername"].get<string>();

	bot.userInit();
	bots.push_back(bot);

	std::thread inputThread([&bot]() {
        while (true) {
            string userInput;
            cout << "Masukkan perintah untuk dikirimkan sebagai paket: ";
            std::getline(cin, userInput);

            if (!userInput.empty()) {
                // Kirim paket ke semua bot
                for (int i = 0; i < bot.maxBots; ++i) {
                    ENetPeer* peer = bot.peers[i];
                    if (peer != nullptr) {
                        bot.SendPacket(2, userInput, peer);
                    }
                }
                cout << "Paket dikirim: " << userInput << endl;
            }
        }
    });
    // Loop utama untuk bot
    while (true) {
        bot.eventLoop();
        bot.userLoop();
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Hindari penggunaan CPU tinggi
    }

    // Pastikan thread berjalan paralel (detach digunakan untuk tidak blocking)
    inputThread.detach();

    return 0;
}