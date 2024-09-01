#include <random>
#include <regex>

#include "server.h"
#include "crypto.h"
#include "client.h"

std::vector<std::string> pending_trxs;

std::string generate_unique_id(const std::string& base_id) {
        static std::random_device rd; // 随机数生成器
        static std::mt19937 gen(rd()); // 以随机设备初始化 Mersenne Twister 生成器
        std::uniform_int_distribution<> dis(1000, 9999); // 定义一个分布范围
        std::string new_id = base_id;
        new_id = base_id + std::to_string(dis(gen)); // 添加随机生成的4位数
        return new_id;
}


Server::Server() = default;

std::shared_ptr<Client> Server::add_client(std::string id)
{
    if(clients.find(get_client(id)) != clients.end()){
        id = generate_unique_id(id);
    }
    const double coins = 5;
    auto new_client = std::make_shared<Client>(id, *this);
    clients.insert(std::make_pair(new_client, coins));
    return new_client;
}

std::shared_ptr<Client> Server::get_client(std::string id) const
{
    for(auto& [client, _] : clients){
        if(client->get_id() == id)
            return client;
    }
    return nullptr;
}

double Server::get_wallet(std::string id)
{
    for(auto& [client, coins] : clients){
        if(client->get_id() == id)
            return coins;
    }
    return -1;
}
#include <iostream>
bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value)
{
    std::regex trx_regex(R"(^(\w+)-(\w+)-([0-9]+(\.[0-9]+)?)$)");
    std::smatch match;

    if(!std::regex_match(trx, match, trx_regex)){
        throw std::runtime_error("Invalid transcation format");
    }


    sender = match[1];
    receiver = match[2];
    value = std::stod(match[3]);
    // std::cout<<sender<<" " <<receiver<< " "<< value<<" ******"<<std::endl;
    return true;
}


bool Server::add_pending_trx(std::string trx, std::string signature)
{
    std::string sender, receiver;
    double value;
    parse_trx(trx, sender, receiver, value);
    if(crypto::verifySignature(get_client(sender)->get_publickey(), trx, signature)){
        if(get_client(receiver)){
            if(get_wallet(sender) > value){
                pending_trxs.push_back(trx);
                clients[get_client(sender)] -= value;
                clients[get_client(receiver)] += value;
                return true;
            }
        }
    }
    return false;
}


size_t Server::mine()
{
    std::string mempool;
    for(auto& trx : pending_trxs){
        mempool += trx;
    }
    while(1){
        for(auto& client : clients){
            size_t rdnum = client.first->generate_nonce();
            if(crypto::sha256(mempool + std::to_string(rdnum)).substr(0, 10).find("000") !=std::string::npos){
                client.second += 6.25;
                pending_trxs.clear();
            }
            return rdnum;
        }
    }
}

